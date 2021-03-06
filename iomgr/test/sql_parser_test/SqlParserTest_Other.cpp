// Copyright (C) 2019-2020 Siodb GmbH. All rights reserved.
// Use of this source code is governed by a license that can be found
// in the LICENSE file.

// Project headers
#include "TestContext.h"
#include "dbengine/parser/DBEngineRequestFactory.h"
#include "dbengine/parser/SqlParser.h"
#include "dbengine/parser/expr/AllExpressions.h"

// Google Test
#include <gtest/gtest.h>

namespace dbengine = siodb::iomgr::dbengine;
namespace parser_ns = dbengine::parser;

TEST(SqlParser_Query, UseDatabase)
{
    // Parse statement and prepare request
    const std::string statement("USE DATABASE my_database");
    parser_ns::SqlParser parser(statement);
    parser.parse();
    const auto dbeRequest =
            parser_ns::DBEngineRequestFactory::createRequest(parser.findStatement(0));

    // Check request type
    ASSERT_EQ(dbeRequest->m_requestType, requests::DBEngineRequestType::kUseDatabase);

    // Check request
    const auto& request = dynamic_cast<const requests::UseDatabaseRequest&>(*dbeRequest);
    ASSERT_EQ(request.m_database, "MY_DATABASE");
}

TEST(SqlParser_Query, ShowDatabases)
{
    // Parse statement and prepare request
    const std::string statement("SHOW DATABASES");
    parser_ns::SqlParser parser(statement);
    parser.parse();
    const auto dbeRequest =
            parser_ns::DBEngineRequestFactory::createRequest(parser.findStatement(0));

    // Check request type
    ASSERT_EQ(dbeRequest->m_requestType, requests::DBEngineRequestType::kShowDatabases);
}
