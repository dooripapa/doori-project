// Copyright 2019 The doori-project Authors. All rights reserved.
// Use of this source code is governed by a GPL-3.0 license that can be found in the LICENSE file.
// See the AUTHORS file for names of contributors.
//
// Created by doori on 19. 7. 25.
//
#pragma once
#include<memory>
#include <sys/epoll.h>

namespace doori{

class event_iterator:public std::iterator<std::input_iterator_tag,struct epoll_event>
{
public:
	event_iterator():p(nullptr){}
    event_iterator(struct epoll_event* pEpollEvent):p(pEpollEvent){}
    event_iterator(const event_iterator& EpollEvent) : p(EpollEvent.p){}
    event_iterator& operator++() {++p;return *this;}
    event_iterator operator++(int) {event_iterator temp(*this); operator++();return temp;}
    bool operator==(const event_iterator& rhs) const {return p==rhs.p;}
    bool operator!=(const event_iterator& rhs) const {return p!=rhs.p;}
    event_iterator& operator*() {return *this;}
    int getFd(){return p->data.fd;}
private:
    struct epoll_event *p;
};

class EpollEvents
{
public:
    EpollEvents(){}
    ~EpollEvents(){}

    auto operator=(EpollEvents&& rhs)-> EpollEvents&;
    auto setSize( int event_max_size ) -> int;
    auto setInvokedEventCnt( int eventedCnt ) -> int;
    auto getInstance() -> struct epoll_event*;
    auto getSize() -> const int&;
    auto begin() -> event_iterator& ;
    auto end() -> event_iterator& ;
    auto cbegin() -> const event_iterator& ;
    auto cend() -> const event_iterator& ;
protected:
private:
    std::unique_ptr<struct epoll_event[]> mEventInfos;
	int mEventSize;
    event_iterator  mBeginIt;
    event_iterator  mEndIt;
};


}
