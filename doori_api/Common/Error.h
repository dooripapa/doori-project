// Copyright 2019 The doori-project Authors. All rights reserved.
// Use of this source code is governed by a GPL-3.0 license that can be found in the LICENSE file.
// See the AUTHORS file for names of contributors.
//
// Created by doori on 19. 7. 25.
//
#pragma once
#include <array>
#include <string>
#include "Log.h"

using namespace std;

namespace doori::Common {

    class Error {

    public:

        Error();
        Error(Error&& rhs) = default;
        Error(const Error& rhs) = default;
        auto operator=(Error&& rhs) -> Error& = default;
        auto operator=(const Error& rhs) -> Error& = default;

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
        virtual ~Error() = default;

        /**
         * Error객체를 초기화함.
         */
        auto Clear() noexcept -> void;

        /**
         * Error의 상태값 리턴함
         * @return bool : true 성공, false 실패
         */
        [[nodiscard]] auto Status() const noexcept -> bool;

        /**
         * Error의 errno 리턴함
         * @return int : errno
         * @see errno은 보통 시스템콜함수의 errno을 셋팅함
         */
        [[nodiscard]] auto Errno() const noexcept -> int;

        /**
         * Error의 description
         * @return const std::string& : 에러 설명 문자열
         */
        [[nodiscard]] auto Cause() const noexcept -> const std::string&;

    protected:

        /**
         * 시스템 콜 함수 호출 후 에러가 발생된 경우,
         * Error 객체를 상태를 셋팅한다.
         */
        void InjectedBySystemcall() noexcept;
        void LoggingBySystemcall(const string& loggingCause) noexcept;

        template<int N>
        void LoggingBySystemcallError(char const(&loggingCause)[N]) noexcept;


        /**
         * 호출자의 의한 에러 셋팅.
         * @note -1, false 내부적으로 셋팅됨.
         * @param cause
         */
        inline void InjectedByClientError(const string &cause) noexcept;
        inline void LoggingByClientError(const string& cause) noexcept;

        template<int N>
        auto InjectedByClientError(char const(&value)[N]) noexcept -> void;

        template<int N>
        void LoggingByClientError(char const(&loggingCause)[N]) noexcept;

    private:
        bool mStatus;
        int mErrno;
        std::string mCause;
    }; //class

    template<int N>
    auto Error::occur(int errnum, bool status, char const(&value)[N]) noexcept -> void
    {
        mErrno = errnum;
        mStatus = status;
        mCause = std::string(value);
    }

    template<int N>
    auto Error::InjectedByClientError(const char (&value)[N]) noexcept -> void {
        mErrno = -1;
        mStatus = false;
        mCause = std::string(value);
    }

    template<int N>
    void Error::LoggingByClientError(const char (&loggingCause)[N]) noexcept  {
        mErrno = -1;
        mStatus = false;
        mCause = std::string(loggingCause);
        LOG(ERROR, loggingCause);
    }

    template<int N>
    void Error::LoggingBySystemcallError(const char (&loggingCause)[N]) noexcept {
        mErrno = errno;
        if( mErrno > 0 ) {
            mStatus = false;

            char errorStr[1024] = {0};
            auto p = strerror_r(errno, errorStr, sizeof(errorStr) );

            mCause = p;

            LOG(ERROR, loggingCause, ", errno:", mErrno, ", cause:", mCause);
        }
        else {
            Clear();
        }
    }
} //doori
