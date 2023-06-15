// Copyright 2019 The doori-project Authors. All rights reserved.
// Use of this source code is governed by a GPL-3.0 license that can be found in the LICENSE file.
// See the AUTHORS file for names of contributors.
//
// Created by doori on 19. 7. 25.
//
#include "Runner.h"

namespace doori::api::Process{

Runner::Runner(Application &&app) : mApp(app.clone()) {
}

///@todo ret 값에 따라, exception 발생시킬것.
auto Runner::run() -> void {
    auto ret = mApp->Daemonize() ? doAsBackground() : doAsForeground();
}

auto Runner::doAsBackground() noexcept -> int {
    displayLogInfo();

    //*****************************
    //Daemon()
    //*****************************
    daemon(0,0);

    //LOG path Setting
    LOG_CREATE(mApp->LogFile(),mApp->LogLevel());
    LOG(INFO, "***************************");
    LOG(INFO, "Background process! : ", mApp->ProcessName() );
    LOG(INFO, "***************************");
    mApp->operator()();
    return 0;
}

auto Runner::doAsForeground() noexcept -> int {
    displayLogInfo();

    //LOG path Setting
    LOG_CREATE(mApp->LogFile(),mApp->LogLevel());
    LOG(INFO, "***************************");
    LOG(INFO, "Foreground process! : ", mApp->ProcessName() );
    LOG(INFO, "***************************");
    mApp->operator()();
    return 0;
}

auto Runner::displayLogInfo() noexcept -> void {
    std::cout<< "Log file : "<< mApp->LogFile() <<endl;
    string logLevelStr;
    switch( mApp->LogLevel() )
    {
        case Common::Log::LEVEL::D:
            logLevelStr="DEBUG";
            break;
        case Common::Log::LEVEL::E:
            logLevelStr="ERROR";
            break;
        case Common::Log::LEVEL::F:
            logLevelStr="FAIL";
            break;
        case Common::Log::LEVEL::I:
            logLevelStr="INFO";
            break;
        case Common::Log::LEVEL::W:
            logLevelStr="WARN";
            break;
    }
    std::cout<< "Log level : "<< logLevelStr <<endl;
}

Runner::Runner() : mApp(nullptr){

}

///@todo mApp 복사하는 방법
Runner::Runner(const Runner &rhs) {
//    mApp = std::move(rhs.mApp);
}

auto Runner::doRelease() -> void {
    auto iRet = mApp->Terminate();
    LOG(INFO, "Application is terminated Exit code[", iRet, "]");
}

Runner::~Runner() {
    doRelease();
}

}//namespace doori
