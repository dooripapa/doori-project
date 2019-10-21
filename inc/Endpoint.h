// Copyright 2019 The doori-project Authors. All rights reserved.
// Use of this source code is governed by a GPL-3.0 license that can be found in the LICENSE file.
// See the AUTHORS file for names of contributors.
//
// Created by doori on 19. 7. 25.
//
#pragma	once
#include "Addr.h"

namespace doori{

class Endpoint
{
public:
    enum class TYPE:unsigned int{TCP, UDP, SHM, MSQ};
    enum STATUS:bool{TRUE=true, FALSE=false};
    Endpoint();
    Endpoint(doori::Addr);
    Endpoint(doori::Addr, TYPE);
	auto Transport() noexcept -> TYPE;
    auto setAddress(doori::Addr addr) noexcept ->  void;
    auto Address() const noexcept -> const doori::Addr&;
	auto Set() noexcept -> STATUS;
private:
    STATUS mSet;
	/* socket (TCP, UDP)*/
	doori::Addr mAddr;
	/* Shared memory */
	/* message queue */
	TYPE	mType;
};

}//namespace doori
