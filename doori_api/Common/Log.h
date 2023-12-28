// Copyright 2019 The doori-project Authors. All rights reserved.
// Use of this source code is governed by a GPL-3.0 license that can be found in the LICENSE file.
// See the AUTHORS file for names of contributors.
//
// Created by doori on 19. 7. 25.
//
#pragma clang diagnostic push
#pragma ide diagnostic ignored "NotImplementedFunctions"
#pragma once
#include <unistd.h>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <mutex>
#include <iomanip>
#include <fstream>

using namespace std;

namespace doori::api::Common{
    using std::chrono::system_clock;

    class Log
    {
    public:
        enum class LEVEL
		{
			D=0,
			I=1,
			W=2,
			E=3,
			F=4
		};

        auto static convertToLevel(const string& level) -> LEVEL;
        auto static logging() -> Log&;
        auto setLogEnv(const string& path, LEVEL level) -> void;

		auto log(LEVEL level) -> void;

        template<typename T>
        auto log(T value) -> void;

        auto plog(const char *format) -> void;

        template<typename T, typename... Args>
        auto plog(const char* format, const T& value, const Args&... args) -> void;

        template<typename T, typename... Tlist>
        auto log(T arg, Tlist ... args) -> void;

        template<typename... Tlist>
        auto writeLog(LEVEL level
                    ,const char *fileName
                    ,const char *funcName
                    ,int codeLine
                    ,Tlist ... args) -> void;

        template<typename T, typename... Args>
        auto printLog(LEVEL level
                ,const char *fileName
                ,const char *funcName
                ,int codeLine
                ,const char* format
                ,const T& value
                ,const Args&... args) -> void;

    private:
        ostream	    *mLogfile;
        LEVEL	    mLevel;
        mutex		_logMutex;
        static Log instance;   //singleton

		Log();
        ~Log();

        auto _writeLineLog() -> void;
    };

}//namespace doori

#include "Log.hpp"

#define DEBUG	doori::api::Common::Log::LEVEL::D
#define INFO	doori::api::Common::Log::LEVEL::I
#define WARN	doori::api::Common::Log::LEVEL::W
#define ERROR	doori::api::Common::Log::LEVEL::E
#define FATAL	doori::api::Common::Log::LEVEL::F

#define LOG_CREATE(PATH, LEVEL) doori::api::Common::Log::logging().setLogEnv(PATH, LEVEL);
#define LOG(LEVEL, ...) doori::api::Common::Log::logging().writeLog(LEVEL, __FILE__, __FUNCTION__, __LINE__, __VA_ARGS__)
#define PRINT(LEVEL, format, ...) doori::api::Common::Log::logging().printLog(LEVEL, __FILE__, __FUNCTION__, __LINE__, format, __VA_ARGS__)

#pragma clang diagnostic pop