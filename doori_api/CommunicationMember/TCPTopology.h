//
// Created by jaeseong on 23. 2. 7.
//

#ifndef DOORI_PROJECT_TCPTOPOLOGY_H
#define DOORI_PROJECT_TCPTOPOLOGY_H

#include "ITopology.h"
#include "Endpoint.h"

namespace doori {
    namespace CommunicationMember {

        class TCPTopology : public ITopology {
        public:
            TCPTopology() = delete;

            virtual ~TCPTopology();

            /**
             * 초기화를 합니다.
             * @param dataPassage 데이터를 주고 받을 수 있는 추상리소스
             */
            TCPTopology(int dataPassage);

            /**
             * 원격지 노드에 데이르터ㄹ 송신합니다.
             * @param data
             * @return 0 : Success, -1 : Fail
             */
            int Send(const DataStream::IStream &data) override;

            /**
             * 원격지으로 온 데이터를 수신합니다.
             * @param data
             * @return 0 : Success, -1 : Fail
             */
            int Recv(DataStream::IStream &data) override;
        private:
            int mDataPassage;
        };

    } // doori
} // CommunicationMember

#endif //DOORI_PROJECT_TCPTOPOLOGY_H
