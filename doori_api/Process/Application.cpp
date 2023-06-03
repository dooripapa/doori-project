// Copyright 2019 The doori-project Authors. All rights reserved.
// Use of this source code is governed by a GPL-3.0 license that can be found in the LICENSE file.
// See the AUTHORS file for names of contributors.
//
// Created by doori on 19. 7. 25.
//
#include "Application.h"
#include "Common/Log.h"

namespace doori::Process{

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
    return mDefaultLogPath + mDefaultLogName;
}
auto Application::LogLevel() noexcept -> Common::Log::LEVEL{
    return Common::Log::LEVEL::D;
}

auto Application::Terminate() noexcept -> int {
    return 0;
}

}//namespace doori
