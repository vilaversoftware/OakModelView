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

#include <boost/test/included/unit_test.hpp>
using boost::unit_test_framework::test_suite;

#ifdef XML_BACKEND

#include "XMLDocument.h"
using namespace Oak;

void test_OpenXMLDoc()
{
    XML::Document document1;
    BOOST_CHECK(document1.isNull());

    BOOST_REQUIRE(document1.load(std::string(RESOURCE_PATH)+"test_doc.xml"));

    BOOST_CHECK(!document1.isNull());

    XML::Document document2 = document1;

    BOOST_CHECK(document1 == document2);

    document2 = XML::Document();

    document2.clone(document1);

    BOOST_CHECK(document1 != document2);
}


test_suite* Test_XMLDoc()
{
    test_suite* test = BOOST_TEST_SUITE( "XMLDoc" );

    test->add(BOOST_TEST_CASE(&test_OpenXMLDoc));

    return test;
}
#endif XML_BACKEND
