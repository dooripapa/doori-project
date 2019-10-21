// Copyright 2019 The doori-project Authors. All rights reserved.
// Use of this source code is governed by a GPL-3.0 license that can be found in the LICENSE file.
// See the AUTHORS file for names of contributors.
//
// Created by doori on 19. 7. 25.
//
#pragma once
#include <array>

namespace doori{

class Error {
public:
    auto static singleton() -> Error&;
    auto occur(int errnum, bool status, std::string cause) noexcept -> void;
    template<int N>
    auto occur(int errnum, bool status, char const(&value)[N]) noexcept -> void;
    ~Error() = default;
    Error(Error&& rhs) = delete;
    Error(const Error& rhs) = delete;
    auto operator=(Error&& rhs) -> Error& = delete;
    auto operator=(const Error& rhs) -> Error& = delete;
    auto clear() noexcept -> void;
    auto Status() const noexcept -> bool;
    auto Errno() const noexcept -> int;
    auto Cause() const noexcept -> const std::string&;

private:
    bool mStatus;
    int mErrno;
    std::string mCause;
    Error();
    static Error oneInstance;
};

template<int N>
auto Error::occur(int errnum, bool status, char const(&value)[N]) noexcept -> void
{
    mErrno = errnum;
    mStatus = status;
    mCause = std::string(value);
}

inline bool CHECK() {
    return Error::singleton().Status();
}

inline void EVENT(int system_errno, bool status_value, std::string error_cause) {
    Error::singleton().occur(system_errno, status_value, error_cause);
}

inline void CLEAR(){
    Error::singleton().clear();
}

inline Error& WHAT(){
    return Error::singleton();
}

}
