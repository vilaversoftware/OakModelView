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

#include "VariantRef.h"

namespace Oak {
namespace Model {

// =============================================================================
// Class definition
// =============================================================================
class InputOptions
{
public:
    InputOptions();

    void getOptions(VariantRef optList) const;

protected:
    Variant m_includedOptions;
    Variant m_excludedOptions;
};

} // namespace Model
} // namespace Oak