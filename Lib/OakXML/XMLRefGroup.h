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

#ifdef XML_BACKEND

#include <vector>
#include <stdarg.h>
#include <memory>

#include "XMLRef.h"

namespace Oak {
namespace XML {

class RefGroup;
typedef std::unique_ptr<RefGroup> RefGroupUPtr;

// =============================================================================
// Class definition
// =============================================================================
class RefGroup : public Ref
{
public:
    RefGroup(RefUPtr eRef1 = RefUPtr(), RefUPtr eRef2 = RefUPtr(), RefUPtr eRef3 = RefUPtr(), RefUPtr eRef4 = RefUPtr());
    RefGroup(const RefGroup& copy);
    RefGroup(RefGroup &&move);

    RefGroup& operator=(const RefGroup& copy);
    RefGroup& operator=(RefGroup&& move);

    virtual RefUPtr copy() const override;
    RefGroupUPtr copyGroup() const;

    virtual Element getTarget(Element source, bool create = false) const override;
    virtual void clearTarget(Element source, bool onlyIfEmpty = true) const override;

    virtual Element getSource(Element target) const override;

    virtual bool isNull() const { return false; }

    bool empty() const { return m_references.empty(); }
    int count() const { return (int)m_references.size(); }
    Ref* at(int index) { return m_references.at(index).get(); }
    template<typename T>
    T* at(int index) { return dynamic_cast<T*>(m_references.at(index).get()); }
    RefUPtr take(int index) { return std::move(m_references.at(index)); }

    void add(RefUPtr eRef);
    void clear();

    template<class... _Types> inline
    static typename RefGroupUPtr MakeUPtr(_Types&&... _Args)
    {
        return (RefGroupUPtr(new RefGroup(_STD forward<_Types>(_Args)...)));
    }

protected:
    std::vector<RefUPtr> m_references;

};

} // namespace XML
} // namespace Oak

#endif // XML_BACKEND