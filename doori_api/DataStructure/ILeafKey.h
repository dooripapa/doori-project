//
// Created by jaeseong on 23. 6. 13.
//

#ifndef DOORI_PROJECT_ILEAFKEY_H
#define DOORI_PROJECT_ILEAFKEY_H

#include <string>

using namespace std;

namespace doori::DataStructure{

    class ILeafKey{

    public:
        virtual string GetKeyName() const noexcept = 0;
        virtual string GetDepthKey(unsigned int depth) const noexcept = 0;
        virtual uint GetDepth() const noexcept = 0;

    };

}

#endif //DOORI_PROJECT_ILEAFKEY_H
