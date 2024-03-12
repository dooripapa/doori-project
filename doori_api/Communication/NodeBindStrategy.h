//
// Created by KOSCOM on 2024-01-10.
//

#ifndef DOORI_PROJECT_NODEBINDSTRATEGY_H
#define DOORI_PROJECT_NODEBINDSTRATEGY_H

#include "INode.h"

namespace doori::api::Communication {
    /**
     * INode 인터페이스를 상속 받아서, Node가 기본적으로 호출해야 할 함수를 정의한다.
     * 더불어, 기본적으로 호출되어야 할 함수는 전략패턴을 사용해서 주입한다.
     * @tparam T : TCPNode, SHMNode, UDPNode ...
     * @tparam TFromStrategy : source 영역에서 remote영역에 접속하기 위한 기본적이 로직 정의.
     * @tparam TToStragegy : remote에서 연결을 위한 기본적인 로직 정의
     */
    template<typename T, typename TFromStrategy, typename TToStragegy = std::function<int(T &t)> >
    class NodeBindStrategy : public INode {
        using FromStrategy = std::function<int(T &t)>;
        using ToStrategy = std::function<int(T &t)>;
    public:
        NodeBindStrategy(T &t, const TFromStrategy &from, const TToStragegy &to = [](T &t){return 0;} )
        : _t(t), _from(from), _to(to) {}

        int From() override {
            return _from(_t);
        };

        int To() override {
            return _to(_t);
        };

    private:
        T &_t;
        FromStrategy _from;
        ToStrategy _to;

    };
}//doori

#endif //DOORI_PROJECT_NODEBINDSTRATEGY_H
