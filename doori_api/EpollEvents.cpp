// Copyright 2019 The doori-project Authors. All rights reserved.
// Use of this source code is governed by a GPL-3.0 license that can be found in the LICENSE file.
// See the AUTHORS file for names of contributors.
//
// Created by doori on 19. 7. 25.
//
#include "EpollEvents.h"
#include <memory>

using namespace std;

namespace doori{

auto
EpollEvents::operator=(EpollEvents&& rhs)
-> EpollEvents&
{
    if (this==&rhs)
        return *this;

    mEventInfos = std::move(rhs.mEventInfos);
	mEventSize = rhs.mEventSize;
    mBeginIt = rhs.mBeginIt;
    mEndIt = rhs.mEndIt;

    return *this;
}

auto
EpollEvents::setSize(int event_max_size ) -> int
{
    mEventInfos = make_unique<struct epoll_event[]>(event_max_size) ;
	mEventSize = event_max_size;
    // mBeginIt = mEventInfos.get();
    // mEndIt   = mEventInfos.get()+eventMax;
    return 0;
}

auto
EpollEvents::setInvokedEventCnt(int eventedCnt ) -> int
{
    mBeginIt = mEventInfos.get();
    mEndIt   = mEventInfos.get()+eventedCnt;
    return 0;
}

auto
EpollEvents::getInstance() -> struct epoll_event*
{
    return mEventInfos.get();
}

auto
EpollEvents::getSize() -> const int&
{
    return mEventSize;
}


auto
EpollEvents::begin() -> event_iterator&
{
    return mBeginIt;
}

auto
EpollEvents::end() -> event_iterator&
{
    return mEndIt;
}

auto
EpollEvents::cbegin() -> const event_iterator&
{
    return mBeginIt;
}

auto
EpollEvents::cend() -> const event_iterator&
{
    return mEndIt;
}

}//namespace doori
