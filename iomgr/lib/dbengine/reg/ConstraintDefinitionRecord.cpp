// Copyright (C) 2019-2020 Siodb GmbH. All rights reserved.
// Use of this source code is governed by a license that can be found
// in the LICENSE file.

#include "ConstraintDefinitionRecord.h"

// Project headers
#include "Helpers.h"
#include "../ConstraintDefinition.h"

// Common project headers
#include <siodb/common/utils/PlainBinaryEncoding.h>

// Boost headers
#include <boost/lexical_cast.hpp>

// xxHash library
#include "xxhash.h"

namespace siodb::iomgr::dbengine {

const Uuid ConstraintDefinitionRecord::s_classUuid =
        boost::lexical_cast<Uuid>("2077e02b-168a-48ab-9a66-80f0effc4031");

ConstraintDefinitionRecord::ConstraintDefinitionRecord(
        const ConstraintDefinition& constraintDefinition)
    : m_id(constraintDefinition.getId())
    , m_type(constraintDefinition.getType())
    , m_expression(constraintDefinition.serializeExpression())
    , m_hash(constraintDefinition.getHash())
{
}

std::uint64_t ConstraintDefinitionRecord::computeHash(
        ConstraintType constraintType, const BinaryValue& binaryValue) noexcept
{
    std::uint8_t buffer[8];
    ::pbeEncodeUInt32(static_cast<std::uint32_t>(constraintType), buffer);
    ::pbeEncodeUInt32(binaryValue.size(), buffer + 4);
    const auto h = XXH64(buffer, sizeof(buffer), kHashSeed);
    return binaryValue.empty() ? h : XXH64(binaryValue.data(), binaryValue.size(), h);
}

std::size_t ConstraintDefinitionRecord::getSerializedSize(unsigned version) const noexcept
{
    return Uuid::static_size() + ::getVarIntSize(version) + ::getVarIntSize(m_id)
           + ::getVarIntSize(static_cast<std::uint32_t>(m_type))
           + ::getSerializedSize(m_expression);
}

std::uint8_t* ConstraintDefinitionRecord::serializeUnchecked(
        std::uint8_t* buffer, unsigned version) const noexcept
{
    std::memcpy(buffer, s_classUuid.data, Uuid::static_size());
    buffer += Uuid::static_size();
    buffer = ::encodeVarInt(version, buffer);
    buffer = ::encodeVarInt(m_id, buffer);
    buffer = ::encodeVarInt(static_cast<std::uint32_t>(m_type), buffer);
    buffer = ::serializeUnchecked(m_expression, buffer);
    return buffer;
}

std::size_t ConstraintDefinitionRecord::deserialize(const std::uint8_t* buffer, std::size_t length)
{
    if (length < Uuid::static_size())
        helpers::reportInvalidOrNotEnoughData(kClassName, "$classUuid", 0);
    if (std::memcmp(s_classUuid.data, buffer, Uuid::static_size()) != 0)
        helpers::reportClassUuidMismatch(kClassName, buffer, s_classUuid.data);

    std::size_t totalConsumed = Uuid::static_size();

    std::uint32_t classVersion = 0;
    int consumed = ::decodeVarInt(buffer + totalConsumed, length - totalConsumed, classVersion);
    if (consumed < 1) helpers::reportInvalidOrNotEnoughData(kClassName, "$classVersion", consumed);
    totalConsumed += consumed;

    if (classVersion > kClassVersion)
        helpers::reportClassVersionMismatch(kClassName, classVersion, kClassVersion);

    consumed = ::decodeVarInt(buffer + totalConsumed, length - totalConsumed, m_id);
    if (consumed < 1) helpers::reportInvalidOrNotEnoughData(kClassName, "id", consumed);
    totalConsumed += consumed;

    std::uint32_t type = 0;
    consumed = ::decodeVarInt(buffer + totalConsumed, length - totalConsumed, type);
    if (consumed < 1) helpers::reportInvalidOrNotEnoughData(kClassName, "type", consumed);
    m_type = static_cast<ConstraintType>(type);
    totalConsumed += consumed;

    try {
        totalConsumed +=
                deserializeObject(buffer + totalConsumed, length - totalConsumed, m_expression);
    } catch (std::exception& ex) {
        helpers::reportDeserializationFailure(kClassName, "expression", ex.what());
    }

    return totalConsumed;
}

}  // namespace siodb::iomgr::dbengine
