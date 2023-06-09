//
// Created by jaeseong on 23. 6. 5.
//

#ifndef DOORI_PROJECT_STREAMTEMPLATE_H
#define DOORI_PROJECT_STREAMTEMPLATE_H


#include "IHeader.h"
#include "IBody.h"
#include <vector>
#include "Common/Error.h"
#include "Common/Util.h"
#include <iostream>

using namespace std;

namespace doori::Stream {

    template<typename T>
    class StreamTemplate :  public T , Common::Error{
    public:
        StreamTemplate(T t) : T(t), Common::Error(0, true) {
        }

    };
};

#endif //DOORI_PROJECT_STREAMTEMPLATE_H
