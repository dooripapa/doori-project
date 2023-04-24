// Copyright 2019 The doori-project Authors. All rights reserved.
// Use of this source code is governed by a GPL-3.0 license that can be found in the LICENSE file.
// See the AUTHORS file for names of contributors.
//
// Created by doori on 19. 7. 25.
//
#include "Epoll.h"

using namespace std;

namespace doori::CommunicationMember{

    Epoll::Epoll(const std::function<int(int)>& foreHeadDelegation, const std::function<int(int, string&)>& rearwardDelegation ) {
        mEventDelegateMethod = rearwardDelegation;
        mForeheadSectionProcedure = foreHeadDelegation;
    }

    auto Epoll::Init(int fd) -> int
    {
        mEpoll_fd = epoll_create1(0);
        if( mEpoll_fd == -1 )
        {
            LOG(ERROR, "Create error" );
            return -1;
        }

        mListen_socket = doori::CommunicationMember::TcpApi::Listen(fd, 10);
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

    auto Epoll::EqualListner( int socket_fd ) const -> bool
    {
        return(mListen_socket==socket_fd);
    }

    auto Epoll::AddWatcherAsConn() -> int
    {
        struct epoll_event ev{};
        int conn_sock;
        conn_sock = CommunicationMember::TcpApi::Accept(mListen_socket);
        LOG(INFO, "Accepted FD[", conn_sock,"]");
        if (conn_sock == -1)
        {
            LOG(ERROR, "fail to accept socket.");
            return -1;
        }
        ev.events = EPOLLIN;

        // Timeout 설정함
        conn_sock = CommunicationMember::TcpApi::SetTimeoutOpt(conn_sock, 10);
        ev.data.fd = conn_sock;
        if (epoll_ctl(mEpoll_fd, EPOLL_CTL_ADD, conn_sock, &ev) == -1)
        {
            LOG(ERROR, "epoll, fail to EPOLL_CTL_ADD");
            return -1;
        }

        mConnectedSocketList[conn_sock] = {WATCHER::TYPE::DESIGN, mEventDelegateMethod};

        return conn_sock;
    }

    auto Epoll::AddWatcher( int socket_fd ) -> int
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

    auto Epoll::AddUniqueWatcher(int socket_fd, WATCHER::TYPE type, const std::function<int(int, string &)> &delegation) -> int
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

    [[noreturn]] auto Epoll::RunningEventDelegateMethod() ->int
    {
        LOG(INFO, "==================================");
        LOG(INFO, "running Method for Event Delegate!");
        while (true)
        {
            if (WaitForEvents(mEventContainer, 1000*10))
                LOG(DEBUG, "Event!");
            else
                LOG(DEBUG, "timeout!");

            for(const auto& it : mEventContainer)
            {
                if (EqualListner(it.getFd()) )
                {
                    LOG(INFO, "Add watcher FD : [", it.getFd(), "]");
                    AddWatcherAsConn();
                }
                else
                {
                    LOG(DEBUG, "Evented FD : [", it.getFd(), "]" );
                    ExecuteTask( it.getFd() );
                }
            }
        }
    }

    auto Epoll::MoveEventContainer(EpollEvents&& event_container ) noexcept ->void
    {
        mEventContainer = std::move(event_container);
    }

    auto Epoll::WaitForEvents(EpollEvents& event_container, const int& timeout ) const -> int
    {
        char	errorStr[1024]={0};
        int Cnt = epoll_wait(mEpoll_fd, event_container.get(), event_container.getSize(), timeout);
        if ( Cnt == -1 )
        {
            LOG(ERROR, "Wait error[",errno,"] :" ,strerror_r(errno, errorStr, sizeof(errorStr)));
            throw new system_error();
        }
        event_container.setInvokedEventCnt(Cnt);
        return Cnt;
    }

    auto Epoll::ExecuteTask(int socket_fd, const std::function<int(int socket_fd, string& stream)>& delegation ) -> int
    {
        int iRet = 0;
        int iReadLen = 0;

        string buffer;
        iReadLen = mForeheadSectionProcedure(socket_fd);
        if(iReadLen ==  -1)
        {
            LOG(ERROR, "Header Section procedure error");
            return iReadLen;
        }
        LOG(DEBUG, "Body Section Data Size[", iReadLen, "]");

        std::unique_ptr<char[]> dataContainer = std::make_unique<char []>(iReadLen+1);  // 1 is null size
        memset(dataContainer.get(), 0x00, iReadLen + 1);

        errno = 0;
        iRet = CommunicationMember::TcpApi::Recv(socket_fd, dataContainer.get(), iReadLen);
        if( iRet < 0 ) {
            LOG(ERROR, "TcpApi::Recv error, cause[", ::strerror(errno), "]");
            return -1;
        }

        if( iRet == 0 ) {
            LOG(ERROR, "TcpApi::Recv error, closed by opposite side");

            if ( (iRet=epoll_ctl(mEpoll_fd, EPOLL_CTL_DEL, socket_fd, nullptr)) == -1)
                LOG(ERROR, "fail to deregister socket_fd to epoll instance" );

            mConnectedSocketList.erase(socket_fd);
            close(socket_fd);

            return -2;
        }
        else {
            buffer = dataContainer.get();
            iRet = delegation(socket_fd, buffer );
        }

        return iRet;
    }

    ///@return -2 Connection Lost
    auto Epoll::ExecuteTask(int socket_fd) -> int
    {
        int iRet = 0;
        int iReadLen = 0;

        string buffer;
        iReadLen = mForeheadSectionProcedure(socket_fd);
        if(iReadLen ==  -1)
        {
            LOG(ERROR, "Header Section procedure error");
            return iReadLen;
        }
        LOG(DEBUG, "Body Section Data Size[", iReadLen, "]");

        std::unique_ptr<char[]> dataContainer = std::make_unique<char []>(iReadLen+1);  // 1 is null size
        memset(dataContainer.get(), 0x00, iReadLen + 1);

        errno = 0;
        iRet = CommunicationMember::TcpApi::Recv(socket_fd, dataContainer.get(), iReadLen);
        if( iRet < 0 ) {
            LOG(ERROR, "TcpApi::Recv error, cause[", ::strerror(errno), "]");
            return -1;
        }

        if( iRet == 0 ) {
            LOG(ERROR, "TcpApi::Recv error, closed by opposite side");

            if ( (iRet=epoll_ctl(mEpoll_fd, EPOLL_CTL_DEL, socket_fd, nullptr)) == -1)
                LOG(ERROR, "fail to deregister socket_fd to epoll instance" );

            mConnectedSocketList.erase(socket_fd);
            close(socket_fd);

            return -2;
        }
        else {

            buffer = dataContainer.get();

            iRet = mConnectedSocketList[socket_fd].second.operator()(socket_fd, buffer);
        }
        return iRet;
    }

    auto Epoll::GetListener() noexcept -> int {
        return mEpollRootConnection;
    }

    auto Epoll::SendWatchers(string stream) noexcept -> bool {
        auto bRet=true;
        for(const auto& i:mConnectedSocketList)
        {
            if( i.second.first != WATCHER::TYPE::RECEIVER )
                if( doori::CommunicationMember::TcpApi::Send(i.first,  stream.c_str(), stream.size() ) < 0 ) {
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

    auto Epoll::moveFrom(Epoll &&rhs) noexcept -> void {
        mConnectedSocketList = std::move(rhs.mConnectedSocketList);
        mEventContainer      = std::move(rhs.mEventContainer     );
        mEpollRootConnection = std::move(rhs.mEpollRootConnection);
        mForeheadSectionProcedure = std::move(rhs.mForeheadSectionProcedure);
        mEventDelegateMethod = std::move(rhs.mEventDelegateMethod);
        mEpoll_fd            = rhs.mEpoll_fd;
        rhs.mEpoll_fd        = 0;
        mListen_socket       = rhs.mListen_socket;
        rhs.mListen_socket   = 0;
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
