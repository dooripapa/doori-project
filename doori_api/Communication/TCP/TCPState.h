//
// Created by jslee on 24. 1. 6.
//

#ifndef DOORI_PROJECT_TCPSTATE_H
#define DOORI_PROJECT_TCPSTATE_H

#include "TCPNode.h"
#include "Common/Error.h"

namespace doori::api::Communication::TCP {

    class TCPNode;

    class TCPState : public Error{
    public:
        virtual void establish(TCPNode *node);

        virtual void close(TCPNode *node);

        virtual void wait(TCPNode *node);
    protected:
        static void changeState(TCPNode *node, TCPState *state);

        /**
         * 입력 받은 port로 바인딩 합니다.
         * @param sock ::socket(AF_INET, SOCK_STREAM, 0) 함수로 리턴된 값.
         * @param ip 공백 문자열이 들어오면 INADDR_ANY로 바인딩.
         * @param port 양의 정수 문자열
         * @return -1이면 error, > 0, 바인딩 fd값.
         */
        static int binding(int sock, const string& ip, const string& port);

        /**
         * binding된 소켓을 listen상태로 변경.
         * @param sock binding된 ::socket(AF_INET, SOCK_STREAM, 0) 함수
         * @param backlogNum 접속가능한 listen 대기큐 수
         * @return -1이면 error, > 0, 바인딩 fd값.
         */
        static int listen(int sock, int backlogNum);

        /**
         * 입력 받은 ip 유효성 검증.
         * @param ip
         * @return 실패 false, 성공 true
         */
        static bool IsValidIP(const string& ip);

        /**
         * 입력 받은 port 유효성 검증.
         * @param port
         * @return 실패 false, 성공 true
         */
        static bool IsValidPort(const string& port);

        /**
         * 소켓 재사용 설정함.
         * @param sock ::socket(AF_INET, SOCK_STREAM, 0) 리턴 된 fd값.
         * @param ip 재사용 될 ip 주소값.
         * @param port 재사용 될 port 주소값.
         * @return
         */
        static bool reuseSockOpt(int sock, const string &ip, const string &port);

        /**
         * ip, port 체크함.
         * @param ip
         * @param port
         * @return 체크 이상 없으면 port를 int형으로 리턴함. 실패하면 -1
         */
        static int checkAddress(const string &ip, const string &port);

        /**
         * Timeout Non-blocking으로 설정함.
         * @param sock ::socket(AF_INET, SOCK_STREAM, 0) 리턴 된 fd값.
         * @param timeout
         * @return 실패시 false 성공시 true
         */
        static bool setNonblockingOpt(int sock, std::uint8_t timeout);
    };
}


#endif //DOORI_PROJECT_TCPSTATE_H
