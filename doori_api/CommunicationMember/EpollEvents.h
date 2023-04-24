// Copyright 2019 The doori-project Authors. All rights reserved.
// Use of this source code is governed by a GPL-3.0 license that can be found in the LICENSE file.
// See the AUTHORS file for names of contributors.
//
// Created by doori on 19. 7. 25.
//
#pragma once
#include<memory>
#include <sys/epoll.h>

namespace doori::CommunicationMember{

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
    auto getFd() const  -> int {return p->data.fd;} ;
private:
    struct epoll_event *p;
};

class EpollEvents
{
public:
    /**
     * 기본생성자
     */
    EpollEvents(){}
    /**
     * 기본소멸자
     */
    ~EpollEvents(){}

    /**
     * 이동대입연산자
     * @param rhs 
     * @return 자기자신의 참조값
     */
    auto operator=(EpollEvents&& rhs)-> EpollEvents&;
    /**
     * 이벤트 담을 수 있는 사이즈
     * @param event_max_size 
     * @return 
     */
    auto setSize( int event_max_size ) -> int;
    /**
     * 이벤트가 발생 됐을 때, 이벤트의 수
     * @param eventedCnt 
     * @return 
     */
    auto setInvokedEventCnt( int eventedCnt ) -> void;
    /**
     * 자산의 포인터를 리턴함
     * @return
     */
    auto get() -> struct epoll_event*;
    /**
     * 이벤트를 담을 수 있는 셋팅된 값 리턴
     * @return int
     * @note setSize() 호출할때 셋팅값이 리턴됨
     */
    auto getSize() -> int;
    /**
     * 이벤트가 발생됐을때,이벤트 순차적으로 접근 가능한 iterator 시작값
     * @return
     */
    auto begin() -> event_iterator& ;
    /**
     * 이벤트가 발생됐을때,이벤트 순차적으로 접근 가능한 iterator 마지막값
     * @return
     */
    auto end() -> event_iterator& ;
    /**
     * 이벤트가 발생됐을때,이벤트 순차적으로 접근 가능한 iterator 시작값
     * @return const event_iterator& 변경 불가능한 참조값
     */
    auto cbegin() -> const event_iterator& ;
    /**
     * 이벤트가 발생됐을때,이벤트 순차적으로 접근 가능한 iterator 마지막값
     * @return const event_iterator& 변경 불가능한 참조값
     */
    auto cend() -> const event_iterator& ;
private:
    std::unique_ptr<struct epoll_event[]> mEventInfos;
	int mEventSize;
    event_iterator  mBeginIt;
    event_iterator  mEndIt;
};


}
