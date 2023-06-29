// Copyright 2019 The doori-project Authors. All rights reserved.
// Use of this source code is governed by a GPL-3.0 license that can be found in the LICENSE file.
// See the AUTHORS file for names of contributors.
//
// Created by doori on 19. 7. 25.
//
#pragma once
#include <memory>
#include <string>
#include <unistd.h>
#include "Application.h"
#include "Getopt.h"

namespace doori::api::Process{

class Runner {
public:
    Runner();
    ~Runner();
    Runner(const Runner& rhs) = delete;
    Runner(Runner&& rhs);
    Runner& operator=(const Runner& rhs) = delete;
    Runner& operator=(Runner&& rhs);
    explicit Runner(Application&& app );
    auto run() -> void;
private:
    auto doAsBackground() noexcept -> int;
    auto doAsForeground() noexcept -> int;
    auto displayLogInfo() noexcept -> void;
    auto doRelease() -> void;
    std::unique_ptr< Application > mApp;
};

}//namespace doori
