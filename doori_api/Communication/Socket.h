//
// Created by jaeseong on 23. 5. 19.
//

#ifndef DOORI_PROJECT_SOCKET_H
#define DOORI_PROJECT_SOCKET_H

#include "Common/Log.h"
#include "Common/Error.h"
#include "ITopologyNode.h"
#include <sys/socket.h>
#include <bits/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <functional>

namespace doori::api::Communication {
////////////////////////////////////////////////////////// // 비트 연산을 위해서 enum class 선언하지 않고
    // 단순 enum으로 처리.
    //////////////////////////////////////////////////////////
    enum SOCK_STATUS {
        INIT        = 0
        ,BINDING    = 1 << 0
        ,LISTEN     = 1 << 1
        ,ACCEPT     = 1 << 2
        ,ESTABLISED = 1 << 3
        ,CLOSED     = 1 << 4
    };

    //////////////////////////////////////////////////////////
    // 소켓 옵션 설정
    // 비트 연산을 위해서 enum class 선언하지 않고
    // 단순 enum으로 처리.
    //////////////////////////////////////////////////////////
    enum SOCK_OPT {
        REUSE           = 0           // 주소와 포트를 재사용할 수 있도록 허용합니다.
        ,KEEPALIVE      = 1 << 0      // TCP 연결에서 keep-alive 메시지를 사용하도록 설정합니다.
        ,LINGER         = 1 << 1      // 소켓이 close될 때 남아 있는 데이터 처리를 제어합니다.
        ,RCVBUF         = 1 << 2      // 수신 버퍼의 크기를 설정합니다.
        ,SNDBUF         = 1 << 3      // 송신 버퍼의 크기를 설정합니다.
        ,RCVTIMEO       = 1 << 4      // 수신 시간 초과를 설정합니다.
        ,SNDTIMEO       = 1 << 5      // 송신 시간 초과를 설정합니다.
        ,BROADCAST      = 1 << 6      // 소켓에서 브로드캐스트 메시지를 허용합니다.
        ,OFF_NAGLE      = 1 << 7      // Nagle 알고리즘을 비활성화하여 지연을 최소화합니다.
    };

    class Socket : public ITopologyNode {
    public:
        Socket();
        Socket(int fd, SOCK_STATUS status);
        Socket& operator=(const Socket & rhs) = default;
        Socket& operator=(Socket && rhs) = default;
        Socket(const Socket & rhs) = default;
        Socket(Socket && rhs) = default;
        virtual ~Socket() = default;

        [[nodiscard]] bool Init() ;

        [[nodiscard]] bool IsBitwise(SOCK_STATUS status) const;

        int GetBitwise() const;

        void SetBitwise(SOCK_STATUS status);

        void SetBitwise(int fd, SOCK_STATUS status);

        void SetOptBitwise(SOCK_OPT option);

        [[nodiscard]] bool IsOptBitwise(SOCK_OPT option) const;

        [[nodiscard]] int GetFd() const;

        long Send(const string& data) const ;

        long Send(const char* data, uint16_t dataSize) const;

        template<int N>
        long Send(char const(&data)[N]) const {
            auto ret = send(mFd, data, N-1, 0);
            if(ret == -1) {
                LOG(ERROR, "send error" );
                return -1;
            }
            return ret;
        };

        long Recv(string &container, uint32_t tilDataSize) const;

        int Close();


    private:
        int mFd;
        enum SOCK_STATUS mStatus;
        enum SOCK_OPT mOption;
    };

} // Communication

#endif //DOORI_PROJECT_SOCKET_H
