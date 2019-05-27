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

#include "XMLChildRef.h"

#include "../ServiceFunctions/Assert.h"

namespace Oak::XML {

// =============================================================================
// (public)
ChildRef::ChildRef(std::string tagName, int index)
    : Ref(),
      m_tagName(tagName),
      m_index(index)
{
    ASSERT(m_index >= 0);
}

// =============================================================================
// (public)
ChildRef::ChildRef(const ChildRef& copy)
    : Ref()
{
    *this = copy;
}

// =============================================================================
// (public)
ChildRef::ChildRef(ChildRef&& move)
    : Ref()
{
    *this = std::move(move);
}

// =============================================================================
// (public)
ChildRef &ChildRef::operator=(const ChildRef& copy)
{
    m_tagName = copy.m_tagName;
    m_index = copy.m_index;
    return *this;
}

// =============================================================================
// (public)
ChildRef &ChildRef::operator=(ChildRef&& move)
{
    m_tagName = std::move(move.m_tagName);
    m_index = move.m_index;
    return *this;
}

// =============================================================================
// (public)
RefUPtr ChildRef::copy() const
{
    return MakeUPtr(*this);
}

// =============================================================================
// (public)
ChildRefUPtr ChildRef::copyChild() const
{
    return MakeUPtr(*this);
}

// =============================================================================
// (public)
Element ChildRef::getTarget(Element source, bool create) const
{
    if (source.isNull()) { return Element(); }

    int index = 0;
    Element refElement = source.firstChild(m_tagName);

    while (index < m_index && !refElement.isNull()) {
        refElement = refElement.nextSibling(m_tagName);
        index++;
    }

    // Creats the reference element if it is missing
    if (create && refElement.isNull() && !m_tagName.empty()) {
        index--; // Last element was not found
        refElement = source.lastChild(m_tagName);
        while (index < m_index) {
            refElement = source.insertAfter(m_tagName, refElement);
            index++;
        }
    }

    return refElement;
}

// =============================================================================
// (public)
void ChildRef::clearTarget(Element source, bool onlyIfEmpty) const
{
    Element refElement = getTarget(source);

    if (refElement.isNull()) { return; }

    if (!onlyIfEmpty || refElement.empty()) {
        source.removeChild(refElement);
    }
}

// =============================================================================
// (public)
Element ChildRef::getSource(Element target) const
{
    if (target.isNull()) { return Element(); }

    return target.parentElement();
}

} // namespace Oak::XML

#endif // XML_BACKEND
