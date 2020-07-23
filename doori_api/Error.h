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
    /**
     * 싱글톤을 위한 공개함수.
     * @return 복사본이 없는 자기자신의 참조값
     * @note 직접접근해서 사용하지 말것.
     */
    auto static singleton() -> Error&;
    /**
     * 에러가 생겼다면, 이 함수로 셋팅한다.
     * @param errnum : system call errnum
     * @param status : bool true 성공, false 실패
     * @param cause : 에러를 설명할 문자열
     */
    auto occur(int errnum, bool status, const std::string& cause) noexcept -> void;
    /**
     * 에러가 생겼다면, 이 함수로 셋팅한다.
     * @param errnum : system call errnum
     * @param status : bool true 성공, false 실패
     * @param cause : 에러를 설명할 c스타일 리터럴 문자열
     */
    template<int N>
    auto occur(int errnum, bool status, char const(&value)[N]) noexcept -> void;
    /**
     * 소멸자
     */
    ~Error() = default;
    /**
     * 이동생성자 호출 금지
     * @param rhs : Error&&
     */
    Error(Error&& rhs) = delete;
    /**
     * 복사생성자 호출 금지
     * @param rhs : const Error&
     */
    Error(const Error& rhs) = delete;
    /**
     * 대입이동연산자 호출 금지
     * @param rhs : Error&&
     * @return Error&
     */
    auto operator=(Error&& rhs) -> Error& = delete;
    /**
     * 대입복사생성자 호출 금지
     * @param rhs : const Error&
     * @return Error&
     */
    auto operator=(const Error& rhs) -> Error& = delete;
    /**
     * Error객체를 초기화함.
     */
    auto clear() noexcept -> void;
    /**
     * Error의 상태값 리턴함
     * @return bool : true 성공, false 실패
     */
    auto Status() const noexcept -> bool;
    /**
     * Error의 errno 리턴함
     * @return int : errno
     * @see errno은 보통 시스템콜함수의 errno을 셋팅함
     */
    auto Errno() const noexcept -> int;
    /**
     * Error의 description
     * @return const std::string& : 에러 설명 문자열
     */
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

/**
 * Error 상태값
 * @return bool : true 성공, false 실패
 */
inline bool CHECK() {
    return Error::singleton().Status();
}

/**
 * 에러가 생겼다면, 이 함수로 셋팅한다.
 * @param system_errno
 * @param status_value
 * @param error_cause
 */
inline void EVENT(int system_errno, bool status_value, std::string error_cause) {
    Error::singleton().occur(system_errno, status_value, error_cause);
}

/**
 * Error 초기화
 */
inline void CLEAR(){
    Error::singleton().clear();
}

/**
 * Error description
 * @return const std::string& : Error description, 에러의 설명문자열
 */
inline const std::string& WHAT(){
    return Error::singleton().Cause();
}

}
