/**
 * oakmodelview - version 0.1.0
 * --------------------------------------------------------
 * Copyright (C) 2017, by Mikkel Nøhr Løvgreen (mikkel@oakmodelview.com)
 * Report bugs and download new versions at http://oakmodelview.com/
 *
 * This library is distributed under the MIT License.
 * See accompanying file LICENSE in the root folder.
 */

#include "QueryBuilder.h"

#include "ItemQueryChildren.h"
#include "ItemQueryParent.h"
#include "ItemQuerySiblings.h"
#include "LeafQuery.h"

#include "../ServiceFunctions/Trace.h"
#include "OakModelServiceFunctions.h"

namespace Oak::Model {

// =============================================================================
// (public)
QueryBuilder::QueryBuilder()
{

}

// =============================================================================
// (public)
ItemQueryUPtr QueryBuilder::UPtr()
{
    return std::move(m_itemQuery);
}

// =============================================================================
// (public)
LeafQuerySPtr QueryBuilder::leafSPtr(const std::string &leafName)
{
    return LeafQuery::create(std::move(m_itemQuery), leafName);
}

// =============================================================================
// (public)
QueryBuilderSPtr QueryBuilder::children(const std::string &nodeName, bool invertOrder)
{
    if (m_itemQuery) {
        if (invertOrder) {
            ItemQueryUPtr tempQuery = std::make_unique<ItemQueryChildren>(nodeName);
            tempQuery->addChildQuery(std::move(m_itemQuery));
            m_itemQuery = std::move(tempQuery);
        } else {
            m_itemQuery->addChildQuery(std::make_unique<ItemQueryChildren>(nodeName));
        }
    } else {
        m_itemQuery = std::make_unique<ItemQueryChildren>(nodeName);
    }
    return m_thisWPtr.lock();
}

// =============================================================================
// (public)
QueryBuilderSPtr QueryBuilder::parent(bool invertOrder)
{
    if (m_itemQuery) {
        if (invertOrder) {
            ItemQueryUPtr tempQuery = std::make_unique<ItemQueryParent>();
            tempQuery->addChildQuery(std::move(m_itemQuery));
            m_itemQuery = std::move(tempQuery);
        } else {
            m_itemQuery->addChildQuery(std::make_unique<ItemQueryParent>());
        }
    } else {
        m_itemQuery = std::make_unique<ItemQueryParent>();
    }
    return m_thisWPtr.lock();
}

// =============================================================================
// (public)
QueryBuilderSPtr QueryBuilder::siblings(bool invertOrder)
{
    if (m_itemQuery) {
        if (invertOrder) {
            ItemQueryUPtr tempQuery = std::make_unique<ItemQuerySiblings>();
            tempQuery->addChildQuery(std::move(m_itemQuery));
            m_itemQuery = std::move(tempQuery);
        } else {
            m_itemQuery->addChildQuery(std::make_unique<ItemQuerySiblings>());
        }
    } else {
        m_itemQuery = std::make_unique<ItemQuerySiblings>();
    }
    return m_thisWPtr.lock();
}

// =============================================================================
// (public)
QueryBuilderSPtr QueryBuilder::createChildren(const std::string &nodeName)
{
    QueryBuilderSPtr sPtr = std::make_shared<QueryBuilder>();
    sPtr->children(nodeName);
    sPtr->m_thisWPtr = sPtr;
    return sPtr;
}

// =============================================================================
// (public)
QueryBuilderSPtr QueryBuilder::createParent()
{
    QueryBuilderSPtr sPtr = std::make_shared<QueryBuilder>();
    sPtr->parent();
    sPtr->m_thisWPtr = sPtr;
    return sPtr;
}

// =============================================================================
// (public)
QueryBuilderSPtr QueryBuilder::createSiblings()
{
    QueryBuilderSPtr sPtr = std::make_shared<QueryBuilder>();
    sPtr->siblings();
    sPtr->m_thisWPtr = sPtr;
    return sPtr;
}

// =============================================================================
// (public)
QueryBuilderSPtr QueryBuilder::createInverse(const ItemQuery &query, const NodeDef *sourceNodeDef)
{
    ASSERT(sourceNodeDef);
    QueryBuilderSPtr sPtr = std::make_shared<QueryBuilder>();
    sPtr->m_thisWPtr = sPtr;

    const ItemQuery * q = &query;
    const NodeDef *nodeDef = sourceNodeDef;

    while (q != nullptr) {
        const ItemQueryChildren * itemQueryChildren = dynamic_cast<const ItemQueryChildren *>(q);
        if (itemQueryChildren) {
            sPtr->parent(true);
        }
        const ItemQueryParent * itemQueryParent = dynamic_cast<const ItemQueryParent *>(q);
        if (itemQueryParent) {
            sPtr->children(nodeDef->name(), true);
        }
        const ItemQuerySiblings * itemQuerySiblings = dynamic_cast<const ItemQuerySiblings *>(q);
        if (itemQuerySiblings) {
            sPtr->siblings(true);
        }

        nodeDef = q->_nodeDef(nodeDef);
        q = q->childQuery();
    }

    return sPtr;
}

// =============================================================================
// (public)
LeafQuerySPtr QueryBuilder::createLeaf(const std::string &leafName)
{
    return LeafQuery::create(leafName);
}

// =============================================================================
// (public)
ItemQueryUPtr QueryBuilder::duplicate(const ItemQueryUPtr &c)
{
    {
        const ItemQueryChildren * itemQuery = dynamic_cast<const ItemQueryChildren * >(c.get());
        if (itemQuery) {
            return ItemQueryUPtr(new ItemQueryChildren(*itemQuery));
        }
    }

    {
        const ItemQueryParent * itemQuery = dynamic_cast<const ItemQueryParent * >(c.get());
        if (itemQuery) {
            return ItemQueryUPtr(new ItemQueryParent(*itemQuery));
        }
    }

    {
        const ItemQuerySiblings * itemQuery = dynamic_cast<const ItemQuerySiblings * >(c.get());
        if (itemQuery) {
            return ItemQueryUPtr(new ItemQuerySiblings(*itemQuery));
        }
    }

    return ItemQueryUPtr(new ItemQuery(*c.get()));
}

// =============================================================================
// (public)
ItemQueryUPtr QueryBuilder::createItemQuery(const std::string& queryString)
{
    std::vector<std::string> strList = split(queryString, ';', true);

    QueryBuilderSPtr builder = std::make_shared<QueryBuilder>();

    for (const std::string& str: strList) {

        auto it1 = std::find(str.begin(), str.end(), '{');
        auto it2 = std::find(str.begin(), str.end(), '}');

        std::string type(str.begin(), it1);

        if (type == "C" || type == "Children") {
            ASSERT(it1 != it2);
            std::string name(it1+1, it2);
            builder->children(name);
        } else if (type == "P" || type == "Parent") {
            builder->parent();
        } else if (type == "S" || type == "Siblings") {
            builder->siblings();
        }
    }
    return builder->UPtr();
}

// =============================================================================
// (public)
LeafQuerySPtr QueryBuilder::createLeafQuery(const std::string& queryString)
{
    if (queryString.empty()) { return LeafQuerySPtr(); }

    auto it = std::find(queryString.rbegin(), queryString.rend(), ';');

    if (it == queryString.rend()) {
        // No ItemQuery part
        return createLeaf(queryString);
    }

    size_t index = static_cast<size_t>(queryString.rend() - it);
    std::string itemQueryString = queryString.substr(0, index-1);
    std::string leafName = queryString.substr(index);


    return LeafQuery::create(createItemQuery(itemQueryString), leafName);
}

} // namespace Oak::Model

