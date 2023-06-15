// Copyright 2019 The doori-project Authors. All rights reserved.
// Use of this source code is governed by a GPL-3.0 license that can be found in the LICENSE file.
// See the AUTHORS file for names of contributors.
//
// Created by doori on 19. 7. 25.
//
namespace doori::api::Data{
template<int N>
auto DataSegment::set(int fid, char const(&value)[N]) -> void
{
    deleteDooridata();
    mFid = fid;
    mType = TYPE::S;
    if (N>1)
        mStr = std::string(value);
    else
        mStr = std::string("NaN"); //null
}

template<int N>
DataSegment::DataSegment(int fid, char const(&value)[N])
{
    mFid = fid;
    mType = TYPE::S;
    if (N>1)
        mStr = std::string(value);
    else
        mStr = std::string("NaN"); //null
};

}//namespace doori
