//
// Created by jaeseong on 23. 5. 19.
//

#ifndef DOORI_PROJECT_SOCKET_H
#define DOORI_PROJECT_SOCKET_H

#include "Common/Log.h"
#include "TcpApi.h"

namespace doori {
    namespace CommunicationMember {

        //////////////////////////////////////////////////////////
        // 비트 연산을 위해서 enum class 선언하지 않고
        // 단순 enum으로 처리.
        //////////////////////////////////////////////////////////
        enum SOCK_STATUS {
            INIT        = 0
            ,BINDING    = 1 << 0
            ,LISTEN     = 1 << 1
            ,ACCEPT     = 1 << 2
            ,ESTABLISED = 1 << 3
            ,CLOSED     = 1 << 4
            ,FAILURE    = 1 << 5
        };

        //////////////////////////////////////////////////////////
        // 소켓 옵션 설정
        // 비트 연산을 위해서 enum class 선언하지 않고
        // 단순 enum으로 처리.
        //////////////////////////////////////////////////////////
        enum SOCK_OPT {
            REUSE                   // 주소와 포트를 재사용할 수 있도록 허용합니다.
            ,KEEPALIVE              // TCP 연결에서 keep-alive 메시지를 사용하도록 설정합니다.
            ,LINGER                 // 소켓이 close될 때 남아 있는 데이터 처리를 제어합니다.
            ,RCVBUF                 // 수신 버퍼의 크기를 설정합니다.
            ,SNDBUF                 // 송신 버퍼의 크기를 설정합니다.
            ,RCVTIMEO               // 수신 시간 초과를 설정합니다.
            ,SNDTIMEO               // 송신 시간 초과를 설정합니다.
            ,BROADCAST              // 소켓에서 브로드캐스트 메시지를 허용합니다.
            ,OFF_NAGLE              // Nagle 알고리즘을 비활성화하여 지연을 최소화합니다.
        };

        class Socket {
        public:
            Socket() = delete;
            Socket(int fd, SOCK_STATUS status) : fd(fd), status(status) { };
            Socket& operator=(const Socket & rhs) = default;
            Socket& operator=(Socket && rhs) = default;
            Socket(const Socket & rhs) = default;
            Socket(Socket && rhs) = default;
            ~Socket() = default;

            [[nodiscard]] bool IsFailure() const;

            [[nodiscard]] bool IsBitwise(SOCK_STATUS status) const;

            void SetBitwise(int fd, SOCK_STATUS status);

        private:
            friend TcpApi;
            int fd;
            int status;
        };

    } // doori
} // CommunicationMember

#endif //DOORI_PROJECT_SOCKET_H
