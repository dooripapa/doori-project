// Copyright 2019 The doori-project Authors. All rights reserved.
// Use of this source code is governed by a GPL-3.0 license that can be found in the LICENSE file.
// See the AUTHORS file for names of contributors.
//
// Created by doori on 19. 7. 25.
//
#include "Regex.h"

using namespace std;

namespace doori{

auto
Regex::setObject(const string &object) noexcept -> const string&
{
    mObject = object;    
    return mObject;
}
auto
Regex::setObject(string&& object) noexcept -> const string&
{
    mObject = object;     
    return mObject;
}

auto
Regex::setRegex(const string& regexString) noexcept -> const regex&
{
    mRegex = regex(regexString);
    return mRegex;
}
auto
Regex::setRegex(string&& regexString) noexcept -> const regex&
{
    mRegex = regex(regexString);
    return mRegex;
}

auto
Regex::matching() -> bool
{
    return regex_match(mObject, mRegex);
}


auto
Regex::search() -> bool
{
    return regex_search(mObject, mRegex);
}


auto
Regex::bringPatterns() -> bool
{
    mBegin = sregex_token_iterator(mObject.begin(), mObject.end(), mRegex);
    return search();
}
auto
Regex::bringPatterns(std::vector<int> vec) -> bool
{
    mBegin = sregex_token_iterator(mObject.begin(), mObject.end(), mRegex, vec);
    return search();
}

auto
Regex::bringTokens() -> bool
{
    mBegin = sregex_token_iterator(mObject.begin(), mObject.end(), mRegex, -1);
    return search();
}


auto
Regex::replace(string swapString) -> bool
{
    bool bRet=false;
    string result = regex_replace(mObject, mRegex, swapString);
    if (mObject != result)
    {
        bRet=true;
        LOG(DEBUG, "Old: ", mObject);
        LOG(DEBUG, "-->");
        LOG(DEBUG, "New: ", result);
    }
    return bRet;
}

auto
Regex::cbegin()->const sregex_token_iterator&
{
    return mBegin;
}

auto
Regex::cend()->const sregex_token_iterator&
{
    return mEnd;
}

}//namespace doori
