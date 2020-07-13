// Copyright 2019 The doori-project Authors. All rights reserved.
// Use of this source code is governed by a GPL-3.0 license that can be found in the LICENSE file.
// See the AUTHORS file for names of contributors.
//
// Created by doori on 19. 7. 25.
//
#pragma once
#include <iostream>
#include <string>
#include "Regex.h"

namespace doori{

class Getopt
{
public:
    /**
     * 기본생성자 호출 금지함
     */
    Getopt() = delete;
    /**
     * 복사생성자
     */
    Getopt(const Getopt&) = default;
    /**
     * 이동생성자
     */
    Getopt(Getopt&&) = delete;
    /**
     * 초기화
     * @param argc
     * @param argv
     */
    Getopt(int argc, char **argv);
    auto getOptValue(std::string opt, std::string &value) noexcept -> bool;
    auto getOptValue(std::string opt) noexcept -> std::string;
    auto Process() const noexcept -> std::string;
    auto operator=(const Getopt& rhs) noexcept -> Getopt&;
private:
    auto getValue(std::string opt) noexcept -> std::string;
    int  mArgc;
    char **mArgv;
    std::string mAllCommandLIne;
};


}//namespace doori

