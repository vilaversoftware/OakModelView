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

#ifdef XML_BACKEND

// This class is ment to be an definition to a xml parser
// It is created in the hopes that the TDMLib will be independent of a specific xml parser

#include <map>
#include <string>
#include <vector>
typedef std::basic_ostream<char, std::char_traits<char> > stdTextStream;

#include "pugixml/pugixml.hpp"

#ifndef UNUSED
#define UNUSED(x) (void)x;
#endif

namespace Oak::XML {

typedef std::vector<int>::size_type vSize;

// =============================================================================
// Class definition
// =============================================================================
class Element
{
public:
    Element();
    Element(pugi::xml_node element);

    bool operator==(const Element &element) const;
    bool operator!=(const Element &element) const { return !(*this == element); }

    std::string tagName() const;
    int compareTagName(const std::string &tName) const;

    bool isNull() const;
    bool empty() const;

    pugi::xml_node internalObject() const;

    // Create
    bool cloneElement(const Element &element);
    Element prependChild(const std::string &tagName);
    Element appendChild(const std::string &tagName);
    Element insertBefore(const std::string &tagName, const Element &refChild);
    Element insertAfter(const std::string &tagName, const Element &refChild);
    Element moveBefore(const Element &target, const Element &refChild);
    Element moveAfter(const Element &target, const Element &refChild);
    bool removeChild(const Element &child);
    void clear();
    void save(std::string &str, int indent) const;

    // Element navigation
    Element firstChild(const std::string &tagName = std::string()) const;
    Element lastChild(const std::string &tagName = std::string()) const;

    Element nextSibling(const std::string &tagName = std::string()) const;
    Element previousSibling(const std::string &tagName = std::string()) const;

    Element parentElement() const;

    // Attribute
    bool hasAttribute(const std::string &name) const;
    std::string attribute(const std::string &name) const;
    bool getAttribute(const std::string &name, std::string &value) const;
    std::map<std::string,std::string> attributeMap()const;
    bool setAttribute(const std::string &name, const std::string &value);
    int compareAttribute(const std::string& name, const std::string& value);
    bool removeAttribute(const std::string &name);

    // Element text
    bool hasText() const;
    std::string text() const;
    bool getText(std::string &str) const;
    bool setText(const std::string &text);
    int compareText(const std::string& value);
    bool removeText();

//    Tag names are case-sensitive
//    Tag names must start with a letter or underscore
//    Tag names cannot start with the letters xml (or XML, or Xml, etc)
//    Tag names can contain letters, digits, hyphens(-), underscores(_), and periods(.)
//    Tag names cannot contain spaces
    static bool validateTagName(const std::string& tagName);
    static bool validateAttributeName(const std::string& attributeName);
    static bool validateTagText(const std::string& text);
    static bool validateAttributeText(const std::string& text);

private:
    pugi::xml_node m_element;

//    friend class ParserDocument;
};

} // namespace Oak::XML

#endif // XML_BACKEND
