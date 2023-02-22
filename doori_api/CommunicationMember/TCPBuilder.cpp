//
// Created by jaeseong on 23. 1. 31.
//

#include "TCPBuilder.h"
#include "TCP.h"
#include "Common/Log.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <unistd.h>

#include <utility>

namespace doori::CommunicationMember {

    TCPBuilder::TCPBuilder(Endpoint From, Endpoint To) {
        mFrom = From;
        mTo = To;
    }

    const Endpoint &TCPBuilder::getFrom() {
        return mFrom;
    }

    const Endpoint &TCPBuilder::getTo() {
        return mTo;
    }

    TCPBuilder::~TCPBuilder() {

    }
} // doori