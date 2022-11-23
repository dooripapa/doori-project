// Copyright 2019 The doori-project Authors. All rights reserved.
// Use of this source code is governed by a GPL-3.0 license that can be found in the LICENSE file.
// See the AUTHORS file for names of contributors.
//
// Created by doori on 19. 7. 25.
//
#include "Application.h"
#include "Log.h"

namespace doori{

const string Application::DEFAULT_LOG_NAME="a.out.log";
const string Application::DEFAULT_LOG_PATH="/tmp/";


Application::Application()  {
}

Application::~Application() {
}

auto Application::ProcessName() noexcept -> std::string {
    return std::string{"a.out"};
}

auto Application::Daemonize() noexcept -> bool {
    return false;
}
auto Application::LogFile() noexcept -> std::string {
    return DEFAULT_LOG_PATH+DEFAULT_LOG_NAME;
}
auto Application::LogLevel() noexcept -> Log::LEVEL{
    return Log::LEVEL::D;
}

auto Application::Terminate() noexcept -> int {
    return 0;
}

}//namespace doori
