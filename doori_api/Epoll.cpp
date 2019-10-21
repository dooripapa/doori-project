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

///@brief 인자로 받은 소켓이. listener 소켓 확인
///@return true : listener 소켓 \
///        false : listner 소켓이 아님
auto Epoll::isListener( int socket_fd ) const -> bool
{
    return(mListen_socket==socket_fd);
}

///@brief connect 요청으로 소켓을 Wachter로 등록함
///@return generated Socket FD from Accept function.
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

///@brief 인자로 받은 소켓을 Watcher로 등록함
///@note Wachter로 등록된 소켓은 default callback 함수로 처리됨
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

///@brief 괸리되어지는 소켓으로 등록한다. 이 소켓에 대한 의존적인 함수를 지정됨
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

///@brief 이 함수에서 호출되면, 블록킹 모드로 전환되고, 자동으로 처리됨
///@note 예제소스 참고할것.
auto Epoll::runningEventDelegateMethod() ->int
{
    LOG(INFO, "==================================");
    LOG(INFO, "running Method for Event Delegate!");
    while (true)
    {
        if (waitForEvents(mEventContainer, 1000*10))
            LOG(DEBUG, "Event!");
        else
            LOG(DEBUG, "timeout!");

        for(auto it=mEventContainer.cbegin();it!=mEventContainer.cend();++it)
        {
            if ( isListener((*it).getFd()))
            {
                LOG(INFO, "Add watcher FD : [", (*it).getFd(), "]");
                addWatcherAsConn();
            }
            else
            {
                LOG(DEBUG, "Evented FD : [", (*it).getFd(), "]" );
                executeTask( (*it).getFd() );
            }
        }
    }
}

auto Epoll::moveEventContainer(EpollEvents&& event_container ) noexcept ->void
{
    mEventContainer = std::move(event_container);
}

auto Epoll::waitForEvents(EpollEvents& eventContainer, const int& timeout ) -> int
{
    char	errorStr[1024]={0};
    int Cnt = epoll_wait(mEpoll_fd, eventContainer.getInstance(), eventContainer.getSize(), timeout);
    if ( Cnt == -1 )
    {
        LOG(ERROR, "Wait error[",errno,"] :" ,strerror_r(errno, errorStr, sizeof(errorStr)));
        throw new system_error();
    }
	eventContainer.setInvokedEventCnt(Cnt);
    return Cnt;
}

///@brief 첫번재 인자값 소켓으로 부터 데이터 수신되면, 해당데이트를 두번째 아규먼트에 등록된 콜백함수에 처리를 위임한다.
///@return -2 Connection Lost
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

///@brief epoll로 관리 되고 있는 모든 FD에게 데이터를 보낸다.
///@see 단, WATCHER type이, RECEIVER에 송신하지 않는다.
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

///@brief default callback 함수로 인자로 한, 초기화.
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
