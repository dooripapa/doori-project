//
// Created by jaeseong on 23. 2. 7.
//

#ifndef DOORI_PROJECT_ICONNECTION_H
#define DOORI_PROJECT_ICONNECTION_H

#include "DataStream/IStream.h"

namespace doori {
    namespace CommunicationMember {
        class IConnection {
            /**
             * 통신을 위해 연결요청합니다. 연결 요청 후, 완료가 되면, 통신수단의 접근자를 리턴합니다.
             * @return 통신 객체
             */
            virtual int ConnectTo() = 0;
            /**
             * 통신 연결요청 기다립니다. 연결 요청이 완료가 되면, 통신수단의 접근자를 리턴합니다.
             * @return 통신 객체
             */
            virtual int WaitFor() = 0;
            /**
             * 통신 객체를 이용하여, 데이터 보냅니다.
             * @param rscFd 통신객체
             * @param data 데이터
             * @return 0이면 성공, 그 외 에러
             */
            virtual int Send(int rscFd, const DataStream::IStream& data) = 0;
            /**
             * 통신 객체를 이용하여, 데이터 수신되기를 기다립니다.
             * @param rscFd 통신객체
             * @param data 데이터
             * @return 0이면 성공, 그 외 에러
             */
            virtual int Recv(int rscFd, DataStream::IStream& data) = 0;
        };
    }
}

#endif //DOORI_PROJECT_ICONNECTION_H
