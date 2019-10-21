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

#include "Log.h"
#include "Stream.h"
#include "Addr.h"
#include "Endpoint.h"

namespace doori{

class Connection
{
public:
    auto From() noexcept -> Endpoint&;
    auto To() noexcept -> Endpoint&;
    auto setFrom(Endpoint source_endpoint) noexcept -> void;
    auto setTo(Endpoint dest_endpoint) noexcept -> void;
    auto connectTo() -> int;
    auto waitFor() -> int;
    auto send(const Stream& contents) -> int;
    auto recv(Stream& recv_buffer) -> int;
    auto reply(const Stream& contents, Stream& response_contents) -> int;
    static int recvFrom(int connected_socket, Stream& recv_buffer);
    static int sendTo(int connected_socket, const Stream& contents);
    auto onListening() -> int;
    auto onAccepting() -> int;
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
