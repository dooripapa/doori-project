// Copyright 2019 The doori-project Authors. All rights reserved.
// Use of this source code is governed by a GPL-3.0 license that can be found in the LICENSE file.
// See the AUTHORS file for names of contributors.
//
// Created by doori on 19. 7. 25.
//
#include "Epoll.h"

using namespace std;

namespace doori{

    auto Epoll::init( Connection conn ) -> int
    {
        mEpoll_fd = epoll_create1(0);
        if( mEpoll_fd == -1 )
        {
            LOG(ERROR, "Create error" );
            return -1;
        }

        mEpollRootConnection = conn;
        mListen_socket = mEpollRootConnection.onListening();
        if (mListen_socket == -1)
        {
            LOG(ERROR, "fail to make listening socket" );
            return -1;
        }

        struct epoll_event ev{};
        ev.events = EPOLLIN;
        ev.data.fd = mListen_socket;
        if ( epoll_ctl(mEpoll_fd, EPOLL_CTL_ADD, mListen_socket, &ev) == -1)
        {
            LOG(ERROR, "fail to register listening socket to epoll list" );
            return -1;
        }
        return 0;
    }

    auto Epoll::isListener( int socket_fd ) const -> bool
    {
        return(mListen_socket==socket_fd);
    }

    auto Epoll::addWatcherAsConn() -> int
    {
        struct epoll_event ev{};
        int conn_sock;
        conn_sock = mEpollRootConnection.onAccepting();
        LOG(INFO, "Accepted FD[", conn_sock,"]");
        if (conn_sock == -1)
        {
            LOG(ERROR, "fail to accept socket.");
            return -1;
        }
        ev.events = EPOLLIN;
        ev.data.fd = conn_sock;
        if (epoll_ctl(mEpoll_fd, EPOLL_CTL_ADD, conn_sock, &ev) == -1)
        {
            LOG(ERROR, "epoll, fail to EPOLL_CTL_ADD");
            return -1;
        }

        mConnectedSocketList[conn_sock] = {WATCHER::TYPE::DESIGN, mEventDelegateMethod};

        return conn_sock;
    }

    auto Epoll::addWatcher( int socket_fd ) -> int
    {
        struct epoll_event ev{};
        ev.events = EPOLLIN;
        ev.data.fd = socket_fd;
        if ( epoll_ctl(mEpoll_fd, EPOLL_CTL_ADD, socket_fd, &ev) == -1)
        {
            LOG(ERROR, "Add error" );
            return -1;
        }
        mConnectedSocketList[socket_fd] = {WATCHER::TYPE::DESIGN,mEventDelegateMethod} ;
        return 0;
    }

    auto Epoll::addUniqueWatcher(int socket_fd, WATCHER::TYPE type, const function<int(int, Stream&)> &delegation) -> int
    {
        struct epoll_event ev{};
        ev.events = EPOLLIN;
        ev.data.fd = socket_fd;
        if ( epoll_ctl(mEpoll_fd, EPOLL_CTL_ADD, socket_fd, &ev) == -1)
        {
            LOG(ERROR, "Add error" );
            return -1;
        }
        mConnectedSocketList[socket_fd] = {type,delegation} ;
        return 0;
    }

    [[noreturn]] auto Epoll::runningEventDelegateMethod() ->int
    {
        LOG(INFO, "==================================");
        LOG(INFO, "running Method for Event Delegate!");
        while (true)
        {
            if (waitForEvents(mEventContainer, 1000*10))
                LOG(DEBUG, "Event!");
            else
                LOG(DEBUG, "timeout!");

            for(const auto& it : mEventContainer)
            {
                if ( isListener(it.getFd()))
                {
                    LOG(INFO, "Add watcher FD : [", it.getFd(), "]");
                    addWatcherAsConn();
                }
                else
                {
                    LOG(DEBUG, "Evented FD : [", it.getFd(), "]" );
                    executeTask( it.getFd() );
                }
            }
        }
    }

