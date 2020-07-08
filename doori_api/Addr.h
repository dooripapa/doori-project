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
    /**
     * Address 객체가 같은지 비교 합니다.
     * @param rhs : const Address&
     * @return 객체가 같으면 true, 그렇지 않으면 false
     */
    bool operator==(const Address& rhs) const {
        return (Ip == rhs.Ip && Port == rhs.Port);
    };
}Address;

class Addr
{
public:
	Addr() = default;
	/**
	 * 복사생성자
	 * @param rhs : const Addr&
	 */
	Addr(const Addr& rhs);
	/**
	 * 이동생성자
	 * @param rhs : Addr&&
	 */
	Addr(Addr&& rhs);
	/**
	 * 객체 초기화를 합니다.
	 * @param ip : const std::string&
	 * @param port : const std::string&
	 */
	Addr(const std::string& ip, const std::string& port);
	/**
	 * Address객체를 이용한 엄격한 객체 초기화
	 */
	explicit Addr(Address);
	~Addr() = default;
	/**
	 * Address객체를 이용한 Addr객체 셋팅
	 * @param address : const Address&
	 */
    auto setAddress(const Address& address) -> void;
    /**
     * Addr 객체 도메인을 리턴함.
     * socket 일 경우, sin_family 형
     * @return int
     */
	auto Domain() const -> int;
	/**
	 * Addr 객체의 속성을 리턴함
	 * @return struct sockaddr_in
	 */
	auto getInetAddr() const-> const struct sockaddr_in&;
	/**
	 * Addr의 Ip 속성을 리턴함
	 * @return std::string
	 */
	auto Ip() const -> const std::string&;
	/**
	 * Addr의 Port 속성을 리턴함
	 * @return std::string
	 */
	auto Port() const -> const std::string&;
	/**
	 * Addr의 객체를 Pretty print 함
	 * @return std::string
	 */
	auto pprint() const -> const std::string;
	/**
	 * 복사 대입연산자
	 * @param rhs : const Addr&
	 * @return Addr& : Addr객체의 참조값
	 */
	auto operator=(const Addr& rhs) -> Addr&;
	/**
	 * 이동 대입연산자
	 * @param rhs : Addr&&
	 * @return Addr& : Addr객체의 참조값
	 */
	auto operator=(Addr&& rhs) -> Addr&;
	/**
	 * 객체 비교연산자
	 * @param rhs : const Addr&
	 * @return bool
	 */
	auto operator==(const Addr& rhs) const -> bool;
private:
	auto copyFrom(const Addr& rhs) -> void;
	struct sockaddr_in  mAddr;
	Address             mInetAddress;
};

}//namespace doori
