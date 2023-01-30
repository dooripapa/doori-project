// Copyright 2019 The doori-project Authors. All rights reserved.
// Use of this source code is governed by a GPL-3.0 license that can be found in the LICENSE file.
// See the AUTHORS file for names of contributors.
//
// Created by doori on 19. 7. 25.
//
#include "Error.h"

namespace doori{

Error Error::oneInstance;

Error::Error() : mErrno(0), mStatus(true), mCause("") {}

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

auto Error::singleton() -> Error& {
    return Error::oneInstance;
}

auto Error::Status() const noexcept -> bool {
    return mStatus;
}

auto Error::clear() noexcept -> void {
    mErrno = 0;
    mCause = std::string("");
    mStatus = true;
}

}
