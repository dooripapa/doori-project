// Copyright 2019 The doori-project Authors. All rights reserved.
// Use of this source code is governed by a GPL-3.0 license that can be found in the LICENSE file.
// See the AUTHORS file for names of contributors.
//
// Created by doori on 19. 7. 25.
//
#pragma once

#include <vector>
#include <iostream>
#include <iomanip>
#include <string>
#include "Log.h"

namespace doori{

class Topic
{
public:
    Topic();
    Topic(const Topic&);
    Topic(Topic&&);
    Topic(initializer_list<string>);
    auto set(initializer_list<string>) -> bool;
    auto set(const string&) -> bool;
    auto operator=(const Topic&) noexcept -> Topic&;
    auto operator=(Topic&&) noexcept -> Topic&;
    auto operator==(const Topic&) const noexcept -> bool;
    auto getTopicName() const noexcept -> std::string;
    auto getTopicName(unsigned int depth) const noexcept -> std::string;
    auto getDepthSize() const noexcept -> uint;
private:
    auto copyFrom(const Topic& rhs) noexcept -> void;
    std::vector<std::string> mTopics;
    std::string mTopicSum;
};

}
