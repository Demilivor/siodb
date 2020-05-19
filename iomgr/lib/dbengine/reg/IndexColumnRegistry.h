// Copyright (C) 2019-2020 Siodb GmbH. All rights reserved.
// Use of this source code is governed by a license that can be found
// in the LICENSE file.

// Project headers
#include "IndexColumnRecord.h"

// Boost header
#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index_container.hpp>

namespace siodb::iomgr::dbengine {

/** In-memory column set column registry */
class IndexColumnRegistry {
public:
    /** Registry record type */
    using value_type = IndexColumnRecord;

private:
    /** Index tag */
    struct ByIdTag {
    };

    /** Index tag */
    struct ByIndexIdTag {
    };

    /** Index tag */
    struct ByColumnDefinitionIdTag {
    };

    /** Registry data container type */
    typedef boost::multi_index::multi_index_container<value_type,
            boost::multi_index::indexed_by<
                    boost::multi_index::hashed_unique<boost::multi_index::tag<ByIdTag>,
                            boost::multi_index::member<value_type, decltype(value_type::m_id),
                                    &value_type::m_id>>,
                    boost::multi_index::hashed_non_unique<boost::multi_index::tag<ByIndexIdTag>,
                            boost::multi_index::member<value_type, decltype(value_type::m_indexId),
                                    &value_type::m_indexId>>,
                    boost::multi_index::hashed_non_unique<
                            boost::multi_index::tag<ByColumnDefinitionIdTag>,
                            boost::multi_index::member<value_type,
                                    decltype(value_type::m_columnDefinitionId),
                                    &value_type::m_columnDefinitionId>>>>
            Container;

public:
    /**
     * Returns read-only index by index column record ID.
     * @return Registry index object.
     */
    const auto& byId() const noexcept
    {
        return m_container.get<ByIdTag>();
    }

    /**
     * Returns read-only index by index ID.
     * @return Registry index object.
     */
    const auto& byIndexId() const noexcept
    {
        return m_container.get<ByIndexIdTag>();
    }

    /**
     * Returns read-only index by constraint id.
     * @return Registry index object.
     */
    const auto& byColumnDefinitionId() const noexcept
    {
        return m_container.get<ByIdTag>();
    }

    /**
     * Returns indication that the registry is empty.
     * @return true if registry is empty, false otherwise.
     */
    bool empty() const noexcept
    {
        return m_container.empty();
    }

    /**
     * Returns number of records in the registry.
     * @return Number of records in the registry.
     */
    std::size_t size() const noexcept
    {
        return m_container.size();
    }

    /**
     * Inserts a record into the registry.
     * @param args Column set column record construction arguments.
     */
    template<typename... Args>
    void emplace(Args&&... args)
    {
        m_container.emplace(std::forward<Args>(args)...);
    }

    /**
     * Inserts a record into the registry.
     * @param record Column set column record.
     */
    void insert(const value_type& record)
    {
        m_container.insert(record);
    }

    /**
     * Inserts a record into the registry.
     * @param record Column set column record.
     */
    void insert(value_type&& record)
    {
        m_container.insert(std::move(record));
    }

    /** Clears registry */
    void clear() noexcept
    {
        m_container.clear();
    }

    /**
     * Swaps content with another registry.
     * @param other Other registry.
     */
    void swap(IndexColumnRegistry& other) noexcept
    {
        if (&other != this) m_container.swap(other.m_container);
    }

private:
    /** Data container */
    Container m_container;
};

}  // namespace siodb::iomgr::dbengine
