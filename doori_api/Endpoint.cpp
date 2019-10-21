// Copyright 2019 The doori-project Authors. All rights reserved.
// Use of this source code is governed by a GPL-3.0 license that can be found in the LICENSE file.
// See the AUTHORS file for names of contributors.
//
// Created by doori on 19. 7. 25.
//
#include "Endpoint.h"

namespace doori{

Endpoint::Endpoint() {
    mSet = STATUS::FALSE;
}

Endpoint::Endpoint(Addr addr) {
    mAddr = addr;
    mType = TYPE::TCP;
    mSet = STATUS::TRUE;
}

Endpoint::Endpoint(Addr addr, TYPE transport_type) {
    mAddr = addr;
    mType = transport_type;
    mSet = STATUS::TRUE;
}

auto Endpoint::Transport() noexcept -> TYPE {
    return mType;
}

auto Endpoint::setAddress(Addr addr) noexcept ->  void{
    mSet = STATUS::TRUE;
    mAddr = addr;
}

auto Endpoint::Address() const noexcept -> const Addr& {
    return mAddr;
}

auto Endpoint::Set() noexcept -> STATUS {
    return mSet;
}

}//namespace doori
