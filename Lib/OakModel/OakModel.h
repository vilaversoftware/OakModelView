/**
 * oakmodelview - version 0.1.0
 * --------------------------------------------------------
 * Copyright (C) 2017, by Mikkel Nøhr Løvgreen (mikkel@oakmodelview.com)
 * Report bugs and download new versions at http://oakmodelview.com/
 *
 * This library is distributed under the MIT License.
 * See accompanying file LICENSE in the root folder.
 */

#pragma once

#include "NodeDef.h"
#include "Item.h"
#include "CallbackFunctions.h"

#ifdef XML_BACKEND
#include "XMLDocument.h"
#endif // XML_BACKEND


namespace Oak::Model {

class ObserverInterface;
typedef std::unique_ptr<ObserverInterface> ObserverInterfaceUPtr;

// =============================================================================
// Class definition
// =============================================================================
class OakModel
{
public:
    OakModel();
    virtual ~OakModel();

    bool isNull() const;
    bool isDefNull() const;
    bool isNodeNull() const;

    bool createNewRootDocument(NodeData::Type backendType, bool setAsCurrent = true);
    void clearRoot();
    const Item& rootItem() const;

    const std::string &rootDefName() const;

    const NodeDef* rootNodeDef() const;
    void setRootNodeDef(NodeDefSPtr def);
    void setRootNodeDef(const NodeDef* def);

    void setRootNode(const NodeData &nodeData);

    void setRootItem(const Item& item);

    const Item& currentItem() const;
    const ItemIndex& currentItemIndex() const;
    void setCurrentItem(const Item &item, bool forceUpdate = false) const;

#ifdef XML_BACKEND
    const std::string& docFilePathXML() { return m_xmlDocFilePath; }
    void setDocFilePathXML(const std::string xmlDocFilePath) { m_xmlDocFilePath = xmlDocFilePath; }
    void setRootNodeXML(const NodeData &rootNodeData, bool setAsCurrent = true);
    bool loadRootNodeXML(const std::string& filePath, bool setAsCurrent = true);
    bool saveRootNodeXML(const std::string& filePath = "");
#endif // XML_BACKEND

    // Starting with the root NodeDef and look through all NodeDefs recursive
    //  untill a match is found
    const NodeDef *findNodeDef(const NodeData &nodeData) const;
    Item itemFromDataPtr(void * dPtr) const;

    ItemIndexUPtr convertItemIndexToNamed(const ItemIndex &itemIndex) const;
    ItemIndexUPtr convertItemIndexToUnnamed(const ItemIndex &itemIndex) const;

protected:
    void onItemInserteAfter(const ItemIndex& itemIndex) const;
    void onItemMoveAfter(const ItemIndex& sourceItemIndex, const ItemIndex& targetItemIndex) const;
    void onItemMoveBefore(const ItemIndex& sourceItemIndex, const ItemIndex& targetItemIndex) const;
    void onItemCloneAfter(const ItemIndex& sourceItemIndex, const ItemIndex& targetItemIndex) const;

    void onItemRemoveAfter(const ItemIndex& itemIndex) const;
    void onItemRemoveBefore(const ItemIndex& itemIndex) const;

    void onLeafChangeBefore(const ItemIndex& itemIndex, const std::string &valueName) const;
    void onLeafChangeAfter(const ItemIndex& itemIndex, const std::string &valueName) const;
    void onVariantLeafChangeAfter(const ItemIndex& itemIndex) const;
    void onKeyLeafChangeAfter(const ItemIndex& itemIndex) const;

    void createObservers();
    void clearObservers();

public:
    Callback notifier_currentItemChanged;
    Callback notifier_rootNodeDataChanged;
    Callback notifier_rootNodeDefChanged;

    Callback notifier_destroyed;

    Callback_ItemIndex notifier_itemInserteAfter;
    Callback_ItemIndexItemIndex notifier_itemMoveBefore;
    Callback_ItemIndexItemIndex notifier_itemMoveAfter;
    Callback_ItemIndexItemIndex notifier_itemCloneAfter;

    Callback_ItemIndex notifier_itemRemoveBefore;
    Callback_ItemIndex notifier_itemRemoveAfter;

    Callback_ItemIndexString notifier_leafChangeBefore;
    Callback_ItemIndexString notifier_leafChangeAfter;
    Callback_ItemIndex notifier_variantLeafChangeAfter;
    Callback_ItemIndex notifier_keyLeafChangeAfter;

protected:
    Item m_rootItem;
    mutable Item m_currentItem;
    mutable ItemIndexUPtr m_currentItemIndex;

    std::vector<ObserverInterfaceUPtr> m_observerList;

    // Used only to keep the definition alive (Smart Pointer)
    NodeDefSPtr m_def;

#ifdef XML_BACKEND
    NodeData m_rootNodeXML;
    std::string m_xmlDocFilePath;
    XML::Document m_xmlDoc;
#endif // XML_BACKEND

    friend class Item;
    friend class Leaf;
};

} // namespace Oak::Model

