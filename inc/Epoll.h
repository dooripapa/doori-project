// Copyright 2019 The doori-project Authors. All rights reserved.
// Use of this source code is governed by a GPL-3.0 license that can be found in the LICENSE file.
// See the AUTHORS file for names of contributors.
//
// Created by doori on 19. 7. 25.
//
#pragma once
#include <iostream>
#include <functional>
#include <tuple>
#include "Connection.h"
#include "Stream.h"
#include "EpollEvents.h"

namespace doori{
struct WATCHER{
    enum class TYPE{
        DESIGN,   //지정된 방식
        RECEIVER  //only 수신처리만
    };
};

class Epoll
{
public:
    explicit Epoll(const std::function<int(int, Stream&)>& delegation);
    Epoll(Epoll&& rhs);
    Epoll(const Epoll&) = delete;
    ~Epoll();
    auto init(Connection conn ) -> int;
    auto isListener( int socket_fd ) const -> bool;
    auto addWatcher( int socket_fd ) -> int;
    auto addUniqueWatcher(int socket_fd, WATCHER::TYPE type, const std::function<int(int, Stream&)>& delegation ) -> int;
    auto addWatcherAsConn() -> int;
    auto runningEventDelegateMethod() -> int;
    auto moveEventContainer(EpollEvents&& event_container ) noexcept -> void;
    auto waitForEvents(EpollEvents& event_container, const int& timeout ) -> int;
    auto executeTask( int socket_fd, const std::function<int(int socket_fd, Stream& stream)>& delegation ) -> int;
    auto executeTask( int socket_fd ) -> int;
    auto getListener() noexcept -> Connection&;
    auto sendWatchers( Stream stream ) noexcept -> bool;
    auto operator=(const Epoll& rhs) noexcept -> Epoll& = delete;
    auto operator=(Epoll&& rhs) noexcept -> Epoll& ;
protected:
private:
    std::unordered_map<int, pair<WATCHER::TYPE ,std::function<int(int,Stream&)>> > mConnectedSocketList;
    EpollEvents mEventContainer;
    Connection mEpollRootConnection;
    std::function<int(int,Stream&)> mEventDelegateMethod;
    int mEpoll_fd=0;
    int mListen_socket=0;
    auto moveFrom(Epoll&& rhs) noexcept -> void;
};

}//namespace doori
