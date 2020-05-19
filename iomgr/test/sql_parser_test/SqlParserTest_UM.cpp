// Copyright (C) 2019-2020 Siodb GmbH. All rights reserved.
// Use of this source code is governed by a license that can be found
// in the LICENSE file.

// Project headers
#include "dbengine/parser/DBEngineRequestFactory.h"
#include "dbengine/parser/SqlParser.h"

// STL headers
#include <iostream>

// Google Test
#include <gtest/gtest.h>

TEST(UM, CreateUser)
{
    // Parse statement
    const std::string statement = "CREATE USER user_name";
    siodb::iomgr::dbengine::parser::SqlParser parser(statement);
    parser.parse();

    const auto dbeRequest = siodb::iomgr::dbengine::parser::DBEngineRequestFactory::createRequest(
            parser.findStatement(0));

    ASSERT_EQ(dbeRequest->m_requestType,
            siodb::iomgr::dbengine::requests::DBEngineRequestType::kCreateUser);

    const auto& createUserRequest =
            dynamic_cast<const siodb::iomgr::dbengine::requests::CreateUserRequest&>(*dbeRequest);

    EXPECT_EQ(createUserRequest.m_name, "USER_NAME");
    EXPECT_EQ(createUserRequest.m_realName, "");
    EXPECT_EQ(createUserRequest.m_active, true);
}

TEST(UM, CreateActiveUser)
{
    // Parse statement
    const std::string statement = "CREATE USER user_name WITH STATE=ACTIVE";
    siodb::iomgr::dbengine::parser::SqlParser parser(statement);
    parser.parse();

    const auto dbeRequest = siodb::iomgr::dbengine::parser::DBEngineRequestFactory::createRequest(
            parser.findStatement(0));

    ASSERT_EQ(dbeRequest->m_requestType,
            siodb::iomgr::dbengine::requests::DBEngineRequestType::kCreateUser);

    const auto& createUserRequest =
            dynamic_cast<const siodb::iomgr::dbengine::requests::CreateUserRequest&>(*dbeRequest);

    EXPECT_EQ(createUserRequest.m_name, "USER_NAME");
    EXPECT_EQ(createUserRequest.m_realName, "");
    EXPECT_EQ(createUserRequest.m_active, true);
}

TEST(UM, CreateInactiveUser)
{
    // Parse statement
    const std::string statement = "CREATE USER user_name WITH STATE=INACTIVE";
    siodb::iomgr::dbengine::parser::SqlParser parser(statement);
    parser.parse();

    const auto dbeRequest = siodb::iomgr::dbengine::parser::DBEngineRequestFactory::createRequest(
            parser.findStatement(0));

    ASSERT_EQ(dbeRequest->m_requestType,
            siodb::iomgr::dbengine::requests::DBEngineRequestType::kCreateUser);

    const auto& createUserRequest =
            dynamic_cast<const siodb::iomgr::dbengine::requests::CreateUserRequest&>(*dbeRequest);

    EXPECT_EQ(createUserRequest.m_name, "USER_NAME");
    EXPECT_EQ(createUserRequest.m_realName, "");
    EXPECT_EQ(createUserRequest.m_active, false);
}

TEST(UM, CreateUserWithRealName)
{
    // Parse statement
    const std::string statement = "CREATE USER user_name WITH REAL_NAME='real name'";
    siodb::iomgr::dbengine::parser::SqlParser parser(statement);
    parser.parse();

    const auto dbeRequest = siodb::iomgr::dbengine::parser::DBEngineRequestFactory::createRequest(
            parser.findStatement(0));

    ASSERT_EQ(dbeRequest->m_requestType,
            siodb::iomgr::dbengine::requests::DBEngineRequestType::kCreateUser);

    const auto& createUserRequest =
            dynamic_cast<const siodb::iomgr::dbengine::requests::CreateUserRequest&>(*dbeRequest);

    EXPECT_EQ(createUserRequest.m_name, "USER_NAME");
    EXPECT_EQ(createUserRequest.m_realName, "real name");
    EXPECT_EQ(createUserRequest.m_active, true);
}

TEST(UM, DropUser)
{
    // Parse statement
    const std::string statement = "DROP USER user_name";
    siodb::iomgr::dbengine::parser::SqlParser parser(statement);
    parser.parse();

    const auto dbeRequest = siodb::iomgr::dbengine::parser::DBEngineRequestFactory::createRequest(
            parser.findStatement(0));

    ASSERT_EQ(dbeRequest->m_requestType,
            siodb::iomgr::dbengine::requests::DBEngineRequestType::kDropUser);

    const auto& dropUserRequest =
            dynamic_cast<const siodb::iomgr::dbengine::requests::DropUserRequest&>(*dbeRequest);

    ASSERT_EQ(dropUserRequest.m_name, "USER_NAME");
}

TEST(UM, AlterUserSetRealName)
{
    // Parse statement
    const std::string statement = "ALTER USER user_name SET REAL_NAME = 'new real name'";
    siodb::iomgr::dbengine::parser::SqlParser parser(statement);
    parser.parse();

    const auto dbeRequest = siodb::iomgr::dbengine::parser::DBEngineRequestFactory::createRequest(
            parser.findStatement(0));

    ASSERT_EQ(dbeRequest->m_requestType,
            siodb::iomgr::dbengine::requests::DBEngineRequestType::kAlterUser);

    const auto& setUserRealNameRequest =
            dynamic_cast<const siodb::iomgr::dbengine::requests::AlterUserRequest&>(*dbeRequest);

    EXPECT_EQ(setUserRealNameRequest.m_name, "USER_NAME");
    ASSERT_TRUE(setUserRealNameRequest.m_realName.has_value());
    EXPECT_EQ(setUserRealNameRequest.m_realName, "new real name");
    EXPECT_FALSE(setUserRealNameRequest.m_active.has_value());
}

