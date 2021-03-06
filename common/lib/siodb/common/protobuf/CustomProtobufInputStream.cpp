// Copyright (C) 2019-2020 Siodb GmbH. All rights reserved.
// Use of this source code is governed by a license that can be found
// in the LICENSE file.

#include "CustomProtobufInputStream.h"

// Project headers
#include "../io/FileIO.h"
#include "../net/ConnectionError.h"

// CRT headers
#include <cstring>

// System headers
#include <unistd.h>

// Protobuf headers
#include <google/protobuf/stubs/logging.h>

// This code is largely based on the Google Protocol Buffers implementation
// of the FileInputStream. See
// https://github.com/protocolbuffers/protobuf/blob/master/src/google/protobuf/io/zero_copy_stream_impl.h
// https://github.com/protocolbuffers/protobuf/blob/master/src/google/protobuf/io/zero_copy_stream_impl.cc
// for more details.

namespace siodb::protobuf {

CustomProtobufInputStream::CustomProtobufInputStream(
        io::IoBase& io, const utils::ErrorCodeChecker& errorCodeChecker, int blockSize)
    : m_copyingInput(io, errorCodeChecker)
    , m_impl(&m_copyingInput, blockSize)
{
}

bool CustomProtobufInputStream::Close()
{
    return m_copyingInput.Close();
}

bool CustomProtobufInputStream::Next(const void** data, int* size)
{
    return m_impl.Next(data, size);
}

void CustomProtobufInputStream::BackUp(int count)
{
    m_impl.BackUp(count);
}

bool CustomProtobufInputStream::Skip(int count)
{
    return m_impl.Skip(count);
}

google::protobuf::int64 CustomProtobufInputStream::ByteCount() const
{
    return m_impl.ByteCount();
}

CustomProtobufInputStream::CopyingInputStream::CopyingInputStream(
        io::IoBase& io, const utils::ErrorCodeChecker& errorCodeChecker)
    : m_errorCodeChecker(errorCodeChecker)
    , m_io(io)
    , m_closeOnDelete(false)
    , m_closed(false)
    , m_errno(0)
    , m_prevSeekFailed(false)
{
}

CustomProtobufInputStream::CopyingInputStream::~CopyingInputStream()
{
    if (m_closeOnDelete) {
        if (!Close()) {
            GOOGLE_LOG(ERROR) << "close() failed: " << std::strerror(m_errno);
        }
    }
}

bool CustomProtobufInputStream::CopyingInputStream::Close()
{
    GOOGLE_CHECK(!m_closed);

    m_closed = true;
    if (!m_io.close()) {
        // The docs on close() do not specify whether a file descriptor is still
        // open after close() fails with EIO. However, the glibc source code
        // seems to indicate that it is not.
        m_errno = errno;
        return false;
    }

    return true;
}

int CustomProtobufInputStream::CopyingInputStream::Read(void* buffer, int size)
{
    GOOGLE_CHECK(!m_closed);

    int result;
    do {
        result = m_io.read(buffer, size);
    } while (
            result < 0
            // Here is our custom logic comes into the game.
            // Now, depending on external condition, we, for example, can treat EINTR as real error.
            && !m_errorCodeChecker.isError(errno));

    if (result < 0) {
        // Read error (not EOF).
        m_errno = errno;
    } else if (errno == 0 && result == 0) {
        // For TCP connection 0 bytes read result without errno set
        // means connection was closed or aborted.
        throw net::ConnectionError("ProtobufInputStream: Connection closed");
    }

    return result;
}

int CustomProtobufInputStream::CopyingInputStream::Skip(int count)
{
    GOOGLE_CHECK(!m_closed);

    if (!m_prevSeekFailed && m_io.skip(count) != (off_t) -1) {
        // Seek succeeded.
        return count;
    } else {
        // Failed to seek.
        // Note to self: Don't seek again, this file descriptor seems to don't support it.
        m_prevSeekFailed = true;
        // Use the default implementation.
        return google::protobuf::io::CopyingInputStream::Skip(count);
    }
}

}  // namespace siodb::protobuf
