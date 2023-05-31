//
// Created by jaeseong on 23. 5. 23.
//

#include <memory>
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

        //공용체이므로, fd외 더 많은 정보를 저장하기 위해. 포인터를 활용함
        auto p = malloc(sizeof(struct EpollData));
        auto structP = (struct EpollData*)p;
        structP->mFd = mListenSocket.GetFd();
        structP->mCallEpollApiProcess = nullptr;

        struct epoll_event ev{};

        ev.events = EPOLLIN ;
        ev.data.ptr = p;
        if (epoll_ctl(mEpollRoot, EPOLL_CTL_ADD, mListenSocket.GetFd(), &ev) == -1) {
            LoggingBySystemcallError("epoll_ctl error:");
            return;
        }

        // 이 객체 상태값으로 성공완료함
        this->Success();

    }

    auto EpollApi::RunningEpoll(int backlogEventNum, int timeout, int(*delegation)(Socket) ) -> void {

        LOG(INFO, "==================================");
        LOG(INFO, "running Method for Event Delegate!");

        auto pEvents = make_unique<struct epoll_event[]>(backlogEventNum);
        if( pEvents == nullptr ) {
            LoggingBySystemcallError("make_unique error");
            throw system_error();
        }

        // seconds
        auto seconds = timeout * 1000;

        while (true)
        {
            int nCnt = epoll_wait(mEpollRoot, pEvents.get(), backlogEventNum, seconds);
            if (nCnt == -1 ) {
                LoggingBySystemcallError("epoll_wait error");
                throw system_error();
            } else if (nCnt == 0){
                LOG(DEBUG, "epoll_wait timeout(", timeout, "s)");
                continue;
            }

            for(int i=0; i<nCnt; i++) {

                //공용체이므로
                auto structP = (struct EpollData*)pEvents[i].data.ptr;

                if( structP->mFd == mListenSocket.GetFd() ) {
                    LOG(DEBUG, "Listen,  Process to add Epoll List ");

                    if( AddFdInEpollList() == -1 ) {
                        LoggingByClientError("AddFdInEpollList error");
                        throw system_error();
                    }
                }
                else {
                    LOG(DEBUG, "Recv -> Processing, FD[", structP->mFd, "]");

                    if( -1 == structP->mCallEpollApiProcess( structP->mFd, delegation ) ) {

                        LoggingByClientError("User's Process done");
                        LOG(ERROR,"socket close(", structP->mFd,")", "heap free(", structP, ")");

                        close(structP->mFd);
                        free(structP);
                    }

                }//else

            }//for

        }//while
    }

    int EpollApi::CallEpollApiProcess( int fd, int(*userFunc)(Socket socket) ) {

        Socket socket = Socket{fd, static_cast<enum SOCK_STATUS>(SOCK_STATUS::INIT | SOCK_STATUS::ESTABLISED) };

        auto ret = userFunc(socket);

        if(ret<=0)
        {
           if(ret==0)
               LOG(INFO, "Closed from the other side" );
           else
               LOG(ERROR, "fail to process defined User's Function" );

            return -1;
        }

        return 0;
    }

    int EpollApi::AddFdInEpollList( ) {

        TcpApi tcpApi{mListenSocket};

        auto requestToConnect = tcpApi.Accept();
        if( tcpApi.Status() ) {
            LoggingByClientError("TcpApi Accept() error");
            return  -1;
        }

        LOG(INFO, "Accepted FD[", requestToConnect, "]");

        auto p = malloc(sizeof(struct EpollData));
        auto structP = (struct EpollData*)p;
        structP->mFd = requestToConnect;
        structP->mCallEpollApiProcess = EpollApi::CallEpollApiProcess;

        struct epoll_event ev{};
        ev.events = EPOLLIN;
        ev.data.ptr = p;
        if (epoll_ctl(mEpollRoot, EPOLL_CTL_ADD, requestToConnect, &ev) == -1) {
            LoggingBySystemcallError("epoll_ctl() for adding FD");
            return  -1;
        }

        return 0;
    }

} // CommunicationMember