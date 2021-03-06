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

#include <list>
#include <string>
#include <vector>
#include <memory>
#include <limits>

#include <NodeData.h>


namespace Oak::Model {

// =============================================================================
// Definitions
// =============================================================================


// =============================================================================
// Enums
// =============================================================================

enum BoolState {
    BoolState_Undefined = -1,
    BoolState_False = 0,
    BoolState_True = 1
};

// =============================================================================
// Conversion service functions
// =============================================================================

std::vector<std::string> split(const std::string &str, char seperator = ';', bool ignoreEmpty = false);
std::vector<std::vector<std::string>> doubleSplit(const std::string &str, char outerSeperator = ';', char innerSeperator = ':', bool ignoreEmpty = false);

std::vector<std::string> splitString(const std::string &str, char seperator, bool skipEmptyParts = false);
std::string joinString(const std::vector<std::string> strList, char seperator);

std::string simplified(const std::string &str);

std::string fromDouble(double value, char f='g', int prec=6);

bool compare(std::string s1, std::string s2, bool caseSensitive = true);
bool contains(const std::vector<std::string> &strList, const std::string &str);
int count(const std::vector<std::string> &strList, const std::string &str);

// trim from start (in place)
void ltrim(std::string &s);

// trim from end (in place)
void rtrim(std::string &s);

// trim from both ends (in place)
void trim(std::string &s);

// trim from start (copying)
std::string ltrim_copy(std::string s);

// trim from end (copying)
std::string rtrim_copy(std::string s);

// trim from both ends (copying)
std::string trim_copy(std::string s);

/*************** Convertion of values ***************/
// string
std::string toString(int value);
std::string toString(const std::vector<int> &values, char seperator);
std::string toString(double value);
std::string toString(bool value, const std::vector<std::string> &nodeList);
std::string toString(BoolState value, const std::vector<std::string> &nodeList);

std::vector<std::string> toStringList(const std::vector<int> &values);
std::vector<std::string> toStringList(const std::vector<std::vector<int>> &values, char seperator);
std::vector<std::string> toStringList(const std::vector<double> &values);
std::vector<std::string> toStringList(const std::vector<bool> &values);
std::vector<std::string> toStringList(const std::vector<BoolState> &values, const std::vector<std::string> &nodeList);

// integer
int toInteger(const std::string &str, bool *ok = nullptr);
std::vector<int> toIntegerList(const std::vector<std::string> &values, bool *ok = nullptr);
std::vector<int> toIntegerList(const std::string &value, char seperator, bool *ok = nullptr);
std::vector<std::vector<int>> toIntegerListOfList(const std::vector<std::string> &values, char seperator, bool *ok = nullptr);

// double
float toFloat(const std::string &str, bool *ok = nullptr);
double toDouble(const std::string &str, bool *ok = nullptr);
std::vector<double> toDoubleList(const std::vector<std::string> &values);

// bool
bool toBool(BoolState value);
bool toBool(const std::string &value, const std::vector<std::string> &nodeList);

// enum BoolState
BoolState toBoolState(bool value);
BoolState toBoolState(const std::string &value, const std::vector<std::string> &nodeList);

std::vector<BoolState> toBoolStateList(const std::vector<bool> &values);
std::vector<BoolState> toBoolStateList(const std::vector<std::string> &values, const std::vector<std::string> &nodeList);

// =============================================================================
// Class definition
// =============================================================================

template<typename TO, typename FROM>
std::unique_ptr<TO> dynamic_unique_pointer_cast(std::unique_ptr<FROM>&& old)
{
    TO* ptr = dynamic_cast<TO*>(old.get());
    if (ptr) {
        old.release();
        return std::unique_ptr<TO>(ptr);
    }
    ASSERT(false);
    return std::unique_ptr<TO>(nullptr);
}

// =============================================================================
// (public)
template<typename T>
void removeDuplicates(std::list<T> &list)
{
    if (list.empty()) { return; }
    auto itCompare = list.begin();
    auto itCurrent = list.begin();
    while (itCompare != list.end()) {
        itCurrent++;
        while (itCurrent != list.end()) {
            if (*itCompare == *itCurrent) {
                itCurrent = list.erase(itCurrent);
            } else {
                itCurrent++;
            }
        }
        if (itCompare == list.end()) {
            return;
        }
        itCompare++;
        itCurrent = itCompare;
    }
}

// =============================================================================
// (public)
template<typename T>
void removeDuplicates(std::vector<T> &vector)
{
    if (vector.empty()) { return; }
    auto itCompare = vector.begin();
    auto itCurrent = vector.begin();
    while (itCurrent != vector.end()) {
        itCurrent++;
        while (itCurrent != vector.end()) {
            if (*itCompare == *itCurrent) {
                itCurrent = vector.erase(itCurrent);
            } else {
                itCurrent++;
            }
        }
        if (itCompare == vector.end()) {
            return;
        }
        itCompare++;
        itCurrent = itCompare;
    }
}

int indexOf(const std::string &str, const char &value, int firstIndex = 0);

} // namespace Oak::Model

