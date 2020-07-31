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
    /**
     * Dictionary's default TOKEN
     */
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
    /**
     * 기본생성자
     */
    Dictionary();
    /**
     * 기본소멸자
     */
    ~Dictionary();
    /**
     * 복사생성자
     * @param rhs
     */
    Dictionary(const Dictionary& rhs);
    /**
     * 이동생성자
     * @param rhs
     */
    Dictionary(Dictionary&& rhs);
    /**
     * 복사대입연산자
     * @param rhs
     * @return 자신의 참조값
     */
    auto operator=(const Dictionary& rhs) -> Dictionary&;
    /**
     * 이동대입연산자
     * @param rhs
     * @return
     */
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
    auto Value(const TOKEN_INFO id) noexcept -> const std::string&;
    /**
     * record TOCKEN's info in log file
     */
    auto logging() noexcept -> void;
private:
    auto parserDic(std::fstream& dicFile) -> bool;
    auto copyFrom(Dictionary&& rhs) noexcept -> void;
    auto copyFrom(const Dictionary& rhs) noexcept -> void;
    static const int MAX_AS_A_LINE=10240;
    std::array< std::string , TOKEN_INFO::END> mWords;
};

}
