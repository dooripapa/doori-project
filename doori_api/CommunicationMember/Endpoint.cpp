// Copyright 2019 The doori-project Authors. All rights reserved.
// Use of this source code is governed by a GPL-3.0 license that can be found in the LICENSE file.
// See the AUTHORS file for names of contributors.
//
// Created by doori on 19. 7. 25.
//
#include "Endpoint.h"
#include <cassert>

namespace doori::CommunicationMember {

    Endpoint::Endpoint() :mSet{READ_STATUS::FALSE}, mType{TYPE::NON} {
    }

    auto Endpoint::Type() noexcept -> TYPE {
        return mType;
    }

    auto Endpoint::CanRead() noexcept -> READ_STATUS {
        return mSet;
    }

    Endpoint::Endpoint(TYPE transportType, DataStream::Json topologiesInfo) {
        this->mSet = READ_STATUS::TRUE;
        this->mType = transportType;
        switch (transportType) {
            case TYPE::TCP:
                this->mTopoloiesInfo = topologiesInfo;
                break;
            case TYPE::UDP:
            case TYPE::SHM:
            case TYPE::MSQ:
            default:
                static_assert("Endpoint Type, UDP, SHM, MSQ 기능적 정의하지 못함.");
        }
    }

    const DataStream::Json &Endpoint::getTopoloiesInfo() const {
        return mTopoloiesInfo;
    }
}//namespace doori
