// Copyright 2019 The doori-project Authors. All rights reserved.
// Use of this source code is governed by a GPL-3.0 license that can be found in the LICENSE file.
// See the AUTHORS file for names of contributors.
//
// Created by doori on 19. 7. 25.
//
#include "Getopt.h"

using namespace std;

namespace doori{

Getopt::Getopt(int argc, char**argv)
{
    mArgc = argc;
    mArgv = argv;
    for(int i=0;i<mArgc;++i)
    {
        mAllCommandLIne.append(string(mArgv[i]));
        if ( i+1 < mArgc ){
            mAllCommandLIne.append(string(" "));
        }
    }
    LOG(INFO, "Command Line[", mAllCommandLIne, "]");
}

auto Getopt::getOptValue(std::string opt, std::string &value) noexcept -> bool
{
    value=getValue(opt);
    return !value.empty()?true:false;
}

auto Getopt::getOptValue(std::string opt) noexcept -> std::string {
    return getValue(opt);
}

auto Getopt::getValue(std::string opt) noexcept -> std::string {
    Regex dooriRegex;
    string value;
    dooriRegex.setObject(mAllCommandLIne);

    dooriRegex.setRegex(R"~([[:blank:]]+)~");

    auto resultRex= dooriRegex.bringTokens();

    if (!resultRex)
        return "";

    for(auto it=dooriRegex.cbegin();it!=dooriRegex.cend();it++)
    {
        if (opt==*it)
        {
            if (it++!=dooriRegex.cend())
            {
                if ('-'!=static_cast<string>(*it)[0])
                    value=(*it);
                else
                    value="";

            }
        }
    }
    return value;
}

auto Getopt::operator=(const Getopt &rhs) noexcept -> Getopt & {
    if( this ==&rhs  )
        return *this;

    mArgc = rhs.mArgc;
    mArgv = rhs.mArgv;
    mAllCommandLIne = rhs.mAllCommandLIne;
    return *this;
}

auto Getopt::Process() const noexcept -> std::string {
    return string(mArgv[0]);
}


}//namespace doori
