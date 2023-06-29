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
#include "Common/Error.h"
#include "Common/Log.h"
#include "Etc/Regex.h"

using namespace doori::api::Etc;

namespace doori::api::Data{

class Dictionary {

    public:

        Dictionary();
        ~Dictionary();
        Dictionary(const Dictionary& rhs);
        Dictionary(Dictionary&& rhs);
        auto operator=(const Dictionary& rhs) -> Dictionary&;
        auto operator=(Dictionary&& rhs) -> Dictionary&;

        /**
         * Dictionay 포멧형식의 파일로 초기화
         * @param filepath : Dictionary file 절대값 위치
         * @return bool
         */
        auto load(const std::string& filepath) -> bool;

        /**
         * Dictionary's default TOKEN 해당하는 값을 리턴
         * @param id : TOKEN
         * @return string TOKEN 해당하는 값을 문자열로 리턴
         */
        auto Value(uint Index) noexcept -> const std::string&;

        /**
         * record TOCKEN's info in log file
         */
        auto logging(string content, uint index) noexcept -> void;

    private:
        auto parserDic(std::fstream& dicFile) -> bool;
        auto copyFrom(Dictionary&& rhs) noexcept -> void;
        auto copyFrom(const Dictionary& rhs) noexcept -> void;
        enum {
            MAX_AS_A_LINE=10240
        };
        std::array< std::string , MAX_AS_A_LINE> mWords;
};

}
