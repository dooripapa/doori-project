//
// Created by jaeseong on 23. 5. 23.
//

#ifndef DOORI_PROJECT_EPOLLAPI_H
#define DOORI_PROJECT_EPOLLAPI_H

#include "Common/Error.h"
#include "Socket.h"

namespace doori::Communication {

    class EpollApi;

    struct EpollData {
    private:
        friend EpollApi;
        int mFd;
        int (*mCallEpollApiProcess)( int fd, int(*userFunc)(Socket) );
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
         * @param timeout  epoll event가 타임아웃 값(초) second.
         * @param delegation 데이터를 수신시, 처리한다. ex) int delegation( Socket socket )
         * @note delegation 유저용 프로세스를 구현할 때, 0인 경우 상대측으로 부터 소켓이 닫는 상태값으로 사용해야 하며,
         * 그 외는 음수는 에러처리가 된다. 내부에서 자동으로 소켓을 닫고, 할당된 메모리를 해제한다.
         */
        [[noreturn]] void RunningForeground(int backlogEventNum, int timeout, int(*delegation)(Socket) );

    private:
        int AddFdInEpollList();

        static int CallEpollApiProcess( int fd, int(*userFunc)(Socket socket)  );

        int    mEpollRoot;
        Socket & mListenSocket;
    };

} // Communication

#endif //DOORI_PROJECT_EPOLLAPI_H
