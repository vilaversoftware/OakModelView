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

#pragma once

#include "NodeQuery.h"


namespace Oak::Model {

// =============================================================================
// Class definition
// =============================================================================
class NodeQueryParent : public NodeQuery
{
public:
    NodeQueryParent();
    NodeQueryParent(const NodeQueryParent& copy);
    NodeQueryParent(NodeQueryParent&& move);

protected:
    virtual Node first(const Node &refNode) const override;
    virtual Node last(const Node &refNode) const override;

    virtual const NodeDef *_nodeDef(const NodeDef *nDef) const override;
};

} // namespace Oak::Model

