//
// Created by jaeseong on 23. 2. 7.
//

#ifndef DOORI_PROJECT_ITOPOLOGY_H
#define DOORI_PROJECT_ITOPOLOGY_H
#include <string>


namespace doori {
    namespace CommunicationMember {
        class ITopology {
        public:
            /**
             * 통신 객체를 이용하여, 데이터 보냅니다.
             * @param data 보낼 데이터
             * @return 0이면 성공, 그 외 에러
             */
            virtual int Send(const std::string& data) = 0;
            /**
             * 통신 객체를 이용하여, 데이터 수신되기를 기다립니다.
             * @param data 수신 데이터를 담을 컨테이너
             * @param utilSize 지정된 값이 수신 될 때까지 데이터 사이즈값.
             * @return 0이면 성공, 그 외 에러
             */
            virtual int Recv(std::string& data, std::uint8_t utilSize) = 0;
        };
    }
}

#endif //DOORI_PROJECT_ITOPOLOGY_H
