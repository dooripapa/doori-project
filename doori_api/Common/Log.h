// Copyright 2019 The doori-project Authors. All rights reserved.
// Use of this source code is governed by a GPL-3.0 license that can be found in the LICENSE file.
// See the AUTHORS file for names of contributors.
//
// Created by doori on 19. 7. 25.
//
#pragma once
#include <unistd.h>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <mutex>
#include <iomanip>
#include <fstream>

using namespace std;

namespace doori::Common{
    using std::chrono::system_clock;

    class Log
    {
    public:
        enum class LEVEL:unsigned int{D=1,I=2,W=3,E=4,F=5};
        auto static convertToLevel(const string& level) -> LEVEL;
        auto static logging() -> Log&;
        auto setLogEnv(const string& path, LEVEL level) -> void;

        template<typename T>
        auto log(T value) -> void;

        template<typename T, typename... Tlist>
        auto log(T arg, Tlist ... args) -> void;

        template<typename T>
        auto writeLog(LEVEL level
                    ,const char *fileName
                    ,const char *funcName
                    ,int codeLine
                    ,T arg) -> void;

        template<typename T, typename... Tlist>
        auto writeLog(LEVEL level
                    ,const char *fileName
                    ,const char *funcName
                    ,int codeLine
                    ,T arg, Tlist ... args) -> void;

    private:
        ofstream	mLogfile;
        LEVEL	    mlevel;
        mutex		_logMutex;
        static Log instance;

        Log();
        ~Log();

        auto _writeLog( LEVEL level ) -> void;
        template<typename T>
        auto _writeLog(T t) -> void
        {
            if(!mLogfile)
                cout<<t;
            else
                mLogfile<<t;
        };
        auto _writeLineLog() -> void;
        auto _writeLogLevel(ofstream& of, LEVEL level) -> void;
    };
}//namespace doori

#include "Log.hpp"

#define DEBUG	doori::Common::Log::LEVEL::D
#define INFO	doori::Common::Log::LEVEL::I
#define WARN	doori::Common::Log::LEVEL::W
#define ERROR	doori::Common::Log::LEVEL::E
#define FATAL	doori::Common::Log::LEVEL::F

#define LOG_CREATE(PATH, LEVEL) doori::Common::Log::logging().setLogEnv(PATH, LEVEL);
#define LOG(LEVEL, ...) doori::Common::Log::logging().writeLog(LEVEL, __FILE__, __FUNCTION__, __LINE__, __VA_ARGS__)

#ifdef TRACKING
	#define TRACE doori::Common::Log::logging().writeLog(DEBUG, __FILE__, __PRETTY_FUNCTION__, __LINE__,"TRACKING");
#endif