    auto Epoll::moveEventContainer(EpollEvents&& event_container ) noexcept ->void
    {
        mEventContainer = std::move(event_container);
    }

    auto Epoll::waitForEvents(EpollEvents& eventContainer, const int& timeout ) const -> int
    {
        char	errorStr[1024]={0};
        int Cnt = epoll_wait(mEpoll_fd, eventContainer.get(), eventContainer.getSize(), timeout);
        if ( Cnt == -1 )
        {
            LOG(ERROR, "Wait error[",errno,"] :" ,strerror_r(errno, errorStr, sizeof(errorStr)));
            throw new system_error();
        }
        eventContainer.setInvokedEventCnt(Cnt);
        return Cnt;
    }

    auto Epoll::executeTask( int socket_fd, const std::function<int(int socket_fd, Stream& stream)>& callback_function ) -> int
    {
        int iRet = 0;
        Stream buffer;
        iRet = Connection::recvFrom( socket_fd, buffer );
        if (iRet < 0 )
        {
            if (iRet == -2)
            {
                if ( (iRet=epoll_ctl(mEpoll_fd, EPOLL_CTL_DEL, socket_fd, nullptr)) == -1)
                    LOG(ERROR, "fail to deregister socket_fd to epoll instance" );
                mConnectedSocketList.erase(socket_fd);
                close(socket_fd);
            }
            else
                LOG(INFO, "Connection recvFrom error");
            return iRet;
        }
        iRet = callback_function(socket_fd, buffer );
        return iRet;
    }

    ///@return -2 Connection Lost
    auto Epoll::executeTask( int socket_fd ) -> int
    {
        int iRet = 0;
        Stream buffer;
        iRet = Connection::recvFrom( socket_fd, buffer );
        if (iRet < 0 )
        {
            if (iRet == -2)
            {
                if ( (iRet=epoll_ctl(mEpoll_fd, EPOLL_CTL_DEL, socket_fd, nullptr)) == -1)
                    LOG(ERROR, "fail to deregister socket_fd to epoll instance" );
                mConnectedSocketList.erase(socket_fd);
                close(socket_fd);
            }
            else
                LOG(INFO, "Connection recvFrom error");
            return iRet;
        }
        iRet = mConnectedSocketList[socket_fd].second.operator()(socket_fd, buffer);
        return iRet;
    }

    auto Epoll::getListener() noexcept -> Connection & {
        return mEpollRootConnection;
    }

    auto Epoll::sendWatchers(Stream stream) noexcept -> bool {
        auto bRet=true;
        for(const auto& i:mConnectedSocketList)
        {
            if( i.second.first != WATCHER::TYPE::RECEIVER )
                if( Connection::sendTo(i.first, stream) < 0 ) {
                    LOG(ERROR, "faile to send stream, FD : ", i.first);
                    bRet = false;
                }
        }
        return bRet;
    }

    Epoll::~Epoll() {
        LOG(INFO, "Epoll resource doRelease");
        for(const auto& i:mConnectedSocketList)
            close(i.first);
    }

    Epoll::Epoll(const std::function<int(int, Stream &)> &delegation) {
        mEventDelegateMethod = delegation;
    }

    auto Epoll::moveFrom(Epoll &&rhs) noexcept -> void {
        mConnectedSocketList = std::move(rhs.mConnectedSocketList);
        mEventContainer      = std::move(rhs.mEventContainer     );
        mEpollRootConnection = std::move(rhs.mEpollRootConnection);
        mEventDelegateMethod = std::move(rhs.mEventDelegateMethod);
        mEpoll_fd            = rhs.mEpoll_fd;rhs.mEpoll_fd=0;
        mListen_socket       = rhs.mListen_socket;rhs.mListen_socket=0;
    }

    auto Epoll::operator=(Epoll &&rhs) noexcept -> Epoll & {
        if(this==&rhs)
            return *this;

        moveFrom( std::move(rhs) );
        return *this;
    }

    Epoll::Epoll(Epoll && rhs) {
        moveFrom( std::move(rhs) );
    }

}//namespace doori
