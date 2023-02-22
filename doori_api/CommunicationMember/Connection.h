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
#include "CommunicationMember/Endpoint.h"
#include "CommunicationMember/IConnection.h"

namespace doori::CommunicationMember {

/**
 * From(endpoint) -----Connection-----> To(endpoint)
 * 형상화된 추상화된 객체로 이해할 것.
 */
    class Connection {
    public:
        /**
         * From에 해당하는 Endpoint 객체를 리턴함
         * @return Endpoint : Endpoint객체 참조값
         */
        auto From() noexcept -> Endpoint &;

        /**
         * To에 해당하는 Endpoint 객체를 리턴함
         * @return Endpoint : Endpoint객체 참조값
         */
        auto To() noexcept -> Endpoint &;

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
        auto connectTo(struct sockaddr_in destAddrIn, struct sockaddr_in sourceAddrIn) -> int;

        /**
         * struct sockaddr_in 셋팅된 값으로, Bind, Listen, Accept절차를 거쳐 연결을 대기함
         * @return  int : -1 에러, 성공하면 file descriptor 리턴함
         */
        auto waitFor(struct sockaddr_in sockaddrIn) -> int;

        /**
         * 연결이 된 FD통해, 데이터를 보냅니다.
         * @param fd: 커넥션이 완료된 File Descriptor
         * @param contents : const Stream&
         * @return int : <0 에러값, 0 성공
         */
        auto send(int fd, const Stream &contents) -> int;

        /**
         * 연결이 된 FD로 통해서 데이터를 수신한다.
         * @param recv_buffer : Stream&
         * @return int : <0 에러값, 0 성공
         */
        auto recv(int fd, Stream &recv_buffer) -> int;

        /**
         * 연결이 된 FD로 통해서 데이터를 보내고, 데이터를 오기를 기다립니다.
         * @param contents : request 데이터
         * @param response_contents : response 데이터
         * @return int : <0 에러값, 0 성공
         */
        auto reply(int fd, const Stream &contents, Stream &response_contents) -> int;

        /**
         * 지정된 소켓으로 데이터를 수신합니다.
         * @param connected_socket : socket
         * @param recv_buffer : 데이터 수신을 위한 버퍼, Stream& 객체
         * @return int : <0 에러값, 0 성공
         */
        static int recvFrom(int connected_socket, Stream &recv_buffer);

        /**
         * 지정된 소켓에 데이터를 보냅니다.
         * @param connected_socket : socket
         * @param contents : 보낼 데이터 버퍼, const Stream&
         * @return int : <0 에러값, 0 성공
         */
        static int sendTo(int connected_socket, const Stream &contents);

        /**
         * socket Listening 상태로 만듦
         * @return int : binding 된 File descriptor
         */
        auto onListening(struct sockaddr_in sockaddrIn) -> int;

        /**
         * 바인딩(Bind)가 완료된 FD를 이용하여 accept() 함수를 호출한다.
         * @note 이함수가 호출하기전에 선행적으로 bind함수가 호출되어 있어야 한다.
         * @return int : 0< 에러값, 연결요청이 성공되었다면 file descriptor
         */
        auto onAccepting(int fd, struct sockaddr_in sockAddrIn) -> int;

    private:
        static int processBind(int &socketFd, const Addr &object);

        static int processSend(int connected_socketFd, const Stream &contents);

        /**
         * struct sockaddr_in 구조체에 셋팅된 값으로 바인딩 file descriptor 리턴한다.
         * @note 바인딩된 소켓은 다음과 같은 특성이 있다
         * 도메인 : AF_INET
         * 도메인타입 : SOCK_STREAM
         * 바인딩소켓은 SO_REUSEPORT, SO_REUSEADDR 으로 재사용으로 설정함
         * @param addrIn a
         * @return -1 : fail, 성공이면 0보드 큰값 소켓 FD를 리턴함.
         */
        static int Bind(struct sockaddr_in &addrIn);

        /**
         * 소켓의 연결이 완료된 fd에 데이터를 보낸다.
         * @param fd 커넥션이 맺어진 상태의 소켓
         * @param data data
         * @param dataLen data 사이즈
         * @return
         */
        static int Send(int fd, byte data[], ssize_t dataLen);

        static int processRecv(int connected_socketFd, Stream &recv_buffer);

        /**
        *@brief 소켓으로 해당길이까지 데이터 수신
        *@details
        *@date
        *@param
        *@return -1 : unkonwn errorSt
                 -2 : socket closed
        */
        static int waitDataUtill(int iSocketfd, std::string &str, ssize_t requestedLen);

        Endpoint mSource;
        Endpoint mDest;
    };


}//namespace doori
