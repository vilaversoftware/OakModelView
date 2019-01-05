/**
 * oakmodelview - version 0.1.0
 * --------------------------------------------------------
 * Copyright (C) 2017, by Mikkel Nøhr Løvgreen (mikkel@oakmodelview.com)
 * Report bugs and download new versions at http://oakmodelview.com/
 *
 * This library is distributed under the MIT License.
 * See accompanying file LICENSE in the root folder.
 */

#include "QuickOakModelBuilder.h"

#include <string>

#include "NodeDef.h"

#include "../ServiceFunctions/Trace.h"

// =============================================================================
// (public)
QuickOakModelBuilder::QuickOakModelBuilder(QObject* parent)
    : QObject(parent)
{
    m_builderOwner = std::make_unique<Oak::Model::ModelBuilderData>();
    m_builder = m_builderOwner.get();
}

// =============================================================================
// (public)
bool QuickOakModelBuilder::validate()
{
    return m_builder->validateModel();
}

// =============================================================================
// (public)
Oak::Model::NodeDefSPtr QuickOakModelBuilder::create()
{
    return m_builder->createModel();
}