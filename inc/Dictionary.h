// Copyright 2019 The doori-project Authors. All rights reserved.
// Use of this source code is governed by a GPL-3.0 license that can be found in the LICENSE file.
// See the AUTHORS file for names of contributors.
//
// Created by doori on 19. 7. 25.
//
#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include "Error.h"

namespace doori{

class Dictionary {
public:
    enum TOKEN_INFO{
        VERSION,
        TNSD_IP,
        TNSD_PORT,
        BINDING_IP_FOR_TNSD,
        BINDING_PORT_FOR_TNSD,
        BINDING_IP_FOR_MULTISESSION,
        BINDING_PORT_FOR_MULTISESSION,
        LOG_NAME,
        LOG_PATH,
        LOG_LEVEL,
        MY_TOPIC,
        END
    };
public:
    Dictionary();
    ~Dictionary();
    Dictionary(const Dictionary& rhs);
    Dictionary(Dictionary&& rhs);
    auto operator=(const Dictionary& rhs) -> Dictionary&;
    auto operator=(Dictionary&& rhs) -> Dictionary&;
    auto open(const std::string& filepath) -> bool;
    auto Value(const TOKEN_INFO id) noexcept -> const std::string&;
    auto pprint() noexcept -> void;
private:
    auto parserDic(std::fstream& dicFile) -> bool;
    auto copyFrom(Dictionary&& rhs) noexcept -> void;
    auto copyFrom(const Dictionary& rhs) noexcept -> void;
    static const int MAX_AS_A_LINE=10240;
    std::array< std::string , TOKEN_INFO::END> mWords;
};

}
