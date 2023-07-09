//
// Created by jaeseong on 23. 5. 23.
//

#include <memory>
#include <thread>
#include <utility>
#include "EpollApi.h"
#include "TcpApi.h"

namespace doori::api::Communication{

    EpollApi::EpollApi(Socket socket) : Common::Error(), mListenSocket(socket) {

    }

    void EpollApi::InitEpoll( std::function<int(Socket)> delegator ) {

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
        auto pEpollData = new (struct EpollData);
        pEpollData->mFd = mListenSocket.GetFd();
        pEpollData->mCallEpollApiProcess = nullptr;

        mUserFunction = std::move(delegator);

        struct epoll_event ev{};

        ev.events = EPOLLIN ;
        ev.data.ptr = pEpollData;
        if (epoll_ctl(mEpollRoot, EPOLL_CTL_ADD, mListenSocket.GetFd(), &ev) == -1) {
            LoggingBySystemcallError("epoll_ctl error:");
            return;
        }

        // 이 객체 상태값으로 성공완료함
        this->AsSuccess();

    }

    auto EpollApi::RunningForeground(int backlogEventNum, int timeout ) -> void {

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
                auto structP = static_cast<struct EpollData*>(pEvents[i].data.ptr);

                if( structP->mFd == mListenSocket.GetFd() ) {
                    LOG(DEBUG, "Listen,  Process to add Epoll List ");

                    if(addFdInEpollList() == -1 ) {
                        LoggingByClientError("addFdInEpollList error");
                        throw system_error();
                    }
                }
                else {
                    LOG(DEBUG, "Recv -> Processing, FD[", structP->mFd, "]");

                    Socket socket = Socket{structP->mFd, static_cast<enum SOCK_STATUS>(SOCK_STATUS::INIT | SOCK_STATUS::ESTABLISED) };

                    if( -1 == structP->mCallEpollApiProcess( socket ) ) {

                        LoggingByClientError("User's Process done");
                        LOG(ERROR,"socket close(", structP->mFd,")", "heap free(", structP, ")");

                        close(structP->mFd);
                        delete structP;
                    }

                }//else

            }//for

        }//while
    }

    int EpollApi::addFdInEpollList( ) {

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
        structP->mCallEpollApiProcess = mUserFunction;

        struct epoll_event ev{};
        ev.events = EPOLLIN;
        ev.data.ptr = p;
        if (epoll_ctl(mEpollRoot, EPOLL_CTL_ADD, requestToConnect, &ev) == -1) {
            LoggingBySystemcallError("epoll_ctl() for adding FD");
            return  -1;
        }

        return 0;
    }

    void EpollApi::RunningBackground(int backlogEventNum, int timeout) {

        std::function< void(int, int) > threadFunction = [this](int backlogEventNum, int timeout) {
            this->RunningForeground(backlogEventNum, timeout);
        };

        //쓰레드 초기화합니다.
        thread t{ threadFunction, backlogEventNum, timeout};

        //관리가능하도록 멤버변수로 저장함.
        mBackground.swap(t);

    }

    void EpollApi::JoinBackground() {

        mBackground.join();

    }

    int EpollApi::AddFdIntoEpollList(int fd, function<int(Socket)> delegator) {

        LOG(INFO, "Register FD[", fd, "]");

        auto pEpollData = new (struct EpollData);
        pEpollData->mFd = fd;
        pEpollData->mCallEpollApiProcess = std::move(delegator);

        struct epoll_event ev{};
        ev.events = EPOLLIN;
        ev.data.ptr = pEpollData;
        if (epoll_ctl(mEpollRoot, EPOLL_CTL_ADD, fd, &ev) == -1) {
            LoggingBySystemcallError("epoll_ctl() for adding FD");
            return  -1;
        }

        return 0;
    }

    int EpollApi::AddFdIntoEpollList(Socket socket, function<int(Socket)> delegator) {

        auto fd = socket.GetFd();

        LOG(INFO, "Register FD[", fd, "]");

        auto pEpollData = new (struct EpollData);
        pEpollData->mFd = fd;
        pEpollData->mCallEpollApiProcess = std::move(delegator);

        struct epoll_event ev{};
        ev.events = EPOLLIN;
        ev.data.ptr = pEpollData;
        if (epoll_ctl(mEpollRoot, EPOLL_CTL_ADD, fd, &ev) == -1) {
            LoggingBySystemcallError("epoll_ctl() for adding FD");
            return  -1;
        }

        return 0;
    }

} // Communication