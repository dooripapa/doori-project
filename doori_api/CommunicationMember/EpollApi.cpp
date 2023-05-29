//
// Created by jaeseong on 23. 5. 23.
//

#include "EpollApi.h"

namespace doori {
    namespace CommunicationMember {
        EpollApi::EpollApi() : Common::Error(), mEpollSocket(-1, SOCK_STATUS::CLOSED), mListenSocket(-1, SOCK_STATUS::CLOSED){

        }

        EpollApi::EpollApi(const Socket& listenSocket) : Common::Error(), mEpollSocket(-1, SOCK_STATUS::CLOSED), mListenSocket(listenSocket) {

        }

        void EpollApi::CreateEpoll() {

            if(!mListenSocket.IsBitwise(SOCK_STATUS::LISTEN)
            && !mListenSocket.IsBitwise(SOCK_STATUS::BINDING) ) {
                InjectedByClientError("Socket is not listening and binding");
                LOG(ERROR, "CreateEpoll error:", Cause() );
                return;
            }

            auto iEpollFd = epoll_create1(0);
            if( iEpollFd == -1 ) {
                InjectedBySystemcall();
                LOG(ERROR, "epoll_create1 error:", Cause() );
                return;
            }

            struct epoll_event ev{};

            ev.events = EPOLLIN ;
            ev.data.fd = mListenSocket.GetFd();
            if (epoll_ctl(iEpollFd, EPOLL_CTL_ADD, mListenSocket.GetFd(), &ev) == -1) {
                InjectedBySystemcall();
                LOG(ERROR, "epoll_ctl error:", Cause() );
                return;
            }

            mEpollSocket = Socket{iEpollFd, SOCK_STATUS::INIT};

        }

        auto EpollApi::RunningEpoll(int backlogEventNum, int timeout, int (*delegation)(Socket)) -> void {

            if(!mEpollSocket.IsBitwise(SOCK_STATUS::INIT))
                throw system_error();

            if(!mListenSocket.IsBitwise(SOCK_STATUS::LISTEN)
            && !mListenSocket.IsBitwise(SOCK_STATUS::BINDING) )
                throw system_error();

            LOG(INFO, "==================================");
            LOG(INFO, "running Method for Event Delegate!");

            auto pEvents = make_unique<struct epoll_event[]>(backlogEventNum);
            if( pEvents == nullptr ) {
                LOG(ERROR, "make_unique error");
                throw system_error();
            }

            while (true)
            {
                int nCnt = epoll_wait(mEpollSocket.GetFd(), pEvents.get(), backlogEventNum, timeout);
                if (nCnt == -1 ) {
                    InjectedBySystemcall();
                    throw system_error();
                } else if (nCnt == 0){
                    LOG(DEBUG, "timeout!");
                    continue;
                }

                for(int i=0; i<nCnt; i++) {

                    if(pEvents[i].data.fd == mListenSocket.GetFd() ) {

                        LOG(DEBUG, "Listen -> Accept");

                        if( AddAsEpollList( delegation ) == -1 ) {
                            InjectedByClientError("AddAsEpollList error");
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

        int EpollApi::AddAsEpollList( int(*delegation)(Socket) ) {

            TcpApi tcpApi{mListenSocket};

            auto conn_sock = tcpApi.Accept();
            if( tcpApi.Status() ) {
                InjectedByClientError("TcpApi Accept() error");
                return  -1;
            }

            LOG(INFO, "Accepted FD[", conn_sock.GetFd(),"]");

            // Timeout 설정함
            tcpApi.SetTimeoutOpt(10);
            if (tcpApi.Status()) {
                InjectedByClientError("TcpApi SetTimeoutOpt() error.");
                return  -1;
            }

            struct epoll_event ev{};
            ev.events = EPOLLIN;
            ev.data.fd = conn_sock.GetFd();
            ev.data.ptr = (void*)delegation;
            if (epoll_ctl(mEpollSocket.GetFd(), EPOLL_CTL_ADD, conn_sock.GetFd(), &ev) == -1) {
                InjectedBySystemcall();
                LOG(ERROR, "epoll_ctl error : fail to EPOLL_CTL_ADD");
                return  -1;
            }

            return 0;
        }

    } // doori
} // CommunicationMember