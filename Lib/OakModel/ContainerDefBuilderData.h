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

#include <memory>
#include <string>
#include <vector>

namespace Oak::Model {

class NodeDef;
typedef std::shared_ptr<NodeDef> NodeDefSPtr;

class NodeDefBuilder;
typedef std::shared_ptr<NodeDefBuilder> NodeDefBuilderSPtr;

// =============================================================================
// Class definition
// =============================================================================
class ContainerDefBuilderData
{
public:
    ContainerDefBuilderData();
    virtual ~ContainerDefBuilderData() {}

    void validate(std::vector<std::string> &errorMessages) const;

    bool set(NodeDefBuilderSPtr builder, std::vector<NodeDefSPtr> nodeDefList) const;

    int minCount = 0;
    int maxCount = std::numeric_limits<int>::max();
    std::string name;
    std::string variantId;
};

typedef std::unique_ptr<ContainerDefBuilderData> ContainerDefBuilderDataUPtr;

} // namespace Oak::Model
