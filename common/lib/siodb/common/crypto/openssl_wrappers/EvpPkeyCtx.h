// Copyright (C) 2019-2020 Siodb GmbH. All rights reserved.
// Use of this source code is governed by a license that can be found
// in the LICENSE file.

#pragma once

// Project headers
#include "../../utils/HelperMacros.h"
#include "../OpenSslError.h"

// OpenSSL headers
#include <openssl/evp.h>

namespace siodb::crypto {

/** Wrapper for the OpenSSL EVP_PKEY_CTX. */
class EvpPKeyCtx {
public:
    /**
     * Initializes object of class EvpPKeyCtx.
     * @throw OpenSslError if context could not be created
     */
    EvpPKeyCtx(EVP_PKEY* key, ENGINE* engine)
        : m_ctx(EVP_PKEY_CTX_new(key, engine))
    {
        if (!m_ctx) throw OpenSslError("EVP_PKEY_CTX_new failed");
    }

    DECLARE_NONCOPYABLE(EvpPKeyCtx);

    /**
     * Deinitializes object.
     */
    ~EvpPKeyCtx()
    {
        EVP_PKEY_CTX_free(m_ctx);
    }

    /**
     * Converts class into EVP_PKEY_CTX*.
     * @return Context.
     */
    operator EVP_PKEY_CTX*() noexcept
    {
        return m_ctx;
    }

    /**
     * Converts class into const EVP_PKEY_CTX*.
     * @return Const context.
     */
    operator const EVP_PKEY_CTX*() const noexcept
    {
        return m_ctx;
    }

private:
    /** Context */
    EVP_PKEY_CTX* m_ctx;
};

}  // namespace siodb::crypto