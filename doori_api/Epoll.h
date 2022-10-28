// Copyright 2019 The doori-project Authors. All rights reserved.
// Use of this source code is governed by a GPL-3.0 license that can be found in the LICENSE file.
// See the AUTHORS file for names of contributors.
//
// Created by doori on 19. 7. 25.
//
#pragma once
#include <iostream>
#include <functional>
#include <tuple>
#include "Connection.h"
#include "Stream.h"
#include "EpollEvents.h"

namespace doori{
struct WATCHER{
    enum class TYPE{
        DESIGN,   //지정된 방식
        RECEIVER  //only 수신처리만
    };
};

/**
 * Epoll 객체는 복사생성자(복사대입) 호출하지 못하도록 한다.
 */
class Epoll
{
public:
    /**
     * Epoll 초기화.
     * 엄격한 함수포인터 적용하여, 잠재적인(컴파일러 추론) 변경을 막는다.
     * @param delegation : const std::function< int(int, Stream&) >&
     */
    explicit Epoll(const std::function<int(int, Stream&)>& delegation);
    /**
     * 이동생성자
     * @note 이동생성자만 허용
     * @param rhs
     */
    Epoll(Epoll&& rhs);
    /**
     * 복사생성자 금지
     */
    Epoll(const Epoll&) = delete;
    /**
     * 소멸자
     */
    ~Epoll();
    /**
     * Epoll 통신단 초기화
     * @param conn : Connection 객체
     * @return int : -1 : 실패, 0 성공
     */
    auto init(Connection conn ) -> int;
    /**
     * 인자값(file descriptor)이 listening 소켓인지 확인
     * @param socket_fd : socket
     * @return bool : true listening socket, false no listening socket
     */
    auto isListener( int socket_fd ) const -> bool;
    /**
     * 인자값(file descriptor)을 Epoll에 등록, 해당 인자값에 이벤트가 발생되면 감지된다.
     * 이후, delegation에 이벤트 처리하도록 자동호출되게 됨.
     * @param socket_fd : file descriptor
     * @return int : -1 실패, 0 성공
     */
    auto addWatcher( int socket_fd ) -> int;
    /**
     * 인자값(file descriptor)을 Epoll에 등록, 해당 인자값에 이벤트가 발생되면 감지된다.
     * 이후, 이벤트가 발생되면, 등록한 delegation에서 처리하도록 함
     * @param socket_fd : file descriptor
     * @param type : WATCHER::TYPE
     * @param delegation : 첫번째 아규먼트 file descriptor 전용 이벤트 처리위임자
     * @return int : -1 실패, 0 성공
     */
    auto addUniqueWatcher(int socket_fd, WATCHER::TYPE type, const std::function<int(int, Stream&)>& delegation ) -> int;
    /**
     * 이벤트가 발생하면, 그 이벤트가 연결 요청일 경우(listening socket) 자동으로 Epoll에 등록함
     * @return int : -1 실패, >0 자동으로 Epoll에 등록되는 file descriptor
     */
    auto addWatcherAsConn() -> int;
    /**
     * Epoll 초기화 되었다면, 적절하게 자동으로 처리됨.
     * @note 이 함수에서 호출되면, 블록킹 모드로 전환되고, 자동으로 처리됨
     * @return
     */
    [[noreturn]] auto runningEventDelegateMethod() -> int;
    /**
     * Epoll의 event container 초기화
     * @param event_container : EpollEvents 객체
     */
    auto moveEventContainer(EpollEvents&& event_container ) noexcept -> void;
    /**
     * Epoll에 event 발생할때까지 대기
     * @param event_container : EpollEvents 객체
     * @param timeout : event 발생할때까지 timeout
     * @return int : trigger 된 event 수
     */
    auto waitForEvents(EpollEvents& event_container, const int& timeout ) const -> int;
    /**
     * 해당 file descriptor에 데이터 수신 후, delegation 위임하여, 처리하도록 함
     * @param socket_fd : file descriptor
     * @param delegation : const std::function<int(int socket_fd, Stream& stream)>&
     * @return int : -2, connection lost,  delegation 리턴값
     */
    auto executeTask( int socket_fd, const std::function<int(int socket_fd, Stream& stream)>& delegation ) -> int;
    /**
     * 해당 file descriptor에 데이터 수신 후, Epoll 초기화할때 등록된 delegation에 위임하여, 처리하도록 함
     * @param socket_fd : file descriptor
     * @return int : -2, connection lost, Epoll 초기화할때 등록된 delegation 리턴값
     */
    auto executeTask( int socket_fd ) -> int;
    /**
     * Epoll 등록된 Connection 리턴함
     * @note 리턴값이 Connection 참조값이므로, 외부에서 조작시, Epoll 성질도 변경된다.
     * @return Connection 참조값
     */
    auto getListener() noexcept -> Connection&;
    /**
     * Epoll 등록된 모든 file descriptor들에게 Stream 객체를 송신
     * @param stream : Stream 객체
     * @note Epoll에 WATCHER::TYPE이 RECEIVER 이면 송신하지 않음
     * @return bool : true 전체 file descriptor 송신성공, false : 송신실패
     */
    auto sendWatchers( Stream stream ) noexcept -> bool;
    /**
     * 복사대입연자 금지
     */
    auto operator=(const Epoll& rhs) noexcept -> Epoll& = delete;
    /**
     * 이동대입연산자만 허용
     * @param rhs : Epoll&&
     * @return 자기자신의 참조값
     */
    auto operator=(Epoll&& rhs) noexcept -> Epoll& ;
protected:
private:
    std::unordered_map<int, pair<WATCHER::TYPE ,std::function<int(int,Stream&)>> > mConnectedSocketList;
    EpollEvents mEventContainer;
    Connection mEpollRootConnection;
    std::function<int(int,Stream&)> mEventDelegateMethod;
    int mEpoll_fd=0;
    int mListen_socket=0;
    auto moveFrom(Epoll&& rhs) noexcept -> void;
};

}//namespace doori
