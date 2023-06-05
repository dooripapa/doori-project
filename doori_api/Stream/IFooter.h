//
// Created by jaeseong on 23. 6. 5.
//

#ifndef DOORI_PROJECT_IFOOTER_H
#define DOORI_PROJECT_IFOOTER_H

#include <vector>

using namespace std;

namespace doori::Stream{

    class IFooter{
    public:
        /**
         * footer 구성된 row data를 리턴합니다.
         * @return vector<char>
         */
        virtual vector<char> Get() = 0;
    };
};

#endif //DOORI_PROJECT_IFOOTER_H
