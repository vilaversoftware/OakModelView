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


namespace Oak::Model {

// =============================================================================
// Class definition
// =============================================================================
class Color
{
public:
    Color();
    Color(int red, int green, int blue, int alpha = 255);
    Color(const Color& copy);

    Color& operator=(const Color &copy);

    bool operator==(const Color &c) const;

    bool isValid() const;

    int red() const;
    int green() const;
    int blue() const;
    int alpha() const;

    bool setRed(int value);
    bool setGreen(int value);
    bool setBlue(int value);
    bool setAlpha(int value);

    bool set(int red, int green, int blue, int alpha = 255);

protected:
    int m_red;
    int m_green;
    int m_blue;
    int m_alpha;
};

} // namespace Oak::Model

