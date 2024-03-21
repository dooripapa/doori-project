//
// Created by jaeseong on 24. 3. 21.
//

#ifndef DOORI_PROJECT_IIPCTO_H
#define DOORI_PROJECT_IIPCTO_H

#include "IIPCFrom.h"

class IIPCTo{
public:
    virtual int connect(IIPCFrom) = 0;

    virtual int to() = 0;
};

#endif //DOORI_PROJECT_IIPCTO_H
