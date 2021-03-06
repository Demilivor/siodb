// Copyright (C) 2019-2020 Siodb GmbH. All rights reserved.
// Use of this source code is governed by a license that can be found
// in the LICENSE file.

#include "CustomProtobufOutputStream.h"

namespace siodb::protobuf {

CustomProtobufOutputStream::CustomProtobufOutputStream(
        io::IoBase& io, const utils::ErrorCodeChecker& errorCodeChecker, int blockSize)
    : m_copyingOutput(io, errorCodeChecker)
    , m_impl(&m_copyingOutput, blockSize)
{
}

bool CustomProtobufOutputStream::Next(void** data, int* size)
{
    return m_impl.Next(data, size);
}

void CustomProtobufOutputStream::BackUp(int count)
{
    m_impl.BackUp(count);
}

google::protobuf::int64 CustomProtobufOutputStream::ByteCount() const
{
    return m_impl.ByteCount();
}

CustomProtobufOutputStream::CopyingOutputStream::CopyingOutputStream(
        io::IoBase& io, const utils::ErrorCodeChecker& errorCodeChecker)
    : m_errorCodeChecker(errorCodeChecker)
    , m_io(io)
    , m_errno(0)
{
}

bool CustomProtobufOutputStream::CopyingOutputStream::Write(const void* buffer, int size)
{
    int result = 0;
    do {
        result = m_io.write(buffer, size);
    } while (
            result < 0
            // Here is our custom logic comes into the game.
            // Now, depending on external condition, we, for example, can treat EINTR as real error.
            && !m_errorCodeChecker.isError(errno));

    if (result < 0) {
        // Read error (not EOF).
        m_errno = errno;
    }

    return result;
}

}  // namespace siodb::protobuf
