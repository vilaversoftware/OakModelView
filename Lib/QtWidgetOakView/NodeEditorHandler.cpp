/*
 * OakModelView (http://oakmodelview.com/)
 * Author: Mikkel Nøhr Løvgreen (mikkel@oakmodelview.com)
 * ------------------------------------------------------------------------
 * Licensed to Vilaversoftware IVS who licenses this file to you under the
 * Apache License, Version 2.0 (the "License"); you may not use this file
 * except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "NodeEditorHandler.h"

#include <QLabel>
#include <QGridLayout>
#include <QSpacerItem>


namespace Oak::View::QtWidgets {

// =============================================================================
// (public)
NodeEditorHandler::NodeEditorHandler(QObject *parent, const Model::Node &node)
    : QObject(parent), m_node(node)
{
}

// =============================================================================
// (public)
NodeEditorHandler::~NodeEditorHandler()
{
    clearEditor();
}

// =============================================================================
// (public)
bool NodeEditorHandler::operator==(const Model::NodeDef* nodeDef)
{
    return m_node.def() == nodeDef;
}

// =============================================================================
// (public)
bool NodeEditorHandler::operator!=(const Model::NodeDef* nodeDef)
{
    return m_node.def() != nodeDef;
}

// =============================================================================
// (public)
const Model::NodeDef* NodeEditorHandler::nodeDef() const
{
    return m_node.def();
}

// =============================================================================
// (public)
QWidget* NodeEditorHandler::getEditor()
{
    createEditor();
    return m_editor;
}

// =============================================================================
// (public)
bool NodeEditorHandler::setNode(const Model::NodeData &nodeData)
{
    if (!m_node.def()->validate(nodeData)) { return false; }

    m_node = Model::Node(m_node.def(), nodeData, m_node.model());

    foreach (ValueEditorHandler* vHandler, m_valueEditorMap) {
        vHandler->setNode(nodeData);
    }

    foreach (ContainerEditorHandler* cHandler, m_containerEditorMap) {
        cHandler->setNode(nodeData);
    }

    return true;
}

// =============================================================================
// (public)
void NodeEditorHandler::updateEditorValue(const std::string &name)
{
    m_valueEditorMap[name]->updateEditorValue();
}

// =============================================================================
// (public)
void NodeEditorHandler::updateEditorValues()
{
    foreach (ValueEditorHandler* vHandler, m_valueEditorMap) {
        vHandler->updateLabelValue();
        vHandler->updateEditorValue();
    }

    foreach (ContainerEditorHandler* cHandler, m_containerEditorMap) {
        cHandler->updateEditor();
    }
}

// =============================================================================
// (public)
void NodeEditorHandler::getEditorValues()
{
    foreach (ValueEditorHandler* vHandler, m_valueEditorMap) {
        vHandler->getEditorValue();
    }
}

// =============================================================================
// (public)
void NodeEditorHandler::clearEditor()
{
    if (m_editor) {
        if (m_editor->parent() == nullptr) {
            // Delete the editor if it has no parent
            delete m_editor;
        } else {
            // Disconnect any signal from the m_editor to this object
            disconnect(m_editor, nullptr, this, nullptr);
        }
        m_editor = nullptr;
    }
}

// =============================================================================
// (protected)
void NodeEditorHandler::createEditor()
{
    if (m_editor != nullptr) { return; }

    m_editor = new QWidget();
    QGridLayout* layout = new QGridLayout();
    layout->setSpacing(6);

    int row = 0;

    Model::Node::LeafIterator vIt = m_node.leafBegin();
    Model::Node::LeafIterator vItEnd = m_node.leafEnd();
    while (vIt != vItEnd) {
        if (!m_valueEditorMap.contains(vIt->name())) {
            auto vEditor = new ValueEditorHandler(this, *vIt);
            m_valueEditorMap.insert(vIt->name(), vEditor);
            // TODO: Connect signals
        }
        ValueEditorHandler* vEditor = m_valueEditorMap[vIt->name()];

        if (vEditor->getLabel()) {
            layout->addWidget(vEditor->getLabel(), row, 0);
            layout->addWidget(vEditor->getEditor(), row, 1);
        } else {
            layout->addWidget(vEditor->getEditor(), row, 0, 1, 2);
        }
        row++;
        vIt++;
    }

    auto cList = m_node.def()->containerList();
    for (const Model::ContainerDef* containerPtr: cList)
    {
        std::string id = containerPtr->containerDef()->name();
        if (!m_containerEditorMap.contains(id)) {
            auto cHandler = new ContainerEditorHandler(m_node, id, this);
            m_containerEditorMap.insert(id, cHandler);
        }

        layout->addWidget(m_containerEditorMap[id]->getEditor(), row++, 0, 1, 2);
    }

    if (row == 0) {
        std::string nodeName = m_node.def()->displayName();
        QLabel* emptyLabel = new QLabel(QString("Node %1 is empty").arg(QString::fromStdString(nodeName)));
        layout->addWidget(emptyLabel);
    } else {
        layout->addItem(new QSpacerItem(0,0, QSizePolicy::Preferred, QSizePolicy::Expanding), layout->rowCount(), 0);
    }

    m_editor->setLayout(layout);

    connect(m_editor, SIGNAL(destroyed(QObject*)), this, SLOT(onEditorDestroyed()));
}

// =============================================================================
// (protected slots)
void NodeEditorHandler::onEditorDestroyed()
{
    m_editor = nullptr;
}

} // namespace Oak::View::QtWidgets