TEST(UM, AlterUserSetState)
{
    // Parse statement
    const std::string statement = "ALTER USER user_name SET STATE = ACTIVE";
    siodb::iomgr::dbengine::parser::SqlParser parser(statement);
    parser.parse();

    const auto dbeRequest = siodb::iomgr::dbengine::parser::DBEngineRequestFactory::createRequest(
            parser.findStatement(0));

    ASSERT_EQ(dbeRequest->m_requestType,
            siodb::iomgr::dbengine::requests::DBEngineRequestType::kAlterUser);

    const auto& setUserStateRequest =
            dynamic_cast<const siodb::iomgr::dbengine::requests::AlterUserRequest&>(*dbeRequest);

    EXPECT_EQ(setUserStateRequest.m_name, "USER_NAME");
    ASSERT_TRUE(setUserStateRequest.m_active.has_value());
    EXPECT_TRUE(setUserStateRequest.m_active.value());
    EXPECT_FALSE(setUserStateRequest.m_realName.has_value());
}

TEST(UM, AlterUserSetStateAndRealName)
{
    // Parse statement
    const std::string statement =
            "ALTER USER user_name SET STATE = INACTIVE, REAL_NAME = 'newRealName'";
    siodb::iomgr::dbengine::parser::SqlParser parser(statement);
    parser.parse();

    const auto dbeRequest = siodb::iomgr::dbengine::parser::DBEngineRequestFactory::createRequest(
            parser.findStatement(0));

    ASSERT_EQ(dbeRequest->m_requestType,
            siodb::iomgr::dbengine::requests::DBEngineRequestType::kAlterUser);

    const auto& alterUserRequest =
            dynamic_cast<const siodb::iomgr::dbengine::requests::AlterUserRequest&>(*dbeRequest);

    EXPECT_EQ(alterUserRequest.m_name, "USER_NAME");
    ASSERT_TRUE(alterUserRequest.m_realName.has_value());
    ASSERT_TRUE(alterUserRequest.m_active.has_value());
    EXPECT_TRUE(!alterUserRequest.m_active.value());
    EXPECT_EQ(alterUserRequest.m_realName, "newRealName");
}

TEST(UM, AlterUserAddAccessKey)
{
    // Parse statement
    const std::string statement =
            "ALTER USER user_name ADD ACCESS KEY keyName 'KeyText' STATE = INACTIVE";
    siodb::iomgr::dbengine::parser::SqlParser parser(statement);
    parser.parse();

    const auto dbeRequest = siodb::iomgr::dbengine::parser::DBEngineRequestFactory::createRequest(
            parser.findStatement(0));

    ASSERT_EQ(dbeRequest->m_requestType,
            siodb::iomgr::dbengine::requests::DBEngineRequestType::kAddUserAccessKey);

    const auto& addUserAccessKeyRequest =
            dynamic_cast<const siodb::iomgr::dbengine::requests::AddUserAccessKeyRequest&>(
                    *dbeRequest);

    EXPECT_EQ(addUserAccessKeyRequest.m_userName, "USER_NAME");
    EXPECT_EQ(addUserAccessKeyRequest.m_keyName, "KEYNAME");
    EXPECT_EQ(addUserAccessKeyRequest.m_keyText, "KeyText");
    EXPECT_FALSE(addUserAccessKeyRequest.m_active);
}

TEST(UM, AlterUserDropAccessKey)
{
    // Parse statement
    const std::string statement = "ALTER USER user_name DROP ACCESS KEY keyName";
    siodb::iomgr::dbengine::parser::SqlParser parser(statement);
    parser.parse();

    const auto dbeRequest = siodb::iomgr::dbengine::parser::DBEngineRequestFactory::createRequest(
            parser.findStatement(0));

    ASSERT_EQ(dbeRequest->m_requestType,
            siodb::iomgr::dbengine::requests::DBEngineRequestType::kDropUserAccessKey);

    const auto& dropUserAccessKeyRequest =
            dynamic_cast<const siodb::iomgr::dbengine::requests::DropUserAccessKeyRequest&>(
                    *dbeRequest);

    EXPECT_EQ(dropUserAccessKeyRequest.m_userName, "USER_NAME");
    EXPECT_EQ(dropUserAccessKeyRequest.m_keyName, "KEYNAME");
}

TEST(UM, AlterUserAlterAccessKey)
{
    // Parse statement
    const std::string statement =
            "ALTER USER user_name ALTER ACCESS KEY keyName SET STATE = INACTIVE";
    siodb::iomgr::dbengine::parser::SqlParser parser(statement);
    parser.parse();

    const auto dbeRequest = siodb::iomgr::dbengine::parser::DBEngineRequestFactory::createRequest(
            parser.findStatement(0));

    ASSERT_EQ(dbeRequest->m_requestType,
            siodb::iomgr::dbengine::requests::DBEngineRequestType::kAlterUserAccessKey);

    const auto& alterUserAccessKeyRequest =
            dynamic_cast<const siodb::iomgr::dbengine::requests::AlterUserAccessKey&>(*dbeRequest);

    EXPECT_EQ(alterUserAccessKeyRequest.m_userName, "USER_NAME");
    EXPECT_EQ(alterUserAccessKeyRequest.m_keyName, "KEYNAME");
    ASSERT_TRUE(alterUserAccessKeyRequest.m_active);
    EXPECT_FALSE(*alterUserAccessKeyRequest.m_active);
}
