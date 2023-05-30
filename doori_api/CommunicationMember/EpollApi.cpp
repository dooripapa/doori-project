//
// Created by jaeseong on 23. 5. 23.
//

#include "EpollApi.h"
#include "TcpApi.h"

namespace doori::CommunicationMember {

    EpollApi::EpollApi(Socket &socket) : Common::Error(), mListenSocket(socket) {

    }

    void EpollApi::InitEpoll() {

        if(!mListenSocket.IsBitwise(SOCK_STATUS::LISTEN)
        && !mListenSocket.IsBitwise(SOCK_STATUS::BINDING) ) {
            LoggingByClientError("Socket is not listening and binding");
            return;
        }

        mEpollRoot = epoll_create1(0);
        if( mEpollRoot == -1 ) {
            LoggingBySystemcallError("epoll_create1 error:");
            return;
        }

        struct epoll_event ev{};

        ev.events = EPOLLIN ;
        ev.data.fd = mListenSocket.GetFd();
        if (epoll_ctl(mEpollRoot, EPOLL_CTL_ADD, mListenSocket.GetFd(), &ev) == -1) {
            LoggingBySystemcallError("epoll_ctl error:");
            return;
        }

        // 이 객체 상태값으로 성공완료함
        this->Success();

    }

    auto EpollApi::RunningEpoll(int backlogEventNum, int timeout, int (*delegation)(Socket)) -> void {

        LOG(INFO, "==================================");
        LOG(INFO, "running Method for Event Delegate!");

        auto pEvents = make_unique<struct epoll_event[]>(backlogEventNum);
        if( pEvents == nullptr ) {
            LoggingBySystemcallError("make_unique error");
            throw system_error();
        }

        while (true)
        {
            int nCnt = epoll_wait(mListenSocket.GetFd(), pEvents.get(), backlogEventNum, timeout);
            if (nCnt == -1 ) {
                LoggingBySystemcallError("epoll_wait error");
                throw system_error();
            } else if (nCnt == 0){
                LOG(DEBUG, "epoll_wait timeout(", timeout, ")");
                continue;
            }

            for(int i=0; i<nCnt; i++) {

                if( pEvents[i].data.fd == mListenSocket.GetFd() ) {
                    LOG(DEBUG, "Listen,  Process to add Epoll List ");

                    if(AddFdInEpollList(delegation) == -1 ) {
                        LoggingByClientError("AddFdInEpollList error");
                        throw system_error();
                    }
                }
                else {

                    LOG(DEBUG, "Recv -> Processing");

                    int(*pFunc)(int) = ( int(*)(int) )( pEvents[i].data.ptr );
                    LOG(INFO, "Recv FD[", pEvents[i].data.fd, "]");

                    if( -1 == pFunc(pEvents[i].data.fd) ) {
                        InjectedByClientError("fail to process Data");
                        LOG(ERROR, "fail to process Data");
                    }

                }
            }
        }
    }

    int EpollApi::AddFdInEpollList( int(*delegation)(Socket) ) {

        TcpApi tcpApi{mListenSocket};

        auto requestToConnect = tcpApi.Accept();
        if( tcpApi.Status() ) {
            LoggingByClientError("TcpApi Accept() error");
            return  -1;
        }

        LOG(INFO, "Accepted FD[", requestToConnect, "]");

        auto p = make_unique<EpollData>( EpollData{requestToConnect, delegation});

        struct epoll_event ev{};
        ev.events = EPOLLIN;
        ev.data.ptr = p.get();
        if (epoll_ctl(mEpollRoot, EPOLL_CTL_ADD, requestToConnect, &ev) == -1) {
            LoggingBySystemcallError("epoll_ctl() for adding FD");
            return  -1;
        }

        return 0;
    }

} // CommunicationMember