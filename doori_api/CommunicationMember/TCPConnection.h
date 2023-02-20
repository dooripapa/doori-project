//
// Created by jaeseong on 23. 2. 7.
//

#ifndef DOORI_PROJECT_TCPCONNECTION_H
#define DOORI_PROJECT_TCPCONNECTION_H

#include "IConnection.h"
#include "Endpoint.h"

namespace doori {
    namespace CommunicationMember {

        class TCPConnection : public IConnection {
        public:
            TCPConnection() = delete;

            TCPConnection(Endpoint From, Endpoint To);

            int ConnectTo() override;

            int WaitFor() override;

            int Send(int rscFd, const DataStream::IStream &data) override;

            int Recv(int rscFd, DataStream::IStream &data) override;

        private:
            Endpoint  mFromInfo;
            Endpoint  mToInfo;

            /**
             * struct sockaddr_in 구조체를 ip, port값으로 배정합니다.
             * @param addr
             * @param ip
             * @param port
             */
            static void Set_sockaddr_in(sockaddr_in &addr, const string& ip, const string& port);

            /**
             * Local ip, Local port 입력받아서 재사용(setsockopt) 소켓을 리턴합니다.
             * @param ip local ip
             * @param port local port
             * @return -1 fail. 0 보다 크면 성공, 소켓리턴
             */
            [[nodiscard]] static int Bind(const string& ip, const string& port);
        };

    } // doori
} // CommunicationMember

#endif //DOORI_PROJECT_TCPCONNECTION_H
