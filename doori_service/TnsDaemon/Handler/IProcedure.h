//
// Created by jaeseong on 23. 5. 8.
//

#ifndef DOORI_PROJECT_IPROCEDURE_H
#define DOORI_PROJECT_IPROCEDURE_H

#include "Recv.h"

namespace doori {
    namespace TnsDaemon {
        namespace Handler {

            /**
             * 전방선언
             */
            class State;

            /*!
             * Tnsd 기준으로 절차가 진행되어야 함.
             */
            class IProcedure {
            public:
                /**
                 * Tnsd() 최초기동상태
                 */
                IProcedure();

                /**
                 * Tnsd 연결된 노드에게 어떤 정보를 알림
                 */
                void notify();
                /**
                 * Tnsd의 데이터를 최신화함.
                 */
                void listUp();
                /**
                 * Tnsd에서 뭔가 변화가 발생됨.
                 */
                void changed();
                /**
                 * 어떤 노드로부터 report 요청을 받음. 응답을 보냄.
                 */
                void report();
                /**
                 * 어떤 노드가 Tnsd에게 alive 상태를 보냄.
                 */
                void alive();
                /**
                 * publiser 등록
                 */
                void pub();
                /**
                 * subscriber 등록
                 */
                void sub();
                /**
                 * Tnsd() 데이터 송신
                 * @param protocol
                 */
                void send();
                /**
                 * Tnsd() 데이터 수신
                 */
                void recv();
            private:
                friend class State;
                void changedState(State *);
                State * mState;
            };

        } // doori
    } // TnsDaemon
} // Handler

#endif //DOORI_PROJECT_IPROCEDURE_H
