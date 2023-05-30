//
// Created by jaeseong on 23. 5. 23.
//

#ifndef DOORI_PROJECT_EPOLLAPI_H
#define DOORI_PROJECT_EPOLLAPI_H

#include "Common/Error.h"
#include "Socket.h"

namespace doori::CommunicationMember {

    struct EpollData {
        int fd;
        int (*delegation)(Socket);
    };

    class EpollApi final: public Common::Error  {
    public:
        EpollApi() = delete;
        explicit EpollApi(Socket& socket);
        EpollApi(const EpollApi&) = delete;
        EpollApi(EpollApi&&) = delete;
        EpollApi& operator=(const EpollApi&) = delete;
        EpollApi& operator=(EpollApi&&) = delete;

        /**
         * Epoll 초기화합니다.
         * @note 생성자로부터 받은 Socket객체는 SOCK_STATUS::BINDING && SOCK_STATUS::LISTEN 이어야 합니다.
         */
        void InitEpoll();

        /**
         * Epoll loop back형식으로 처리합니다. Epoll의 연결요청이 아닌 데이터수신 이벤트는 delegation함수로 처리한다.
         * @param backlogEventNum  epoll event가 발생시, 최대 대기 이벤트 수
         * @param timeout  epoll event가 타임아웃 값.
         * @param delegation 데이터를 수신시, 처리한다. ex) int delegation( Socket socket )
         */
        [[noreturn]] void RunningEpoll(int backlogEventNum, int timeout, int (*delegation)(Socket));

    private:
        int AddFdInEpollList(int(*delegation)(Socket) );

        int    mEpollRoot;
        Socket mListenSocket;
    };

} // CommunicationMember

#endif //DOORI_PROJECT_EPOLLAPI_H
