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
        auto divide(const std::string &v1, const std::string &v2) -> tuple<string, string> {
            Bigdecimal v{"Test"};
            return v.divide(v1, v2);
        }
    };

}



#endif //DOORI_PROJECT_DOORITEST_H
