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

#include "ConvertFunctions.h"

#include <iomanip>
#include <ctime>
#include <sstream>
#include "../ServiceFunctions/Assert.h"


namespace Oak::Model {

// =============================================================================
// convert()
// =============================================================================

// =============================================================================
// (public)
bool convert(bool& dest, int src, Conversion* )
{
    dest = src != 0;
    return true;
}

// =============================================================================
// (public)
bool convert(int& dest, bool src, Conversion* )
{
    dest = src ? 1 : 0;
    return true;
}

// =============================================================================
// (public)
bool convert(bool&, double, Conversion* )
{
    return false;
}

// =============================================================================
// (public)
bool convert(double&, bool, Conversion* )
{
    return false;
}

// =============================================================================
// (public)
bool convert(bool& dest, const std::string& src, Conversion* properties)
{
    if (properties == nullptr) { properties = Conversion::globalDefault2(); }
    if (src.compare(properties->boolTrue()) == 0) {
        dest = true;
        return true;
    } else if (src.compare(properties->boolFalse()) == 0) {
        dest = false;
        return true;
    }
    return false;
}

// =============================================================================
// (public)
bool convert(std::string& dest, bool src, Conversion* properties)
{
    if (properties == nullptr) { properties = Conversion::globalDefault2(); }
    if (src) {
        dest = properties->boolTrue();
    } else {
        dest = properties->boolFalse();
    }
    return true;
}

// =============================================================================
// (public)
bool convert(int& dest, double src, Conversion* properties)
{
    if (properties == nullptr) { properties = Conversion::globalDefault2(); }
    if (properties->roundRules() == Conversion::DoubleToInt_Round) {
        dest = static_cast<int>(std::round(src));
    } else if (properties->roundRules() == Conversion::DoubleToInt_Floor) {
        dest = static_cast<int>(std::floor(src));
    } else if (properties->roundRules() == Conversion::DoubleToInt_Ceil) {
        dest = static_cast<int>(std::ceil(src));
    } else if (properties->roundRules() == Conversion::DoubleToInt_Trunc) {
        dest = static_cast<int>(std::trunc(src));
    }
    return true;
}

// =============================================================================
// (public)
bool convert(double& dest, int src, Conversion* )
{
    dest = src;
    return true;
}

// =============================================================================
// (public)
bool convert(int& dest, const std::string& src, Conversion* )
{
    try {
        dest = std::stoi(src);
    } catch(...) {
        return false;
    }
    return true;
}

// =============================================================================
// (public)
bool convert(std::string& dest, int src, Conversion* )
{
    dest = std::to_string(src);
    return true;
}

// =============================================================================
// (public)
bool convert(double& dest, const std::string& src, Conversion* )
{
    try {
        dest = std::stod(src);
    } catch(...) {
        return false;
    }
    return true;
}

// =============================================================================
// (public)
bool convert(std::string& dest, double src, Conversion* properties)
{
    if (properties == nullptr) { properties = Conversion::globalDefault2(); }
    std::stringstream stream;
    switch (properties->doubleToStringMode()) {
    case Conversion::DoubleToString_Default:
        stream << std::defaultfloat;
        break;
    case Conversion::DoubleToString_Scientific:
        stream << std::scientific;
        break;
    case Conversion::DoubleToString_Fixed:
        stream << std::fixed;
        break;
    default:
        ASSERT(false);
        break;
    }
    stream << std::setprecision(properties->doubleToStringPrecision()) << src;
    dest = stream.str();
    return true;
}

// =============================================================================
// (public)
bool convert(std::string &dest, const char *source, Conversion* )
{
    if (source == nullptr) { return false; }
    dest.assign(source);
    return true;
}

// =============================================================================
// (public)
bool convert(std::string &dest, const DateTime &src, Conversion *properties)
{
    UNUSED(properties);
    if (src.isNull()) { return false; }
    dest = src.toString(DateTime::TimeZone::UTC, nullptr);
    return true;
}

// =============================================================================
// (public)
bool convert(DateTime &dest, const std::string &src, Conversion *properties)
{
    UNUSED(properties);
    DateTime dt(src, DateTime::TimeZone::UTC);
    if (dt.isNull()) { return false; }
    dest = dt;
    return true;
}

// =============================================================================
// canConvert()
// =============================================================================

// =============================================================================
// (public)
bool canConvert(bool&, int, Conversion*)
{
    return true;
}

// =============================================================================
// (public)
bool canConvert(int&, const bool&, Conversion*)
{
    return true;
}

// =============================================================================
// (public)
bool canConvert(bool &, double, Conversion*)
{
    return false;
}

// =============================================================================
// (public)
bool canConvert(double&, bool, Conversion*)
{
    return false;
}

// =============================================================================
// (public)
bool canConvert(std::string&, bool, Conversion*)
{
    return true;
}

// =============================================================================
// (public)
bool canConvert(int&, double, Conversion*)
{
    return true;
}

// =============================================================================
// (public)
bool canConvert(double&, int, Conversion*)
{
    return true;
}

// =============================================================================
// (public)
bool canConvert(std::string&, const int&, Conversion*)
{
    return true;
}

// =============================================================================
// (public)
bool canConvert(std::string&, const double&, Conversion*)
{
    return true;
}

// =============================================================================
// (public)
bool canConvert(std::string &, const char *source, Conversion*)
{
    return source != nullptr;
}

// =============================================================================
// (public)
bool canConvert(std::string &, const DateTime &src, Conversion *)
{
    return !src.isNull();
}

// =============================================================================
// (public)
bool canConvert(DateTime &, const std::string &src, Conversion *)
{
    return !DateTime(src, DateTime::TimeZone::UTC).isNull();
}

} // namespace Oak::Model

