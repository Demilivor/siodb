// Copyright (C) 2019-2020 Siodb GmbH. All rights reserved.
// Use of this source code is governed by a license that can be found
// in the LICENSE file.

#include "User.h"

// Project headers
#include <siodb-generated/iomgr/lib/messages/IOManagerMessageId.h>
#include "ThrowDatabaseError.h"
#include "UserAccessKey.h"

// Common project headers
#include <siodb/common/config/SiodbDefs.h>
#include <siodb/common/crypto/DigitalSignatureKey.h>
#include <siodb/iomgr/shared/dbengine/DatabaseObjectName.h>

namespace siodb::iomgr::dbengine {

User::User(UserIdGenerator& userIdGenerator, std::string&& name,
        std::optional<std::string>&& realName, std::optional<std::string>&& description,
        bool active)
    : m_name(validateUserName(std::move(name)))
    , m_realName(std::move(realName))
    , m_description(std::move(description))
    , m_active(active)
    , m_id(userIdGenerator.generateNextUserId())
{
}

User::User(const UserRecord& userRecord)
    : m_name(validateUserName(std::string(userRecord.m_name)))
    , m_realName(userRecord.m_realName)
    , m_description(userRecord.m_description)
    , m_active(userRecord.m_active)
    , m_id(userRecord.m_id)
{
    for (const auto& accessKeyRecord : userRecord.m_accessKeys.byId())
        m_accessKeys.push_back(std::make_shared<UserAccessKey>(*this, accessKeyRecord));
}

UserAccessKeyPtr User::findUserAccessKeyChecked(const std::string& name) const
{
    if (auto userAccessKey = findUserAccessKeyUnlocked(name)) return userAccessKey;
    throwDatabaseError(IOManagerMessageId::kErrorUserAccessKeyDoesNotExist, m_name, name);
}

UserAccessKeyPtr User::addUserAccessKey(std::uint64_t id, std::string&& name, std::string&& text,
        std::optional<std::string>&& description, bool active)
{
    auto it = std::find_if(
            m_accessKeys.begin(), m_accessKeys.end(), [id, &name](const auto& accessKey) noexcept {
                return accessKey->getId() == id || accessKey->getName() == name;
            });
    if (it != m_accessKeys.end())
        throwDatabaseError(IOManagerMessageId::kErrorUserAccessKeyAlreadyExists, m_name, name);

    if (text.size() > siodb::kMaxAccessKeySize) {
        throwDatabaseError(IOManagerMessageId::kErrorUserAccessKeyIsLargerThanMax, m_name, name,
                text.size(), siodb::kMaxAccessKeySize);
    }

    try {
        siodb::crypto::DigitalSignatureKey key;
        key.parseFromString(text);
    } catch (std::exception& e) {
        throwDatabaseError(IOManagerMessageId::kErrorUserAccessKeyIsInvalid, m_name, name);
    }

    auto accessKey = std::make_shared<UserAccessKey>(
            *this, id, std::move(name), std::move(text), std::move(description), active);
    m_accessKeys.push_back(accessKey);
    return accessKey;
}

void User::deleteUserAccessKey(const std::string& name)
{
    auto it = std::find_if(m_accessKeys.begin(), m_accessKeys.end(),
            [&name](const auto& accessKey) noexcept { return accessKey->getName() == name; });
    if (it == m_accessKeys.end())
        throwDatabaseError(IOManagerMessageId::kErrorUserAccessKeyDoesNotExist, m_name, name);

    if (isSuperUser() && getActiveKeyCount() == 1 && (*it)->isActive())
        throwDatabaseError(IOManagerMessageId::kErrorCannotDeleteLastSuperUserAccessKey, name);

    m_accessKeys.erase(it);
}

std::size_t User::getActiveKeyCount() const noexcept
{
    return std::count_if(m_accessKeys.begin(), m_accessKeys.end(),
            [](const auto& key) noexcept { return key->isActive(); });
}

// ----- internals -----

std::string&& User::validateUserName(std::string&& userName)
{
    if (isValidDatabaseObjectName(userName)) return std::move(userName);
    throwDatabaseError(IOManagerMessageId::kErrorInvalidUserName, userName);
}

UserAccessKeyPtr User::findUserAccessKeyUnlocked(const std::string& name) const noexcept
{
    auto it = std::find_if(m_accessKeys.begin(), m_accessKeys.end(),
            [&name](const auto& accessKey) noexcept { return accessKey->getName() == name; });
    return (it == m_accessKeys.end()) ? nullptr : *it;
}

}  // namespace siodb::iomgr::dbengine
