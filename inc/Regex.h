// Copyright 2019 The doori-project Authors. All rights reserved.
// Use of this source code is governed by a GPL-3.0 license that can be found in the LICENSE file.
// See the AUTHORS file for names of contributors.
//
// Created by doori on 19. 7. 25.
//
#pragma	once
#include "Log.h"
#include <regex>

namespace doori{

class Regex
{
public:
    Regex(): mRegex(""), mObject(""){};
    virtual ~Regex(){};

    auto setObject(std::string&& object) noexcept -> const string&;
    auto setObject(const std::string& object) noexcept -> const string&;
    auto setRegex(const std::string& regexStr) noexcept -> const regex&;
    auto setRegex(std::string&& regexStr) noexcept -> const regex&;

    auto matching() -> bool;
    auto search() -> bool;
    auto bringPatterns() -> bool;
    auto bringPatterns(std::vector<int> vec) -> bool;
    auto bringTokens() -> bool;

    auto cbegin() -> const sregex_token_iterator&;
    auto cend() -> const sregex_token_iterator&;
    auto replace( std::string swapString ) -> bool;
private:
    const sregex_token_iterator mEnd;
    sregex_token_iterator mBegin;
    regex mRegex;
    string mObject;
};

}
