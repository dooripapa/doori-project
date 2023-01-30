// Copyright 2019 The doori-project Authors. All rights reserved.
// Use of this source code is governed by a GPL-3.0 license that can be found in the LICENSE file.
// See the AUTHORS file for names of contributors.
//
// Created by doori on 19. 7. 25.
//
#include "Addr.h"

using namespace std;

namespace doori::CommunicationMember{

auto Addr::getInetAddr() const -> const struct sockaddr_in&
{
	return mAddr;
}

auto Addr::Ip() const -> const string&
{
	return mInetAddress.Ip;
}

auto Addr::Port() const -> const string&
{
	return mInetAddress.Port;
}

auto Addr::setAddress(const Address& address) -> void {
    mInetAddress = address;
    uint16_t    u16Temp = 0;
    u16Temp = atoi( mInetAddress.Port.c_str() );

    mAddr.sin_family = AF_INET;
    mAddr.sin_port = htons(u16Temp);
    mAddr.sin_addr.s_addr = mInetAddress.Ip.c_str()?
                            inet_addr( mInetAddress.Ip.c_str() ):INADDR_ANY;
    return;
}

auto Addr::operator=(const Addr& rhs) -> Addr& {
	if( this ==&rhs  )
		return *this;

	copyFrom( rhs );
	return *this;
}

auto Addr::operator=(Addr&& rhs) -> Addr& {
	if( this ==&rhs  )
		return *this;

	copyFrom( rhs );
	return *this;
}

///@bug mAddr == rhs.mAddr 멤버 비교시, sin_zero에서 메모리부분이 다를수도 있음
auto Addr::operator==(const Addr& rhs) const -> bool {
	return ( mInetAddress == rhs.mInetAddress );
}

auto Addr::copyFrom(const Addr& rhs) -> void
{
    this->setAddress(rhs.mInetAddress);
}

Addr::Addr(const Addr &rhs) {
    copyFrom(rhs);
}

Addr::Addr(Addr &&rhs) {
    copyFrom(rhs);
}

auto Addr::pprint() const -> const std::string {
    return mInetAddress.Ip+":"+mInetAddress.Port;
}

Addr::Addr(Address address) : mInetAddress(address) {
}

Addr::Addr(const std::string &ip, const std::string &port) {
    Address addr;
    addr.Ip = ip;
    addr.Port = port;
    setAddress(addr);
}

}//namespace doori
