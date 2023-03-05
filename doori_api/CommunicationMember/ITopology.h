//
// Created by jaeseong on 23. 2. 7.
//

#ifndef DOORI_PROJECT_ITOPOLOGY_H
#define DOORI_PROJECT_ITOPOLOGY_H

#include "DataStream/IStream.h"

namespace doori {
    namespace CommunicationMember {
        class ITopology {
        public:
            /**
             * 통신 객체를 이용하여, 데이터 보냅니다.
             * @param rscFd 통신객체
             * @param data 데이터
             * @return 0이면 성공, 그 외 에러
             */
            virtual int Send(const DataStream::IStream& data) = 0;
            /**
             * 통신 객체를 이용하여, 데이터 수신되기를 기다립니다.
             * @param rscFd 통신객체
             * @param data 데이터
             * @return 0이면 성공, 그 외 에러
             */
            virtual int Recv(DataStream::IStream& data) = 0;
        };
    }
}

#endif //DOORI_PROJECT_ITOPOLOGY_H
