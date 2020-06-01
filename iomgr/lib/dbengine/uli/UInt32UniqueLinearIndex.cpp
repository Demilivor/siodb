// Copyright (C) 2019-2020 Siodb GmbH. All rights reserved.
// Use of this source code is governed by a license that can be found
// in the LICENSE file.

#include "UInt32UniqueLinearIndex.h"

// Project headers
#include "../ikt/UInt32IndexKeyTraits.h"

namespace siodb::iomgr::dbengine {

UInt32UniqueLinearIndex::UInt32UniqueLinearIndex(Table& table, std::string&& name,
        std::size_t valueSize, const IndexColumnSpecification& columnSpec,
        std::uint32_t dataFileSize, std::optional<std::string>&& description)
    : UniqueLinearIndex(table, IndexType::kLinearIndexU32, std::move(name), UInt32IndexKeyTraits(),
            valueSize, &UInt32IndexKeyTraits::compareKeys, columnSpec, dataFileSize,
            std::move(description))
{
}

UInt32UniqueLinearIndex::UInt32UniqueLinearIndex(
        Table& table, const IndexRecord& indexRecord, std::size_t valueSize)
    : UniqueLinearIndex(table, indexRecord, UInt32IndexKeyTraits(), valueSize,
            &UInt32IndexKeyTraits::compareKeys)
{
}

}  // namespace siodb::iomgr::dbengine
