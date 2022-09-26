// Copyright 2019 The doori-project Authors. All rights reserved.
// Use of this source code is governed by a GPL-3.0 license that can be found in the LICENSE file.
// See the AUTHORS file for names of contributors.
//
// Created by doori on 19. 7. 25.
//

#include "Log.h"

namespace doori{

template<typename T>
auto Log::log(T value) -> void
{
    _writeLog( value );
}

template<typename T, typename... Tlist>
auto Log::log(T arg, Tlist ... args) -> void
{
    log( arg );
    log( args... );
}

template<typename T>
auto Log::writeLog(LEVEL level
                    ,const char *fileName
                    ,const char *funcName
                    ,const int codeLine
                    , T arg) -> void
{
	if( mlevel > level )
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
    auto strFuncname = string(funcName);
    if(!mLogfile)
    {
        cout.imbue(std::locale("C.UTF-8"));

        cout << acTime << setfill(' ') ;
        cout << '|' << setfill(' ') << setw(25) << std::left  << (strFilename.size()>25?strFilename.substr(strFilename.size()-25,25):strFilename);
        cout << '|' << setfill(' ') << setw(25) << std::left  << (strFuncname.size()>25?strFuncname.substr(strFuncname.size()-25,25):strFuncname);
        cout << '|' << setfill(' ') << setw( 5) << std::right << codeLine ;
        cout << '|';
    }
    else
    {
        mLogfile.imbue(std::locale("C.UTF-8"));

        mLogfile << acTime << setfill(' ') ;
        mLogfile << '|' << setfill(' ') << setw(25) << std::left  << (strFilename.size()>25?strFilename.substr(strFilename.size()-25,25):strFilename);
        mLogfile << '|' << setfill(' ') << setw(25) << std::left  << (strFuncname.size()>25?strFuncname.substr(strFuncname.size()-25,25):strFuncname);
        mLogfile << '|' << setfill(' ') << setw( 5) << std::right << codeLine ;
        mLogfile << '|';
    }
    log( arg );
	_writeLineLog();
}


template<typename T, typename... Tlist>
auto Log::writeLog(LEVEL level
                    ,const char *fileName
                    ,const char *funcName
                    ,const int codeLine
                    ,T arg, Tlist ... args) -> void
{
    if(!mLogfile) {
        ;
    }
    else {
        if( mlevel > level )
            return;
    }

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
    auto strFuncname = string(funcName);
    if(!mLogfile)
    {
        cout.imbue(std::locale("C.UTF-8"));

        cout << acTime << setfill(' ') ;
        cout << '|' << setfill(' ') << setw(25) << std::left  << (strFilename.size()>25?strFilename.substr(strFilename.size()-25,25):strFilename);
        cout << '|' << setfill(' ') << setw(25) << std::left  << (strFuncname.size()>25?strFuncname.substr(strFuncname.size()-25,25):strFuncname);
        cout << '|' << setfill(' ') << setw( 5) << std::right << codeLine ;
        cout << '|';
    }
    else
    {
        mLogfile.imbue(std::locale("C.UTF-8"));

        mLogfile << acTime << setfill(' ') ;
        mLogfile << '|' << setfill(' ') << setw(25) << std::left  << (strFilename.size()>25?strFilename.substr(strFilename.size()-25,25):strFilename);
        mLogfile << '|' << setfill(' ') << setw(25) << std::left  << (strFuncname.size()>25?strFuncname.substr(strFuncname.size()-25,25):strFuncname);
        mLogfile << '|' << setfill(' ') << setw( 5) << std::right << codeLine ;
        mLogfile << '|';
    }
    log( arg );
    log( args... );
	_writeLineLog();
}

}//namespace doori
