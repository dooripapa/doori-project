// Copyright 2019 The doori-project Authors. All rights reserved.
// Use of this source code is governed by a GPL-3.0 license that can be found in the LICENSE file.
// See the AUTHORS file for names of contributors.
//
// Created by doori on 19. 7. 25.
//
#include "Dictionary.h"

namespace doori::api::Data{

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
    auto Dictionary::load(const std::string &filepath) -> bool {

        fstream dictionaryFile;
        dictionaryFile.open(filepath, std::fstream::ios_base::in);
        if (!dictionaryFile)
        {
            LOG(ERROR, "fail to open Dictionary File");
            return false;
        }

        if (!parserDic(dictionaryFile))
        {
            LOG(ERROR, "Its wrong to format");
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

    auto Dictionary::Value(uint index) noexcept -> const string& {
        return mWords[index];
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

    auto Dictionary::logging(string content, uint index) noexcept -> void {
        LOG(INFO, content , " : [", mWords[index],"]" );
    }

}//namespace doori
