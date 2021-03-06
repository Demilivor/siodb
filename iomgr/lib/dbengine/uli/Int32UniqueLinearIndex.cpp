// Copyright (C) 2019-2020 Siodb GmbH. All rights reserved.
// Use of this source code is governed by a license that can be found
// in the LICENSE file.

#include "Int32UniqueLinearIndex.h"

// Project headers
#include "../ikt/Int32IndexKeyTraits.h"

namespace siodb::iomgr::dbengine {

Int32UniqueLinearIndex::Int32UniqueLinearIndex(Table& table, std::string&& name,
        std::size_t valueSize, const IndexColumnSpecification& columnSpec,
        std::uint32_t dataFileSize, std::optional<std::string>&& description)
    : UniqueLinearIndex(table, IndexType::kLinearIndexI32, std::move(name), Int32IndexKeyTraits(),
            valueSize, &Int32IndexKeyTraits::compareKeys, columnSpec, dataFileSize,
            std::move(description))
{
}

Int32UniqueLinearIndex::Int32UniqueLinearIndex(
        Table& table, const IndexRecord& indexRecord, std::size_t valueSize)
    : UniqueLinearIndex(
            table, indexRecord, Int32IndexKeyTraits(), valueSize, &Int32IndexKeyTraits::compareKeys)
{
}

}  // namespace siodb::iomgr::dbengine
