// Copyright (C) 2019-2020 Siodb GmbH. All rights reserved.
// Use of this source code is governed by a license that can be found
// in the LICENSE file.

#include "Int16UniqueLinearIndex.h"

// Project headers
#include "../ikt/Int16IndexKeyTraits.h"

namespace siodb::iomgr::dbengine {

Int16UniqueLinearIndex::Int16UniqueLinearIndex(Table& table, const std::string& name,
        std::size_t valueSize, const IndexColumnSpecification& columnSpec,
        std::uint32_t dataFileSize)
    : UniqueLinearIndex(table, IndexType::kLinearIndexI16, name, Int16IndexKeyTraits(), valueSize,
            &Int16IndexKeyTraits::compareKeys, columnSpec, dataFileSize)
{
}

Int16UniqueLinearIndex::Int16UniqueLinearIndex(
        Table& table, const IndexRecord& indexRecord, std::size_t valueSize)
    : UniqueLinearIndex(
            table, indexRecord, Int16IndexKeyTraits(), valueSize, &Int16IndexKeyTraits::compareKeys)
{
}

}  // namespace siodb::iomgr::dbengine
