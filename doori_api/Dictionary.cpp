// Copyright 2019 The doori-project Authors. All rights reserved.
// Use of this source code is governed by a GPL-3.0 license that can be found in the LICENSE file.
// See the AUTHORS file for names of contributors.
//
// Created by doori on 19. 7. 25.
//
#include "Dictionary.h"
#include "Log.h"
#include "Regex.h"

namespace doori{

Dictionary::Dictionary() {
}

Dictionary::Dictionary(const Dictionary &rhs) {
    copyFrom(rhs);
}

Dictionary::~Dictionary() {
}


Dictionary::Dictionary(Dictionary &&rhs) {
    if(this==&rhs)
        return;
    copyFrom(std::move(rhs));
}


///@brief Dictionary 포멧 형태의 파일을 읽는다.
auto Dictionary::open(const std::string &filepath) -> bool {

    fstream dictionaryFile;
    dictionaryFile.open(filepath, std::fstream::ios_base::in);
    if (!dictionaryFile)
    {
        LOG(ERROR, "fail to open Dictionary File");
        doori::EVENT(errno,-1,"fail to open");
        return false;
    }

    if (!parserDic(dictionaryFile))
    {
        LOG(ERROR, "Its wrong to format");
        doori::EVENT(0,-1,"Dictionary format is wrong");
        return false;
    }
    return true;
}

auto Dictionary::parserDic(fstream& dicFile) -> bool {
    Regex dooriRegex;
    Regex dooriRegexComment;  //주석

    string regexComment=R"~(^[[:space:]]*\#.*|^[[:space:]]*$)~";
    string regexStr=R"~(^[[:space:]]*\$\|([[:digit:]]+),[ILFDBSR]=(.+)\|\$.*)~";

    char acTemp[MAX_AS_A_LINE];
    while ( dicFile.getline(acTemp, sizeof(acTemp)) )
    {
        dooriRegexComment.setObject(acTemp);
        dooriRegexComment.setRegex(regexComment);

        auto ret = dooriRegexComment.matching();
        if (ret)
        {
            LOG(DEBUG, "comment ignore! :: ", acTemp);
            continue;
        }

        dooriRegex.setObject(acTemp);
        dooriRegex.setRegex(regexStr);
        if(!dooriRegex.matching())
        {
            LOG(ERROR, "Not matching! : ",string(acTemp));
            return false;
        }
        else
        {
            ret=dooriRegex.bringPatterns(vector<int>({1,2}));
            if (ret)
            {
                auto it=dooriRegex.cbegin();
                string id   =*it++;
                string value=*it++;
                mWords[ atoi(id.c_str()) ]=value;
            }
        }
    }
    return true;
}

auto Dictionary::Value(const TOKEN_INFO id) noexcept -> const string& {
    return mWords[id];
}


auto Dictionary::copyFrom(const Dictionary &rhs) noexcept -> void {
    mWords = rhs.mWords;
}

auto Dictionary::copyFrom(Dictionary &&rhs) noexcept -> void {
    mWords = std::move(rhs.mWords);
}

auto Dictionary::operator=(const Dictionary &rhs) -> Dictionary& {
    if(this==&rhs)
        return *this;

    copyFrom(rhs);
    return *this;
}

auto Dictionary::operator=(Dictionary &&rhs) -> Dictionary & {
    if(this==&rhs)
        return *this;

    copyFrom(std::move(rhs));
    return *this;
}

///@brief pretty print
auto Dictionary::pprint() noexcept -> void {
    LOG(INFO, "TNSD_INFO::VER:", mWords[TOKEN_INFO::VERSION] );
    LOG(INFO, "TNSD_INFO::TNSD_IP:", mWords[TOKEN_INFO::TNSD_IP] );
    LOG(INFO, "TNSD_INFO::TNSD_PORT:", mWords[TOKEN_INFO::TNSD_PORT] );
    LOG(INFO, "TNSD_INFO::BINDING_IP_FOR_TNSD:", mWords[TOKEN_INFO::BINDING_IP_FOR_TNSD] );
    LOG(INFO, "TNSD_INFO::BINDING_PORT_FOR_TNSD:", mWords[TOKEN_INFO::BINDING_PORT_FOR_TNSD] );
    LOG(INFO, "TNSD_INFO::BINDING_IP_FOR_MULTISESSION:", mWords[TOKEN_INFO::BINDING_IP_FOR_MULTISESSION] );
    LOG(INFO, "TNSD_INFO::BINDING_PORT_FOR_MULTISESSION:", mWords[TOKEN_INFO::BINDING_PORT_FOR_MULTISESSION] );
    LOG(INFO, "TNSD_INFO::LOG_NAME:", mWords[TOKEN_INFO::LOG_NAME] );
    LOG(INFO, "TNSD_INFO::LOG_PATH:", mWords[TOKEN_INFO::LOG_PATH] );
    LOG(INFO, "TNSD_INFO::LOG_LEVEL:", mWords[TOKEN_INFO::LOG_LEVEL] );
    LOG(INFO, "TNSD_INFO::MY_TOPIC:", mWords[TOKEN_INFO::MY_TOPIC] );
}

}//namespace doori
