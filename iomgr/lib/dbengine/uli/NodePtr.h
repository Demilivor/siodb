// Copyright (C) 2019-2020 Siodb GmbH. All rights reserved.
// Use of this source code is governed by a license that can be found
// in the LICENSE file.

#pragma once

// STL headers
#include <memory>

namespace siodb::iomgr::dbengine::uli {

struct Node;

/** Node shared pointer shortcut type */
using NodePtr = std::shared_ptr<Node>;

}  // namespace siodb::iomgr::dbengine::uli
