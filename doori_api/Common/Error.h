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

namespace doori::api::Common {

    class Error {


    public:

        explicit Error(int errnum = -1, bool status = false);
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
         * Error객체를 정상으로 초기화함.
         */
        auto AsSuccess() noexcept -> void;

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
        void InjectedBySystemcallError() noexcept;

		
		/**
		 * @brief 시스템 호출을 사용하여 오류 메시지를 기록합니다.
		 * 
		 * 이 함수는 파일 이름, 함수 이름, 라인 번호 및 오류 원인을 포함하는 문자열을 사용하여 오류 메시지를 기록합니다. 그런 다음 시스템 호출을 사용하여 오류 메시지를 기록합니다.
		 * 
		 * @tparam FILENAME 오류가 발생한 파일의 이름입니다.
		 * @tparam FUNCNAME 오류가 발생한 함수의 이름입니다.
		 * @tparam LINE 오류가 발생한 라인 번호입니다.
		 * @tparam N 오류 원인을 포함하는 문자열의 크기입니다.
		 * @param FILENAME 오류가 발생한 파일의 이름입니다.
		 * @param FUNCNAME 오류가 발생한 함수의 이름입니다.
		 * @param LINE 오류가 발생한 라인 번호입니다.
		 * @param loggingCause 오류 원인을 포함하는 문자열입니다.
		 */
		template<typename FILENAME, typename FUNCNAME, typename LINE, int N>
		void loggingBySystemcallError(FILENAME, FUNCNAME, LINE, char const(&loggingCause)[N]) noexcept;

        template<typename FILENAME, typename FUNCNAME, typename LINE, typename STRING_CAUSE>
        void loggingBySystemcallError(FILENAME filename, FUNCNAME funcname, LINE line, STRING_CAUSE stringCause) noexcept;

        /**
         * 호출자의 의한 에러 셋팅.
         * @note -1, false 내부적으로 셋팅됨.
         * @param cause
         */
        inline void InjectedByClientError(const string &cause) noexcept;

        template<int N>
        auto InjectedByClientError(char const(&value)[N]) noexcept -> void;

        template<typename FILENAME, typename FUNCNAME, typename LINE, int N>
        void loggingByClientError(FILENAME, FUNCNAME, LINE, char const(&loggingCause)[N]) noexcept;

        template<typename FILENAME, typename FUNCNAME, typename LINE, typename STRING_CAUSE>
        void loggingByClientError(FILENAME filename, FUNCNAME funcname, LINE line, STRING_CAUSE stringCause) noexcept;

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

    /**
     * 명명규칙을 통일하기 위해서, loggingByClientError 함수는 LoggingByClientError 호출함.
     * 함수 호출 시 __FILE__, __FUNCTION__, __LINE__ 자동으로 입력처리되도록 하기 위해서 한번 더 템플릿 및 define문으로 감싼다.
     */
    // const char* 인경우
    template<typename FILENAME, typename FUNCNAME, typename LINE, int N>
    void Error::loggingByClientError(FILENAME filename, FUNCNAME funcname, LINE line, char const(&loggingCause)[N]) noexcept  {
        mErrno = -1;
        mStatus = false;
        mCause = std::string(loggingCause);
        doori::api::Common::Log::logging().writeLog(doori::api::Common::Log::LEVEL::E, filename, funcname, line, loggingCause);
    }

    // string 인 경우
    template<typename FILENAME, typename FUNCNAME, typename LINE, typename STRING_CAUSE>
    void Error::loggingByClientError(FILENAME filename, FUNCNAME funcname, LINE line, STRING_CAUSE stringCause) noexcept  {
        mErrno = -1;
        mStatus = false;
        mCause = stringCause;
        doori::api::Common::Log::logging().writeLog(doori::api::Common::Log::LEVEL::E, filename, funcname, line, stringCause);
    }

#define LoggingByClientError(...) loggingByClientError <const char*, const char*, int, decltype(__VA_ARGS__)> (__FILE__, __FUNCTION__, __LINE__, __VA_ARGS__)

    /**
     * 명명규칙을 통일하기 위해서, loggingBySystemcallError 함수는 LoggingBySystemcallError 호출함.
     * 함수 호출 시 __FILE__, __FUNCTION__, __LINE__ 자동으로 입력처리되도록 하기 위해서 한번 더 템플릿 및 define문으로 감싼다.
     */
    template<typename FILENAME, typename FUNCNAME, typename LINE, int N>
    void Error::loggingBySystemcallError(FILENAME filename, FUNCNAME funcname, LINE line, char const(&loggingCause)[N]) noexcept {
        mErrno = errno;
        if( mErrno > 0 ) {
            mStatus = false;

            char errorStr[1024] = {0};
            auto p = strerror_r(errno, errorStr, sizeof(errorStr) );

            mCause = p;

            doori::api::Common::Log::logging().writeLog(doori::api::Common::Log::LEVEL::F, filename, funcname, line, loggingCause, ", errno:", mErrno, ", cause:", mCause);
        }
        else {
            AsSuccess();
        }
    }

    template<typename FILENAME, typename FUNCNAME, typename LINE, typename STRING_CAUSE>
    void Error::loggingBySystemcallError(FILENAME filename, FUNCNAME funcname, LINE line, STRING_CAUSE stringCause) noexcept {
        mErrno = errno;
        if( mErrno > 0 ) {
            mStatus = false;

            char errorStr[1024] = {0};
            auto p = strerror_r(errno, errorStr, sizeof(errorStr) );

            mCause = p;

            doori::api::Common::Log::logging().writeLog(doori::api::Common::Log::LEVEL::F, filename, funcname, line, stringCause, ", errno:", mErrno, ", cause:", mCause);
        }
        else {
            AsSuccess();
        }
    }

#define LoggingBySystemcallError(...) loggingBySystemcallError <const char*, const char*, int, decltype(__VA_ARGS__)> (__FILE__, __FUNCTION__, __LINE__, __VA_ARGS__)

} //doori
