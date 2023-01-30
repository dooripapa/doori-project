// Copyright 2019 The doori-project Authors. All rights reserved.
// Use of this source code is governed by a GPL-3.0 license that can be found in the LICENSE file.
// See the AUTHORS file for names of contributors.
//
// Created by doori on 19. 7. 25.
//
#pragma once
#include <sys/types.h>  
#include <sys/socket.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <string>

#include "Common/Log.h"
#include "DataStream/Stream.h"
#include "Addr.h"
#include "Endpoint.h"

namespace doori{

/**
 * From(endpoint) -----Connection-----> To(endpoint)
 * 형상화된 추상화된 객체로 이해할 것.
 */
class Connection
{
public:
    /**
     * From에 해당하는 Endpoint 객체를 리턴함
     * @return Endpoint : Endpoint객체 참조값
     */
    auto From() noexcept -> Endpoint&;
    /**
     * To에 해당하는 Endpoint 객체를 리턴함
     * @return Endpoint : Endpoint객체 참조값
     */
    auto To() noexcept -> Endpoint&;
    /**
     * From 초기화함
     * @param source_endpoint
     */
    auto setFrom(Endpoint source_endpoint) noexcept -> void;
    /**
     * To 초기화함
     * @param dest_endpoint
     */
    auto setTo(Endpoint dest_endpoint) noexcept -> void;
    /**
     * To에 연결 요청함
     * From 초기화하지 않았다면, 임의 From 생성 후 초기화함
     * To를 초기화지 않으면 error 리턴함
     * @attention binding 및 connect 실패시, 열려진 소켓을 이 함수에서 close처리한다.
     * @return
     */
    auto connectTo() -> int;
    /**
     * 연결을 대기함
     * From 초기화하지 않았다면, 에러리턴
     * @note 이함수가 호출하기전에 선행적으로 bind함수가 호출되어 있어야 한다.
     * @return  int : -1 에러, 성공하면 file descriptor
     */
    auto waitFor() -> int;
    /**
     * 데이터를 보냅니다.
     * @param contents : const Stream&
     * @return int : <0 에러값, 0 성공
     */
    auto send(const Stream& contents) -> int;
    /**
     * 데이터를 수신합니다.
     * @param recv_buffer : Stream&
     * @return int : <0 에러값, 0 성공
     */
    auto recv(Stream& recv_buffer) -> int;
    /**
     * 데이터를 보내고, 데이터를 오기를 기다립니다.
     * @param contents : request 데이터
     * @param response_contents : response 데이터
     * @return int : <0 에러값, 0 성공
     */
    auto reply(const Stream& contents, Stream& response_contents) -> int;
    /**
     * 지정된 소켓으로 데이터를 수신합니다.
     * @param connected_socket : socket
     * @param recv_buffer : 데이터 수신을 위한 버퍼, Stream& 객체
     * @return int : <0 에러값, 0 성공
     */
    static int recvFrom(int connected_socket, Stream& recv_buffer);
    /**
     * 지정된 소켓에 데이터를 보냅니다.
     * @param connected_socket : socket
     * @param contents : 보낼 데이터 버퍼, const Stream&
     * @return int : <0 에러값, 0 성공
     */
    static int sendTo(int connected_socket, const Stream& contents);
    /**
     * socket Listening 상태로 만듦
     * @return int : binding 된 File descriptor
     */
    auto onListening() -> int;
    /**
     * socket Accept
     * @note 이함수가 호출하기전에 선행적으로 bind함수가 호출되어 있어야 한다.
     * @return int : 0< 에러값, 연결요청이 성공되었다면 file descriptor
     */
    auto onAccepting() -> int;
    /**
     * 리소스 정리합니다.
     */
    auto release() -> void;
protected:
private:
    static int processBind(int& socketFd, const Addr& object);
    static int processSend(int connected_socketFd, const Stream& contents);
    static int processRecv(int connected_socketFd, Stream& recv_buffer);
    static int waitDataUtill( int iSocketfd, std::string& str, ssize_t requestedLen );
    int				mBindSock = -1;
	int				mConnSock = -1;
    Endpoint  mSource;
    Endpoint  mDest;
};


}//namespace doori
