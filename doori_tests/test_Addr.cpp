//
// Created by jaeseong on 23. 1. 31.
//
#include <gtest/gtest.h>
#include "CommunicationMember/Addr.h"
#include "CommunicationMember/Connection.h"
#include "CommunicationMember/Endpoint.h"
#include "CommunicationMember/Epoll.h"
#include "CommunicationMember/EpollEvents.h"
#include <iostream>

using namespace doori;
using namespace std;

TEST(Communication, Sample)
{
    Addr addr{"127.0.0.1", "9999"};

    Endpoint endpoint{addr};
    endpoint.CanRead();

}
