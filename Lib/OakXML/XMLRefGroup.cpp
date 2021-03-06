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

#ifdef XML_BACKEND

#include "XMLRefGroup.h"

namespace Oak::XML {

// =============================================================================
// (public)
RefGroup::RefGroup(RefUPtr eRef1, RefUPtr eRef2, RefUPtr eRef3, RefUPtr eRef4)
{
    if (eRef1) { m_references.push_back(std::move(eRef1)); }
    if (eRef2) { m_references.push_back(std::move(eRef2)); }
    if (eRef3) { m_references.push_back(std::move(eRef3)); }
    if (eRef4) { m_references.push_back(std::move(eRef4)); }
}

// =============================================================================
// (public)
RefGroup::RefGroup(const RefGroup &copy)
    : Ref()
{
    *this = copy;
}

// =============================================================================
// (public)
RefGroup::RefGroup(RefGroup&& move)
    : Ref()
{
    *this = std::move(move);
}

// =============================================================================
// (public)
RefGroup& RefGroup::operator=(const RefGroup &copy)
{
    m_references.clear();
    for (const auto& ref: copy.m_references)
    {
        m_references.push_back(ref->copy());
    }
    return *this;
}

// =============================================================================
// (public)
RefGroup& RefGroup::operator=(RefGroup &&move)
{
    m_references = std::move(move.m_references);
    return *this;
}

// =============================================================================
// (public)
RefUPtr RefGroup::copy() const
{
    return MakeUPtr(*this);
}

// =============================================================================
// (public)
RefGroupUPtr RefGroup::copyGroup() const
{
    return MakeUPtr(*this);
}

// =============================================================================
// (public)
Element RefGroup::getTarget(Element source, bool create) const
{
    Element refElement = source;
    for (const auto& eRef: m_references)
    {
        refElement = eRef->getTarget(refElement, create);
        if (refElement.isNull()) { return Element(); }
    }

    return refElement;
}

// =============================================================================
// (public)
void RefGroup::clearTarget(Element source, bool onlyIfEmpty) const
{
    if (source.isNull()) { return; }

    Element refElement = source;

    if (onlyIfEmpty) {
        // The clearRefElement() function is called for each ElementRef in m_references, with a valid Element.
        // The last one have to be called first so a list of valid elements is first created.
        std::vector<Element> elementList;
        elementList.push_back(source);

        Element element = source;
        auto it = m_references.begin();
        while (it != m_references.end())
        {
            element = it->get()->getTarget(element);
            if (element.isNull()) { break; }
            elementList.push_back(element);
            it++;
        }
        it--;
        // The iterator ends up pointing to the last element reference that gave a valid reference

        // Now calls that clearIfEmpty function on all valid element references, the last one first
        while (elementList.size() > 1) {
            elementList.pop_back();
            it->get()->clearTarget(elementList.back(), onlyIfEmpty);
        }
    } else {
        for (const auto& eRef: m_references)
        {
            eRef->clearTarget(refElement, onlyIfEmpty);
            refElement = eRef->getTarget(refElement);
            if (refElement.isNull()) { return; }
        }
    }

}

// =============================================================================
// (public)
Element RefGroup::getSource(Element target) const
{
    auto it = m_references.end();
    while (it != m_references.begin() && !target.isNull()) {
        it--;
        target = (*it)->getSource(target);
    }
    return target;
}

// =============================================================================
// (public)
void RefGroup::add(RefUPtr eRef)
{
    m_references.push_back(std::move(eRef));
}

// =============================================================================
// (public)
void RefGroup::clear()
{
    m_references.clear();
}

} // namespace Oak::XML

#endif // XML_BACKEND
