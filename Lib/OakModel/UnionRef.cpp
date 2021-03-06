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

#include "UnionRef.h"
#include "UnionValue.h"


#include "../ServiceFunctions/Assert.h"


namespace Oak::Model {

// =============================================================================
// (public)
UnionRef::UnionRef()
    : t(UnionType::Undefined)
{
    r.i = nullptr;
}

// =============================================================================
// (public)
UnionRef::UnionRef(const char *c)
    : t(UnionType::Char)
{
    r.c = c;
}

// =============================================================================
// (public)
UnionRef::UnionRef(const bool &b)
    : t(UnionType::Bool)
{
    r.b = const_cast<bool*>(&b);
}

// =============================================================================
// (public)
UnionRef::UnionRef(const int &i)
    : t(UnionType::Integer)
{
    r.i = &i;
}

// =============================================================================
// (public)
UnionRef::UnionRef(const double &d)
    : t(UnionType::Double)
{
    r.d = &d;
}

// =============================================================================
// (public)
UnionRef::UnionRef(const std::string &s)
    : t(UnionType::String)
{
    r.s = &s;
}

// =============================================================================
// (public)
UnionRef::UnionRef(const DateTime &dt)
    : t(UnionType::DateTime)
{
    r.dt = &dt;
}

// =============================================================================
// (public)
UnionRef::UnionRef(const UnionValue &v)
    : t(v.type())
{
    if (t == UnionType::Undefined) {
       r.i = nullptr;
    } else if (t == UnionType::Bool) {
        r.b = const_cast<bool*>(&v.v.b);
    } else if (t == UnionType::Integer) {
        r.i = const_cast<int*>(&v.v.i);
    } else if (t == UnionType::Double) {
        r.d = const_cast<double*>(&v.v.d);
    } else if (t == UnionType::String) {
        r.s = v.v.s;
    } else if (t == UnionType::DateTime) {
        r.dt = v.v.dt;
    } else {
        ASSERT(false);
    }
}

// =============================================================================
// (public)
UnionRef::UnionRef(const UnionRef &copy)
    : t(copy.t)
{
    memcpy(&r, &copy.r, sizeof(r));
}

// =============================================================================
// (public)
UnionRef::~UnionRef()
{

}

// =============================================================================
// (public)
UnionRef &UnionRef::operator=(const UnionRef &copy)
{
    bool result = copy.get(*this);
    ASSERT(result);
    return *this;
}

// =============================================================================
// (public)
bool UnionRef::operator==(const UnionRef &value) const
{
    return compare(value, false) == 0;
}

// =============================================================================
// (public)
bool UnionRef::operator!=(const UnionRef &value) const
{
    return compare(value, false) != 0;
}

// =============================================================================
// (public)
bool UnionRef::operator>(const UnionRef &value) const
{
    return compare(value, false) > 0;
}

// =============================================================================
// (public)
bool UnionRef::operator>=(const UnionRef &value) const
{
    return compare(value, false) >= 0;
}

// =============================================================================
// (public)
bool UnionRef::operator<(const UnionRef &value) const
{
    return compare(value, false) < 0;
}

// =============================================================================
// (public)
bool UnionRef::operator<=(const UnionRef &value) const
{
    return compare(value, false) <= 0;
}

// =============================================================================
// (public)
int UnionRef::compare(UnionRef value, bool allowConversion, Conversion *properties) const
{
    if (Union::GetValueType(t) != Union::GetValueType(value.t)) {
        if (!allowConversion) { return -1000; }

        UnionValue uValue(*this);
        UnionRef uRef(uValue);

        if (!value.get(uRef, allowConversion, properties)) { return -1000; }
        value = uRef;
    }

    switch (value.t) {
        case UnionType::Undefined:
            return 0;
        case UnionType::Bool:
            return (*r.b == *value.r.b) ? 0 : (*r.b) ? 1 : -1;
        case UnionType::Integer:
            return (*r.i == *value.r.i) ? 0 : (*r.i > *value.r.i) ? 1 : -1;
        case UnionType::Double:
            return (std::abs(*r.d - *value.r.d) < 1.0e-8) ? 0 : (*r.d > *value.r.d) ? 1 : -1;
        case UnionType::String:
            if (value.t == UnionType::String) {
                return r.s->compare(*value.r.s);
            } else {
                return r.s->compare(value.r.c);
            }
        case UnionType::Char:
            if (value.t == UnionType::String) {
                return std::strcmp(r.c, value.r.s->c_str());
            } else {
                return std::strcmp(r.c, value.r.c);
            }
        case UnionType::DateTime:
            return *r.dt == *value.r.dt ? 0 : (r.dt > value.r.dt) ? 1 : -1;
        default:
            ASSERT(false);
    }
    return -1000;
}

// =============================================================================
// (public)
bool UnionRef::isEqual(const UnionRef &value, bool allowConversion, Conversion *properties) const
{
    return compare(value, allowConversion, properties) == 0;
}

// =============================================================================
// (public)
bool UnionRef::isNull() const
{
    return t == UnionType::Undefined;
}

// =============================================================================
// (public)
UnionType UnionRef::type() const
{
    return t;
}

// =============================================================================
// (public)
bool UnionRef::getBool() const
{
    ASSERT(t == UnionType::Bool);
    return *r.b;
}

// =============================================================================
// (public)
int UnionRef::getInt() const
{
    ASSERT(t == UnionType::Integer);
    return *r.i;
}

// =============================================================================
// (public)
double UnionRef::getDouble() const
{
    ASSERT(t == UnionType::Double);
    return *r.d;
}

// =============================================================================
// (public)
const std::string& UnionRef::getCString() const
{
    ASSERT(t == UnionType::String);
    return *r.s;
}

// =============================================================================
// (public)
std::string &UnionRef::getString()
{
    ASSERT(t == UnionType::String);
    return *const_cast<std::string*>(r.s);
}

// =============================================================================
// (public)
const DateTime &UnionRef::getDateTime() const
{
    ASSERT(t == UnionType::DateTime);
    return *r.dt;
}

// =============================================================================
// (public)
bool UnionRef::canGet(UnionRef &target, bool allowConversion, Conversion *properties) const
{
    if (t == UnionType::Undefined) { return false; }
    if (!allowConversion && target.t != t) { return false; }

    switch (target.t) {
        case UnionType::Undefined:
            return false;
        case UnionType::Char:
            return false;
        case UnionType::Bool:
            return canGet(*const_cast<bool*>(target.r.b), allowConversion, properties);
        case UnionType::Integer:
            return canGet(*const_cast<int*>(target.r.i), allowConversion, properties);
        case UnionType::Double:
            return canGet(*const_cast<double*>(target.r.d), allowConversion, properties);
        case UnionType::String:
            return canGet(*const_cast<std::string*>(target.r.s), allowConversion, properties);
        case UnionType::DateTime:
            return canGet(*const_cast<DateTime*>(target.r.dt), allowConversion, properties);
        default:
            ASSERT(false);
            return false;
    }
}

// =============================================================================
// (public)
bool UnionRef::get(UnionRef &target, bool allowConversion, Conversion *properties) const
{
    if (t == UnionType::Undefined) { return false; }
    if (!allowConversion && target.t != t) { return false; }

    switch (target.t) {
        case UnionType::Undefined:
            return false;
        case UnionType::Char:
            return false;
        case UnionType::Bool:
            return get(*const_cast<bool*>(target.r.b), allowConversion, properties);
        case UnionType::Integer:
            return get(*const_cast<int*>(target.r.i), allowConversion, properties);
        case UnionType::Double:
            return get(*const_cast<double*>(target.r.d), allowConversion, properties);
        case UnionType::String:
            return get(*const_cast<std::string*>(target.r.s), allowConversion, properties);
        case UnionType::DateTime:
            return get(*const_cast<DateTime*>(target.r.dt), allowConversion, properties);
        default:
            ASSERT(false);
            return false;
    }
}

// =============================================================================
// (public)
bool UnionRef::canGet(UnionValue &target, bool allowConversion, Conversion *properties) const
{
    UnionRef uRef(target);
    return canGet(uRef, allowConversion, properties);
}

// =============================================================================
// (public)
bool UnionRef::get(UnionValue &target, bool allowConversion, Conversion *properties) const
{
    UnionRef uRef(target);
    return get(uRef, allowConversion, properties);
}

} // namespace Oak::Model

