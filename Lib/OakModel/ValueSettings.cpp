/**
 * oakmodelview - version 0.1.0
 * --------------------------------------------------------
 * Copyright (C) 2017, by Mikkel Nøhr Løvgreen (mikkel@oakmodelview.com)
 * Report bugs and download new versions at http://oakmodelview.com/
 *
 * This library is distributed under the MIT License.
 * See accompanying file LICENSE in the root folder.
 */

#include "../ServiceFunctions/Assert.h"

#include "ValueSettings.h"
#include "UnionRef.h"


namespace Oak::Model {

// =============================================================================
// (public)
ValueSettings::ValueSettings()
{
    m_settingsMap[OPTION_ONLY] = false;
    m_settingsMap[UNIT] = "";
    m_settingsMap[UNIQUE] = -1;
    m_settingsMap[REQUIRED] = -1;
}

// =============================================================================
// (public)
ValueSettings& ValueSettings::operator=(const ValueSettings& copy)
{
    m_settingsMap = copy.m_settingsMap;
    return *this;
}

// =============================================================================
// (public)
const UnionValue &ValueSettings::value(const std::string &settingName, const UnionValue &defaultValue) const
{
    auto it = m_settingsMap.find(settingName);
    if (it != m_settingsMap.end()) {
        return it->second;
    }
    ASSERT(!defaultValue.isNull());
    return defaultValue;
}

// =============================================================================
// (public)
void ValueSettings::setValue(const std::string &settingName, const UnionRef &value)
{
    m_settingsMap[settingName] = value;
}

} // namespace Oak::Model

