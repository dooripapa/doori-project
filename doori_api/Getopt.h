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
     * 기본생성자 금지
     */
    Getopt() = delete;
    /**
     * 복사생성자
     */
    Getopt(const Getopt&) = default;
    /**
     * 이동생성자 금지
     */
    Getopt(Getopt&&) = delete;
    /**
     * 초기화
     * @param argc : main(int argc, char **argv)의 첫번째 아규먼트값
     * @param argv : main(int argc, char **argv)의 두번째 아규먼트값
     */
    Getopt(int argc, char **argv);
    /**
     * optional 문자열을 입력 받으면, optional의 문자열의 매칭문자열을 value에 저장해서 돌려준다
     * @param opt : ex) --log, --file, -f (first char '-')
     * @param value: if there is value, put in &value, or not, it is initialized ""
     * @return bool : true 성공, false 실패
     */
    auto getOptValue(std::string opt, std::string &value) noexcept -> bool;
    /**
     * optional 문자열을 입력 받으면, optional의 문자열의 매칭문자을 리턴함
     * @param opt : ex) --log foo.log
     * @return std::string :  optional의 문자열의 매칭문자을 값 리턴(ex) foo.log)
     */
    auto getOptValue(std::string opt) noexcept -> std::string;
    /**
     * 프로그램명을 리턴함
     * @return 프로그램명을 리턴
     */
    auto Process() const noexcept -> std::string;
    /**
     * 대입복사연산자
     * @param rhs : const Getopt&
     * @return 자기자신의 참조값
     */
    auto operator=(const Getopt& rhs) noexcept -> Getopt&;
private:
    auto getValue(std::string opt) noexcept -> std::string;
    int  mArgc;
    char **mArgv;
    std::string mAllCommandLIne;
};


}//namespace doori

