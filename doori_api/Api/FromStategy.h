//
// Created by jaeseong on 24. 3. 21.
//

#ifndef IPC_FROMSTATEGY_H
#define IPC_FROMSTATEGY_H

#include "TCP.h"
#include "IPC.h"

class FromStategy {

public:

    IPC operator()(TCP) noexcept;

};


#endif //IPC_FROMSTATEGY_H
