//
// Created by jaeseong on 23. 5. 23.
//

#ifndef DOORI_PROJECT_EPOLLAPI_H
#define DOORI_PROJECT_EPOLLAPI_H

#include "Common/Error.h"
#include "Socket.h"

namespace doori {
    namespace CommunicationMember {

        class EpollApi final: public Common::Error  {
        public:
            EpollApi();
            explicit EpollApi(const Socket &listenSocket);
            EpollApi(const EpollApi&) = delete;
            EpollApi(EpollApi&&) = delete;
            EpollApi& operator=(const EpollApi&) = delete;
            EpollApi& operator=(EpollApi&&) = delete;

            /**
             * Epoll 생성합니다.
             * @note socketFd는 넘겨주기전에 바인딩된 상태이어야 한다.
             * @param backlogNum  backlogNum 접속 요청이 들어올 경우, 최대 대기 배열 수
             * @return Epoll용 Socket Wrapper 클래스
             */
            void CreateEpoll();

            /**
             * Epoll loop back형식으로 처리한다. Epoll의 연결요청이 아닌 데이터수신 이벤트는 delegation함수로 처리한다.
             * @param backlogEventNum  epoll event가 발생시, 최대 대기 이벤트 수
             * @param timeout  epoll event가 타임아웃 값.
             * @param delegation 데이터를 수신시, 처리한다. ex) int delegation( Socket socket )
             * @return 리턴하지 않음.
             */
            [[noreturn]] void RunningEpoll(int backlogEventNum, int timeout, int (*delegation)(Socket));

        private:
            int AddAsEpollList( int(*delegation)(Socket) );

            Socket mEpollSocket;
            Socket mListenSocket;
        };

    } // doori
} // CommunicationMember

#endif //DOORI_PROJECT_EPOLLAPI_H
