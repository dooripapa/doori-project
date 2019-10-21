// Copyright 2019 The doori-project Authors. All rights reserved.
// Use of this source code is governed by a GPL-3.0 license that can be found in the LICENSE file.
// See the AUTHORS file for names of contributors.
//
// Created by doori on 19. 7. 25.
//

#pragma once
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

namespace doori{

typedef struct Address{
    std::string Ip;
    std::string Port;
    bool operator==(const Address& rhs) const {
        return (Ip == rhs.Ip && Port == rhs.Port);
    };
}Address;

class Addr
{
public:
	Addr() = default;
	Addr(const Addr& rhs);
	Addr(Addr&& rhs);
	Addr(const std::string& ip, const std::string& port);
	explicit Addr(Address);
	~Addr() = default;
    auto setAddress(const Address& address) -> void;
	auto Domain() const -> int;
	auto getInetAddr() const-> const struct sockaddr_in&;
	auto Ip() const -> const std::string&;
	auto Port() const -> const std::string&;
	auto pprint() const -> const std::string;
	auto operator=(const Addr& rhs) -> Addr&;
	auto operator=(Addr&& rhs) -> Addr&;
	auto operator==(const Addr& rhs) const -> bool;
private:
	auto copyFrom(const Addr& rhs) -> void;
	struct sockaddr_in  mAddr;
	Address             mInetAddress;
};

}//namespace doori
