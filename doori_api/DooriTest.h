//
// Created by jaeseong on 22. 11. 11.
//

#ifndef DOORI_PROJECT_DOORITEST_H
#define DOORI_PROJECT_DOORITEST_H

#include <string>
#include <Bigdecimal.h>

namespace doori {

    class DooriTest {
    public:
        auto revisionAt(const std::string &v1, ushort belowZeroLen) -> std::string {
            Bigdecimal v{"Test"};
            return v.revisionAt(v1, belowZeroLen);;
        }
    };

}



#endif //DOORI_PROJECT_DOORITEST_H
