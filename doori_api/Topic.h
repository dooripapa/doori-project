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
    /**
     * depth는 구애받지 않는 string 정함
     * @note 각각의 단어안에 공백문자열 또는 '.'가 존재하면, false처리
     * @example set( {"1","2","3"} )
     * @todo invalid_argument를 invoke 해야하는지 검토해야함
     * @return true 정상, false 실패
     */
    auto set(initializer_list<string>) -> bool;
    /**
     * 쉼표로 구분되어지는 문자열을 파싱하여 doori_topic를 구성함
     * @example "1.2.3"
     * @return true 정상, false 실패
     */
    auto set(const string&) -> bool;
    auto operator=(const Topic&) noexcept -> Topic&;
    auto operator=(Topic&&) noexcept -> Topic&;
    auto operator==(const Topic&) const noexcept -> bool;
    /**
     * @return 콤마형식의 문자열 리스트를 출력함
     */
    auto getTopicName() const noexcept -> std::string;
    /**
     * @note index는 0부터 시작함
     * @return index값 overflow 일경우, "" null 리턴
     * @todo index값 overflow할 경우 exception처리에 대해서 고민할것.
     */
    auto getTopicName(unsigned int depth) const noexcept -> std::string;
    /**
     * topic 사이즈의 depth를 리턴함
     * @return 0 이상값
     */
    auto getDepthSize() const noexcept -> uint;
private:
    auto copyFrom(const Topic& rhs) noexcept -> void;
    std::vector<std::string> mTopics;
    std::string mTopicSum;
};

}
