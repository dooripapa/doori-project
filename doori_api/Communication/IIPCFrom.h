//
// Created by jaeseong on 24. 3. 21.
//

#ifndef DOORI_PROJECT_IIPCFROM_H
#define DOORI_PROJECT_IIPCFROM_H

#include <string>

class IIPCFrom{

public:
    virtual int waitFor() = 0;

    virtual int replay(const std::string& answer) = 0;

    virtual int from() = 0;
};

#endif //DOORI_PROJECT_IIPCFROM_H
