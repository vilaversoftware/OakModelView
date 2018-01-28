/**
 * oakmodelview - version 0.1.0
 * --------------------------------------------------------
 * Copyright (C) 2017, by Mikkel Nøhr Løvgreen (mikkel@oakmodelview.com)
 * Report bugs and download new versions at http://oakmodelview.com/
 *
 * This library is distributed under the MIT License.
 * See accompanying file LICENSE in the root folder.
 */

#include "NodeDefBuilder.h"

#include "ValueDefBuilder.h"

namespace Oak {
namespace Model {

// =============================================================================
// (public)
NodeDefSPtr NodeDefBuilder::Make(const std::string & name)
{
    NodeDefSPtr nodeDef = NodeDef::MakeSPtr(name);

#ifdef XML_BACKEND
    if (XML::Element::validateTagName(name)) {
        nodeDef->m_tagName = name;
    }
#endif // XML_BACKEND

    return nodeDef;
}

// =============================================================================
// (public)
bool NodeDefBuilder::addValueDef(NodeDefSPtr nodeDef, ValueDefUPtr valueDef)
{
    if (!nodeDef) { return false; }
    if (!valueDef) { return false; }

    // A NodeDef can only have
    if (hasValueI(nodeDef, valueDef)) { return false; }

    nodeDef->m_valueList.push_back(std::move(valueDef));

    return true;
}

// =============================================================================
// (public)
bool NodeDefBuilder::addValueDefAsKey(NodeDefSPtr nodeDef, ValueDefUPtr valueDefKey)
{
    if (!nodeDef) { return false; }
    if (!valueDefKey) { return false; }

    // Derived node definitions inherate its node id value from its base and can not have it's own
    if (nodeDef->hasDerivedBase()) { return false; }

    ValueSettings &settings = VDB::settings(valueDefKey);
    if (!settings.isUniqueSet()) {
        settings.setUnique(true);
    }
    if (!settings.isRequiredSet()) {
        settings.setRequired(true);
    }

    if (addValueDef(nodeDef, std::move(valueDefKey))) {
        int index = nodeDef->valueCount()-1;
        setKeyValueThisAndDerived(nodeDef, index);
        return true;
    }
    return false;
}

// =============================================================================
// (public)
bool NodeDefBuilder::addValueDefAsDerivedId(NodeDefSPtr nodeDef, ValueDefUPtr valueDefDerivedId)
{
    if (!nodeDef) { return false; }
    if (!valueDefDerivedId) { return false; }

    // The node must have an derived id
    if (nodeDef->derivedId().isNull()) { return false; }

    // The value type of the derivedId and the derivedIdValue must match
    if (nodeDef->derivedId().type() != valueDefDerivedId->valueTemplate().type()) { return false; }

    // Derived node definitions inherate its derived id value from its base and can not have it's own
    if (nodeDef->hasDerivedBase()) { return false; }

    //
    std::vector<UnionRef> optionList;
    nodeDef->derivedIdListAll(optionList);
    VDB::setStaticOptions(valueDefDerivedId, optionList);
    VDB::settings(valueDefDerivedId).setOptionsOnly(true);
    if (!valueDefDerivedId->hasDefaultValue()) {
        VDB::setDefaultValue(valueDefDerivedId, nodeDef->derivedId());
    }

    if (addValueDef(nodeDef, std::move(valueDefDerivedId))) {
        int index = nodeDef->valueCount()-1;
        setDerivedIdValueThisAndDerived(nodeDef, index);
        return true;
    }
    return false;
}

// =============================================================================
// (public)
ValueDefUPtr NodeDefBuilder::takeValueDef(NodeDefSPtr nodeDef, const std::string &valueName)
{
    if (!nodeDef || valueName.empty()) { return ValueDefUPtr(); }

    auto it = nodeDef->m_valueList.begin();
    while (it != nodeDef->m_valueList.end()) {
        if ((*it)->name() == valueName) {
            ValueDefUPtr movedValue(std::move(*it));
            nodeDef->m_valueList.erase(it);
            return std::move(movedValue);
        }
    }

    return ValueDefUPtr();
}

// =============================================================================
// (public)
bool NodeDefBuilder::addContainerDef(NodeDefSPtr nodeDef, ContainerDefUPtr cDef)
{
    if (!nodeDef || !cDef) {
        assert(false);
        return false;
    }

    // Check if the NodeDef will be referenced twice (Not sure this is needed)
    if (hasContainerI(nodeDef, cDef)) { return false; }

    cDef->m_hostDef = nodeDef;

    nodeDef->m_containerList.push_back(std::move(cDef));

    // Clear the list of existing containers
    if (nodeDef->m_containerGroup) {
        nodeDef->m_containerGroup->updateContainerList();
    }

    return true;
}

// =============================================================================
// (public)
ContainerDefUPtr NodeDefBuilder::takeContainerDef(NodeDefSPtr nodeDef, const std::string &name)
{
    if (!nodeDef || name.empty()) { return ContainerDefUPtr(); }

    auto it = nodeDef->m_containerList.begin();
    while (it != nodeDef->m_containerList.end()) {
        if ((*it)->containerDef()->name() == name) {
            ContainerDefUPtr movedContainer(std::move(*it));
            nodeDef->m_containerList.erase(it);

            // Clear the list of existing containers
            if (nodeDef->m_containerGroup) {
                nodeDef->m_containerGroup->updateContainerList();
            }

            return std::move(movedContainer);
        }
    }

    return ContainerDefUPtr();
}

// =============================================================================
// (public)
bool NodeDefBuilder::setDisplayName(NodeDefSPtr nodeDef, const std::string& displayName)
{
    if (!nodeDef) { return false; }

    nodeDef->m_displayName = displayName;

    return true;
}

#ifdef XML_BACKEND
// =============================================================================
// (public)
bool NodeDefBuilder::setTagName(NodeDefSPtr nodeDef, const std::string &tagName)
{
    if (!nodeDef) { return false; }
    if (!XML::Element::validateTagName(tagName)) { return false; }
    if (nodeDef->m_tagName == tagName) { return true; }

    NodeDefSPtr derivedRoot = nodeDef;
    while (derivedRoot->hasDerivedBase()) {
        derivedRoot = derivedRoot->m_derivedBase.lock();
    }

    setTagNameThisAndDerived(derivedRoot, tagName);

    return true;
}
#endif // XML_BACKEND

// =============================================================================
// (protected)
bool NodeDefBuilder::hasValueI(NodeDefSPtr nodeDef, const ValueDefUPtr& valueDef)
{
    if (!nodeDef) { return false; }

    // Check ValueDefs on it's base NodeDefs
    const NodeDef* ni = nodeDef.get();
    while (ni->hasDerivedBase()) {
        ni = ni->derivedBase();
        for (const auto& vi: nodeDef->m_valueList)
        {
            if (vi->name() == valueDef->name()) {
                return true;
            }
        }
    }
    // Check ValueDefs on this NodeDef
    // Check ValueDefs on it's derived NodeDefs
    return hasValueIThisAndDerived(nodeDef, valueDef);
}

// =============================================================================
// (protected)
bool NodeDefBuilder::hasValueIThisAndDerived(NodeDefSPtr nodeDef, const ValueDefUPtr &valueDef)
{
    for (const auto& vi: nodeDef->m_valueList)
    {
        if (vi->name() == valueDef->name()) {
            return true;
        }
    }
    if (nodeDef->hasDerivedDiviations()) {
        for (NodeDefSPtr ni: nodeDef->m_derivedDirectList)
        {
            if (hasValueIThisAndDerived(ni, valueDef)) {
                return true;
            }
        }
    }
    return false;
}

// =============================================================================
// (protected)
void NodeDefBuilder::setKeyValueThisAndDerived(NodeDefSPtr nodeDef, int index)
{
    nodeDef->m_keyValueDefIndex = index;
    if (nodeDef->hasDerivedDiviations()) {
        for (NodeDefSPtr ni: nodeDef->m_derivedDirectList)
        {
            setKeyValueThisAndDerived(ni, index);
        }
    }
}

// =============================================================================
// (protected)
void NodeDefBuilder::setDerivedIdValueThisAndDerived(NodeDefSPtr nodeDef, int index)
{
    nodeDef->m_derivedIdValueDefIndex = index;
    if (nodeDef->hasDerivedDiviations()) {
        for (NodeDefSPtr ni: nodeDef->m_derivedDirectList)
        {
            setDerivedIdValueThisAndDerived(ni, index);
        }
    }
}

// =============================================================================
// (protected)
bool NodeDefBuilder::hasContainerI(NodeDefSPtr nodeDef, const ContainerDefUPtr &cDef)
{
    // Check ValueDefs on it's base NodeDefs
    const NodeDef* ni = nodeDef.get();
    while (ni->hasDerivedBase()) {
        ni = ni->derivedBase();
        for (const auto& ci: nodeDef->m_containerList)
        {
            if (ci->m_containerDef->name() == cDef->m_containerDef->name()) {
                return true;
            }
        }
    }
    // Check ValueDefs on this NodeDef
    // Check ValueDefs on it's derived NodeDefs
    return hasContainerIThisAndDerived(nodeDef, cDef);
}

// =============================================================================
// (protected)
bool NodeDefBuilder::hasContainerIThisAndDerived(NodeDefSPtr nodeDef, const ContainerDefUPtr &cDef)
{
    for (const auto& ci: nodeDef->m_containerList)
    {
        if (ci->m_containerDef->name() == cDef->m_containerDef->name()) {
            return true;
        }
    }
    if (nodeDef->hasDerivedDiviations()) {
        for (NodeDefSPtr ni: nodeDef->m_derivedDirectList)
        {
            if (hasContainerIThisAndDerived(ni, cDef)) {
                return true;
            }
        }
    }
    return false;
}

#ifdef XML_BACKEND
// =============================================================================
// (protected)
void NodeDefBuilder::setTagNameThisAndDerived(NodeDefSPtr nodeDef, const std::string &tagName)
{
    nodeDef->m_tagName = tagName;
    if (nodeDef->hasDerivedDiviations()) {
        for (NodeDefSPtr ni: nodeDef->m_derivedDirectList)
        {
            setTagNameThisAndDerived(ni, tagName);
        }
    }
}

#endif // XML_BACKEND

} // namespace Model
} // namespace Oak