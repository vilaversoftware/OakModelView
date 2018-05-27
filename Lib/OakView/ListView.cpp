#include "ListView.h"

#include <QScrollArea>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QSpacerItem>
#include <QResizeEvent>
#include <QScrollBar>

#include <QDebug>

#include "OakModel.h"
#include "ListViewItem.h"

namespace Oak {
namespace View {

// =============================================================================
// (public)
ListView::ListView(QWidget *parent)
    : QFrame(parent)
{
    QString styleSheet = ".QFrame {"
                         "  border: 1px solid rgba(0, 0, 0, 255);"
                         "  border-radius: 4px;"
                         "}"
                         ".QPushButton {"
                         "  background: transparent;"
                         "}"
                         ".QPushButton:hover { "
                         "  border-radius: 3px;"
                         "  border: 1px solid rgba(120, 120, 120, 255);"
                         "}";
    styleSheet += createStyleSheep(QColor(50, 70, 200), 1);
    styleSheet += createStyleSheep(QColor(80, 150, 50), 2);


    m_scrollArea = new QScrollArea();
    m_scrollArea->setFrameShape(QFrame::NoFrame);
    m_scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_scrollArea->setStyleSheet(styleSheet);

    // Horizontal line between scrollable existing items and draggable new items
    auto lineA = new QFrame;
    lineA->setFrameShape(QFrame::HLine);
    lineA->setFrameShadow(QFrame::Sunken);

    // Available new items that can be dragged into place
    auto dragItems = new QWidget();
    m_dragLayout = new QGridLayout();
    m_dragLayout->setMargin(0);
    dragItems->setLayout(m_dragLayout);

    // TEST Buttons - Begin
    auto dragButton1 = new QPushButton("Drag 1");
    auto dragButton2 = new QPushButton("Drag 2");
    auto dragButton3 = new QPushButton("Drag 3");
    auto dragButton4 = new QPushButton("Drag 4");
    auto dragButton5 = new QPushButton("Drag 5");
    auto dragButton6 = new QPushButton("Drag 6");
    m_dragLayout->addWidget(dragButton1, 0, 0);
    m_dragLayout->addWidget(dragButton2, 0, 1);
    m_dragLayout->addWidget(dragButton3, 0, 2);
    m_dragLayout->addWidget(dragButton4, 1, 0);
    m_dragLayout->addWidget(dragButton5, 1, 1);
    m_dragLayout->addWidget(dragButton6, 1, 2);
    // TEST Buttons - End

    auto mainLayout = new QVBoxLayout();
    mainLayout->addWidget(m_scrollArea, 1);
    mainLayout->addWidget(lineA);
    mainLayout->addWidget(dragItems, 0);
    setLayout(mainLayout);
}

ListView::~ListView()
{

}

// =============================================================================
// (public)
void ListView::setOakModel(Model::OakModel *model)
{
    if (m_model == model) { return; }

    if (m_model) {
        // Disconnect the old model
        m_model->notifier_currentItemChanged.remove(this);
//        m_model->notifier_rootNodeDefChanged.remove(this);
//        m_model->notifier_rootNodeChanged.remove(this);
//        m_model->notifier_destroyed.remove(this);

        m_model->notifier_itemInserted.remove(this);
        m_model->notifier_itemMoved.remove(this);
        m_model->notifier_itemCloned.remove(this);
        m_model->notifier_itemRemoved.remove(this);

        m_model->notifier_itemValueChanged.remove(this);
    }

    // Change the model
    m_model = model;

    if (m_model) {
        // connect the new mobel
        m_model->notifier_currentItemChanged.add(this, &ListView::currentItemChanged);
//        m_model->notifier_rootNodeDefChanged.add(this, &ListView::updateTreeStructure);
//        m_model->notifier_rootNodeChanged.add(this, &ListView::updateTreeStructure);
//        m_model->notifier_destroyed.add(this, &ListView::modelDestroyed);

        m_model->notifier_itemInserted.add(this, &ListView::onItemInserted);
        m_model->notifier_itemMoved.add(this, &ListView::onItemMoved);
        m_model->notifier_itemCloned.add(this, &ListView::onItemCloned);
        m_model->notifier_itemRemoved.add(this, &ListView::onItemRemoved);

        m_model->notifier_itemValueChanged.add(this, &ListView::onItemValueChanged);
    }
}

// =============================================================================
// (public)
void ListView::setRootItem(const Model::Item &item)
{
    if (m_rootItem != nullptr) {
        // Do nothing if the root item is the same
        if (m_rootItem->item() == item) { return; }

        disconnect(m_rootItem, SIGNAL(onHeightChanged()), this, SLOT(adjustItemWidth()));
        //Clear existing item before creating a new one
    }
    m_rootItem = new ListViewItem(this, item, 0);
    m_scrollArea->setWidget(m_rootItem);
    m_rootItem->setFixedWidth(m_scrollArea->viewport()->width());

    connect(m_rootItem, SIGNAL(heightChanged(int)), this, SLOT(adjustItemWidth()), Qt::QueuedConnection);
}

// =============================================================================
// (public)
void ListView::currentItemChanged()
{
    ListViewItem * currentViewItem = getViewItem(m_model->currentItem());

    if (currentViewItem == m_currentViewItem) { return; }

    if (m_currentViewItem) {
        m_currentViewItem->clearCurrent();
        disconnect(m_currentViewItem, SIGNAL(destroyed()), this, SLOT(onCurrentItemViewDestoyed()));
    }

    m_currentViewItem = currentViewItem;

    if (m_currentViewItem) {
        m_currentViewItem->setCurrent();
        connect(m_currentViewItem, SIGNAL(destroyed()), this, SLOT(onCurrentItemViewDestoyed()));
    }
}

// =============================================================================
// (public)
void ListView::onItemInserted(const Model::Item &parentItem, int index)
{
    ListViewItem * viewItem = getViewItem(parentItem);
    if (viewItem != nullptr) {
        viewItem->onItemInserted(index);
    }
}

// =============================================================================
// (public)
void ListView::onItemMoved(const Model::Item &sourceParentItem, int sourceIndex, const Model::Item &targetParentItem, int targetIndex)
{
    onItemRemoved(sourceParentItem, sourceIndex);
    onItemInserted(targetParentItem, targetIndex);
}

// =============================================================================
// (public)
void ListView::onItemCloned(const Model::Item &sourceParentItem, int sourceIndex, const Model::Item &targetParentItem, int targetIndex)
{
    Q_UNUSED(sourceParentItem);
    Q_UNUSED(sourceIndex);
    onItemInserted(targetParentItem, targetIndex);
}

// =============================================================================
// (public)
void ListView::onItemRemoved(const Model::Item &parentItem, int index)
{
    ListViewItem * viewItem = getViewItem(parentItem);
    if (viewItem != nullptr) {
        viewItem->onItemRemoved(index);
    }
}

// =============================================================================
// (public)
void ListView::onItemValueChanged(const Model::Item &item, int valueIndex)
{
    if (item.def()->derivedIdValueDefIndex() == valueIndex) {
        // Child items can change when the derived definition change
        Model::Item pItem = item.parent();
        int index = pItem.childIndex(item);

        onItemRemoved(pItem, index);
        onItemInserted(pItem, index);

        currentItemChanged();
    } else if (item.def()->keyValueDefIndex() == valueIndex) {
        // Child items can change when the derived definition change
        ListViewItem * viewItem = getViewItem(item);
        if (viewItem != nullptr) {
            viewItem->updateLabel();
        }
    }
}

// =============================================================================
// (public)
int ListView::maxDepth() const
{
    return m_maxDepth;
}

// =============================================================================
// (public)
void ListView::setMaxDepth(int depth)
{
    if (m_maxDepth == depth) { return; }

    m_maxDepth = depth;

    // TODO: Update view and drag items when depth changes
}

// =============================================================================
// (public)
void ListView::resizeEvent(QResizeEvent *event)
{
    if (m_rootItem != nullptr) {
        m_rootItem->setFixedWidth(m_scrollArea->viewport()->width());
    }
    QWidget::resizeEvent(event);
}

// =============================================================================
// (protected)
ListViewItem * ListView::getViewItem(const Model::Item &item)
{
    if (m_rootItem == nullptr || item.isNull()) { return nullptr; }
    if (item == m_rootItem->item()) {
        return m_rootItem;
    }
    std::vector<Model::Item> itemList;
    itemList.push_back(item);

    Model::Item pItem = item.parent();
    while (pItem != m_rootItem->item()) {
        if (pItem.isNull()) { return nullptr; }
        itemList.push_back(pItem);
        pItem = pItem.parent();
    }

    ListViewItem * viewItem = m_rootItem;
    auto it = itemList.rbegin();
    while (it != itemList.rend()) {
        viewItem = viewItem->child(*it);
        it++;
    }

    return viewItem;
}

// =============================================================================
// (protected)
void ListView::createDragItems() const
{

}

// =============================================================================
// (protected)
void ListView::clearDragItems() const
{

}

// =============================================================================
// (protected)
QString ListView::createStyleSheep(QColor color, int level)
{
    QString colorStr = QString("%1,%2,%3").arg(color.red()).arg(color.green()).arg(color.blue());
    QString stylesheet = QString(".QFrame#level_%2 {"
                                 "  background: qlineargradient(x1:0, y1:1, x2:1, y2:0, stop: 0 rgba(%1, 100), stop: 0.4 rgba(%1, 100), stop: 1 white);"
                                 "}"
                                 ".QFrame#level_%2:hover {"
                                 "  background: qlineargradient(x1:0, y1:1, x2:1, y2:0, stop: 0 rgba(%1, 180), stop: 0.4 rgba(%1, 180), stop: 1 white);"
                                 "}"
                                 ".QPushButton#level_%2:hover { "
                                 "  background: rgba(%1, 100);"
                                 "}").arg(colorStr).arg(level);
    // "  border: 1px solid rgba(%1, 255);"
    return stylesheet;
}

// =============================================================================
// (protected slots)
void ListView::adjustItemWidth()
{
    if (m_rootItem != nullptr) {
        m_rootItem->setFixedWidth(m_scrollArea->viewport()->width());
    }
}

// =============================================================================
// (protected slots)
void ListView::onCurrentItemViewDestoyed()
{
    m_currentViewItem = nullptr;
}

} // namespace View
} // namespace Oak
