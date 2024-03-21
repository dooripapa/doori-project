//
// Created by jaeseong on 24. 3. 21.
//

#include "FromStategy.h"

IPC FromStategy::operator()(TCP tcp) noexcept {

    auto node = tcp.getTopologyNode();

    return 0;
}
