// Copyright 2019 The doori-project Authors. All rights reserved.
// Use of this source code is governed by a GPL-3.0 license that can be found in the LICENSE file.
// See the AUTHORS file for names of contributors.
//
// Created by doori on 19. 7. 25.
//

#include "Log.h"

namespace doori::api::Common{


    template<typename T>
    auto Log::log(T t) -> void
    {
		*mLogfile<<t;
    }

    template<typename T, typename... Args>
    auto Log::plog(const char* format, const T& value, const Args&... args) -> void
    {
        while (*format) {
            if (*format == '%' && *(format + 1) == 't') {
                log(value);
                format += 2;
                plog(format, args...);
                return;
            }
            *mLogfile << *format++;
        }
        throw std::runtime_error("Excess arguments provided");
    }

    template<typename T, typename... Tlist>
    auto Log::log(T arg, Tlist... args) -> void
    {
        log( arg );
        log( args... );
    }

    template<typename... Tlist>
    auto Log::writeLog(LEVEL level
                        ,const char *fileName
                        ,const char *funcName
                        ,const int codeLine
                        ,Tlist... args) -> void
    {
		if( mLevel > level )
			return;

        //lock_guard 생성자를, 생성하자 마자 락을 시도
        //소멸시, mutex(_logMutex) 해제
        lock_guard<mutex>	logFileLck( _logMutex );
        // 디버깅 코드를 기록
        log( level );

    #if 1
        //2001-08-23 14:55:02
        time_t rawtime;
        char acTime[20] ={0,};
        time (&rawtime);
        struct tm *timeinfo = localtime (&rawtime);
        strftime (acTime, 20,"%F %T",timeinfo);
    #else
        //put_time, is third party library, not support by gcc 4.8 or 4.9
        time_t tt = system_clock::to_time_t (system_clock::now());
        struct std::tm * ptm = std::localtime(&tt);
        mLogfile << setfill(' ') << setw(20) << put_time(ptm,"%F %X"); //not supported by gcc
    #endif
		
        auto strFilename = string(fileName);
        size_t pos = strFilename.rfind("/");
        if(pos != std::string::npos)
            strFilename = strFilename.substr(pos+1);
        auto strFuncname = string(funcName);

		mLogfile->imbue(std::locale("C.UTF-8"));

		*mLogfile << acTime << setfill(' ') ;
		*mLogfile << '|' << setfill(' ') << setw(25) << std::left  << (strFilename.size()>25?strFilename.substr(strFilename.size()-25,25):strFilename);
		*mLogfile << '|' << setfill(' ') << setw(25) << std::left  << (strFuncname.size()>25?strFuncname.substr(strFuncname.size()-25,25):strFuncname);
		*mLogfile << '|' << setfill(' ') << setw( 5) << std::right << codeLine ;
		*mLogfile << '|';

        log( args... );

        _writeLineLog();
    }

    template<typename T, typename... Args>
    auto Log::printLog(LEVEL level
            ,const char *fileName
            ,const char *funcName
            ,int codeLine
            ,const char* format
            ,const T& value
            ,const Args&... args) -> void
    {
        if( mLevel > level )
            return;

        //lock_guard 생성자를, 생성하자 마자 락을 시도
        //소멸시, mutex(_logMutex) 해제
        lock_guard<mutex>	logFileLck( _logMutex );
        // 디버깅 코드를 기록
        log( level );

#if 1
        //2001-08-23 14:55:02
        time_t rawtime;
        char acTime[20] ={0,};
        time (&rawtime);
        struct tm *timeinfo = localtime (&rawtime);
        strftime (acTime, 20,"%F %T",timeinfo);
#else
        //put_time, is third party library, not support by gcc 4.8 or 4.9
        time_t tt = system_clock::to_time_t (system_clock::now());
        struct std::tm * ptm = std::localtime(&tt);
        mLogfile << setfill(' ') << setw(20) << put_time(ptm,"%F %X"); //not supported by gcc
#endif

        auto strFilename = string(fileName);
        size_t pos = strFilename.rfind("/");
        if(pos != std::string::npos)
            strFilename = strFilename.substr(pos+1);
        auto strFuncname = string(funcName);

        mLogfile->imbue(std::locale("C.UTF-8"));

        *mLogfile << acTime << setfill(' ') ;
        *mLogfile << '|' << setfill(' ') << setw(25) << std::left  << (strFilename.size()>25?strFilename.substr(strFilename.size()-25,25):strFilename);
        *mLogfile << '|' << setfill(' ') << setw(25) << std::left  << (strFuncname.size()>25?strFuncname.substr(strFuncname.size()-25,25):strFuncname);
        *mLogfile << '|' << setfill(' ') << setw( 5) << std::right << codeLine ;
        *mLogfile << '|';

        plog( format, value, args... );

        _writeLineLog();
    }

}//namespace doori
