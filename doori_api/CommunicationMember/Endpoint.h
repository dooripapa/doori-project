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
    /**
     * 기본생성자
     */
    Endpoint();
    /**
     * 초기화 함수
     * @param addr : Addr 객체
     */
    Endpoint(Addr addr);
    /**
     * 초기화 함수
     * @param addr : Addr 객체
     * @param transport_type : Endpoint::TYPE
     */
    Endpoint(Addr addr, TYPE transport_type);
    /**
     * Endpoint객체의 통신유형.(IPC type)
     * @return TYPE : Endpoint::TYPE
     */
	auto Transport() noexcept -> TYPE;
	/**
	 * Enpoint address 초기화
	 * @param addr : Addr 객체
	 */
    auto setAddress(Addr addr) noexcept ->  void;
    /**
     * Endpoint address 객체 리턴함
     * @return Addr리턴
     */
    auto Address() const noexcept -> const doori::Addr&;
    /**
     * Endpoint 초기화되었는지 상태값 리턴
     * @return Endpoint::STATUS
     */
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
