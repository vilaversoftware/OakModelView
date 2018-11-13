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

#include "Node.h"
#include "NodeDef.h"
#include "Entry.h"


namespace Oak::Model {

class OakModel;

// =============================================================================
// Class definition
// =============================================================================
class Item
{
public:
    Item();
    Item(const NodeDef* def, const Node &node, const OakModel* model = nullptr);
    Item(const Item &copy);
    Item(Item&& move);

    Item& operator=(const Item& copy);
    Item& operator=(Item&& move);

    bool operator==(const Item& _item) const;
    bool operator!=(const Item& _item) const;

    const Entry& operator()(const std::string &name) const;

    Item operator[](int index) const;

    bool isNull() const;
    bool isDefNull() const;
    bool isNodeNull() const;

    void clear();

    const Node &node() const { return m_node; }
    Node node() { return m_node; }

    const NodeDef* def() const { return m_def; }
    const OakModel* model() const;

    void setCurrent();

    std::vector<std::string> valueNameList() const;
    std::vector<std::string> childNameList() const;

    // ************* Entry Access *************
    int entryCount() const;
    bool hasEntry(const std::string &entryName) const;
    int entryIndex(const Entry& entry) const;

    const Entry& entryAt(int index) const;
    Entry& entryAt(int index);

    const Entry& entry(const std::string &entryName) const;
    Entry& entry(const std::string &entryName);

    typedef std::vector<Entry>::const_iterator entryIterator;

    const entryIterator entryBegin() const;
    Item::entryIterator entryBegin();

    const entryIterator entryEnd() const;
    entryIterator entryEnd();

    bool hasKey() const;
    const Entry& entryKey() const;

    bool hasVariants() const;
    const Entry& variantEntry() const;

    // ************* Child Item access *************
    int childCount() const;
    int childCount(const std::string &name) const;

    int childIndex(const Item &refChild) const;
    int childIndex(const std::string &name, const Item &refChild) const;

    Item childAt(int index) const;
    Item childAt(const std::string &name, int index) const;

    Item firstChild() const;
    Item firstChild(const std::string &name) const;

    Item lastChild() const;
    Item lastChild(const std::string &name) const;

    Item nextChild(const Item& refChild) const;
    Item nextChild(const std::string &name, const Item& refChild) const;

    Item previousChild(const Item& refChild) const;
    Item previousChild(const std::string &name, const Item& refChild) const;

    // ************* Parent Item access *************
    Item parent() const;

    // ************* Child Item edit *************
    bool canInsertChild(const std::string &name, int &index) const;

    Item insertChild(const std::string &name, int &index) const;

    bool canCloneChild(int &index, const Item &cloneItem) const;
    bool canCloneChild(const std::string &name, int &index, const Item &cloneItem) const;

    Item cloneChild(int &index, const Item &cloneItem) const;
    Item cloneChild(const std::string &name, int &index, const Item &cloneItem) const;

    bool canMoveChild(int &index, const Item &moveItem) const;
    bool canMoveChild(const std::string &name, int &index, const Item &moveItem) const;

    Item moveChild(int &index, const Item &moveItem) const;
    Item moveChild(const std::string &name, int &index, const Item &moveItem) const;

    bool canRemoveChild(int index) const;
    bool canRemoveChild(const std::string &name, int index) const;

    bool removeChild(int index) const;
    bool removeChild(const std::string &name, int index) const;


    // ************* Convert child index *************
    int convertChildIndexToUnnamed(const std::string &name, int index) const;
    int convertChildIndexToNamed(std::string &name, int index) const;

protected:
    inline void initEntryList() const;

    static void updateUniqueValues(const Item &item);

protected:
    const NodeDef* m_def;
    Node m_node;
    const OakModel* m_model;

    mutable std::vector<Entry> m_entryList;
};

} // namespace Oak::Model

