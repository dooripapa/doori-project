// Copyright 2019 The doori-project Authors. All rights reserved.
// Use of this source code is governed by a GPL-3.0 license that can be found in the LICENSE file.
// See the AUTHORS file for names of contributors.
//
// Created by doori on 19. 7. 25.
//
#include <cstring>
#include "Error.h"

namespace doori::api::Common{

    auto Error::occur(int errnum, bool status, const std::string& cause) noexcept -> void
    {
        mErrno = errnum;
        mStatus = status;
        mCause = cause;
    }

    auto Error::Errno() const noexcept -> int {
        return mErrno;
    }

    auto Error::Cause() const noexcept -> const std::string & {
        return mCause;
    }

    auto Error::Status() const noexcept -> bool {
        return mStatus;
    }

    auto Error::AsSuccess() noexcept -> void {
        mErrno = 0;
        mCause = std::string("");
        mStatus = true;
    }

    void Error::InjectedBySystemcallError() noexcept{
        mErrno = errno;
        if( mErrno > 0 ) {
            mStatus = false;

            char errorStr[1024] = {0};
            auto p = strerror_r(errno, errorStr, sizeof(errorStr) );

            mCause = p;
        }
        else {
            AsSuccess();
        }
    }

    void Error::InjectedByClientError(const string &cause) noexcept{
        mErrno = -1;
        mStatus = false;
        mCause = cause;
    }

    Error::Error(int errnum , bool status ) : mErrno{errnum}, mStatus{status} {

    }
}
