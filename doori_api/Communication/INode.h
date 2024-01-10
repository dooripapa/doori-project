//
// Created by jaeseong on 24. 1. 10.
//

#ifndef DOORI_PROJECT_INODE_H
#define DOORI_PROJECT_INODE_H

namespace doori::api::Communication {
    /**
     * @brief From() 함수는 source 지역에서 해야 할 일을 정의함.
     * To() 함수는 destination 연결하기 위한 지원하는 함수
     */
    class INode {

    public:
        virtual ~INode() = default;

        virtual int From() = 0;

        virtual int To() = 0;

    };
}//doori


#endif //DOORI_PROJECT_INODE_H
