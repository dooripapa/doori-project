// Copyright 2019 The doori-project Authors. All rights reserved.
// Use of this source code is governed by a GPL-3.0 license that can be found in the LICENSE file.
// See the AUTHORS file for names of contributors.
//
// Created by doori on 19. 7. 25.
//

#pragma once

#include <memory>
#include <string>
#include "Getopt.h"
#include "Dictionary.h"

namespace doori{

///@brief doori api를 통해서, 어플리케이션을 만들때는 이 클래스를 상속받아서  \
///       엄격하게 이루어져야 한다.
class Application {
public:
    Application();
    virtual ~Application();
    virtual auto operator()() noexcept -> int = 0;
    virtual auto clone() const noexcept -> std::unique_ptr<Application> = 0;
    virtual auto ProcessName() noexcept -> std::string = 0;
    virtual auto Daemonize() noexcept -> bool = 0;
    virtual auto LogFile() noexcept -> std::string = 0;
    virtual auto LogLevel() noexcept -> Log::LEVEL = 0;
    virtual auto Terminate() noexcept -> int = 0;
private:
    static const std::string DEFAULT_LOG_PATH;
    static const std::string DEFAULT_LOG_NAME;
};

}//namespace doori
