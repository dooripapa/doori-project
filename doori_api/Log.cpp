// Copyright 2019 The doori-project Authors. All rights reserved.
// Use of this source code is governed by a GPL-3.0 license that can be found in the LICENSE file.
// See the AUTHORS file for names of contributors.
//
// Created by doori on 19. 7. 25.
//
#include "Log.h"

using namespace std;

namespace doori{

/* 프로그램이 시작할 때, 이 변수를 초기화를 한다.
   사전에 선언되어 있어야 한다. 런타임 시작전에(이 변수를 사용하는 시점에)
   이 변수가 초기화가 되었다는 보장을 할 수 없으므로, 사전에 선언해서
   , 런타임에 접근 가능하도록 한다.*/
Log Log::instance; 

auto Log::setLogEnv(const string& path, LEVEL level) -> void
{
	mlevel = level;

	try{
		mLogfile.open(path, ofstream::out|ofstream::app);
	}
	catch (...){
		cout << "fail to open " << path << endl;	
		exit(1);
	}
	return;
}

auto Log::logging() -> Log&
{
	return Log::instance;
}

auto Log::_writeLog( LEVEL level ) -> void
{
	switch( level )
	{
		case LEVEL::D:
			mLogfile << "D^ ";
			break;
		case LEVEL::I:
			mLogfile << "I^ ";
			break;
		case LEVEL::W:
			mLogfile << "W^ ";
			break;
		case LEVEL::E:
			mLogfile << "E^ ";
			break;
		case LEVEL::F:
			mLogfile << "F^ ";
			break;
	}
}

auto Log::_writeLineLog( ) -> void { mLogfile << endl; }

Log::Log()
{
}

Log::~Log()
{
	if( mLogfile.is_open() )
	{
		mLogfile.close();
	}
}

///@note 무조건 성공
///@param "DEBUG|INFO|WARN|FATAL|ERROR" 해당되며, 그외(잘못된) 무조건 LEVEL::D로 리턴
auto Log::convvertLevel(const std::string level) -> LEVEL {

    if( level == "DEBUG"){
        return LEVEL::D;
    }
    else if( level == "INFO"){
        return LEVEL::I;
    }
    else if( level == "WARN"){
        return LEVEL::W;
    }
    else if( level == "ERROR"){
        return LEVEL::E;
    }
    else if( level == "FATAL"){
        return LEVEL::F;
    }
    else
    {
        return LEVEL::D;
    }
}


}//namespace doori
