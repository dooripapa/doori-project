// Copyright 2019 The doori-project Authors. All rights reserved.
// Use of this source code is governed by a GPL-3.0 license that can be found in the LICENSE file.
// See the AUTHORS file for names of contributors.
//
// Created by doori on 19. 7. 25.
//
#pragma once

#include <thread>
#include <condition_variable>
#include "CommunicationMember/Endpoint.h"
#include "Topic.h"
#include "Protocol.h"
#include "CommunicationMember/Connection.h"
#include "CommunicationMember/Epoll.h"
#include "DataStructure/Tree.h"
#include "Topic.h"

namespace doori{

class ICommunication
{
public:
    /**
     * 기본생성자 호출금지
     */
    ICommunication() = delete;
    /**
     * 복사생성자 호출금지
     */
    ICommunication(const ICommunication&) = delete;
    /**
     * 생성자 함수
     * @param delegation : std::function<int(int, Stream &)> 데이터수신 처리를 위임할 함수
     */
    explicit ICommunication(std::function<int(int, Stream &)> delegation);
    /**
     * 이동생성자 함수
     * @param rhs
     */
    ICommunication(ICommunication&& rhs);
    /**
     * 소멸자
     */
    virtual ~ICommunication();
    /**
     * 초기화함수
     * @param forTnsd Connection 객체, Tnsd의 정보를 담고 있는 객체
     * @param topic Topic객체, Tnsd에 받고자 하는 메시지형
     * @param myType Tnsd에 연결할때, 자신이 Pub인지 Sub인지 결정하는 타입
     * @warning 에러체크는 필수.
     * @note 이 함수가 호출되면, Tnsd 접속하기전까지. Hang이 걸린다.
     * @return bool true 성공, false 실패
     */
    auto init(Connection forTnsd, Topic topic, Protocol::TREE myType) noexcept -> bool;

    /**
     * 관심이 있는 topic을 Tnsd에 Notify 한다.
     * @todo duplicated topic processing
     * @note Notify한 순간, 어떤 곳(Sub)으로부터 연결을 수신할 수 있으므로, 이 함수가 호출하기전에 연결요청 수신한 가능한 상태여야 한다.
     */
    auto sendNotifyProtocolToTnsd() noexcept -> bool;
    /**
     * Topic정보를 통해 Topic메시지를 접속하고자 하는 정보를 얻거나, 대상을 확인한다.
     * @see Pub이면, Sub의 정보를 , Sub이면 pub의 정보를 가져온다.
     * @return bool true 성공, false 실패
     */
    auto sendListProtocolToTnsd() noexcept -> bool;
    /**
     * 복사대입연산자 호출금지
     * @param rhs const ICommunication&
     * @return
     */
    auto operator=(const ICommunication& rhs) noexcept = delete;
    /**
     * 이동대입연사자
     * @param rhs ICommunication&&
     * @return 자기자산의 참조값
     */
    auto operator=(ICommunication&& rhs) noexcept -> ICommunication&;
protected:
     /**
      * 프로토콜의 보내면, 해당하는 프로토콜의 응답을 받는다.
      * @param sendProtocol : Protocol& 보낼 프로토콜메시지
      * @param responseProtocol : Protocol& 받을 프로토콜메시지 컨테이너
      * @return bool true 성공, false 실패
      */
    auto sendProtocolToTnsd(Protocol &sendProtocol, Protocol &responseProtocol) noexcept -> bool;
    /**
     * Tnsd 프로토콜메시지를 보낸다.
     * @param sendProtocol Protocol&
     * @return bool true 성공, false 실패
     */
    auto sendProtocolToTnsd(Protocol& sendProtocol) noexcept -> bool;
    /**
     * Tnsd에게 alive protocol메시지를 보낸다
     * @param topic Topic객체
     * @return bool true 성공, false 실패
     */
    auto sendAliveProtocolToTnsd(Topic topic) noexcept -> bool;
    /**
     * background 방식으로 TNSD에게 계속적으로 alive protocol 송신하는 함수
     * @return bool true 성공, false 실패
     */
    auto processingAliveService() noexcept -> bool;
    /**
     * Multi session으로부터 데이터를 수신처리 한다.
     * @param forSub Connection 객체
     * @return bool true 성공, false 실패
     */
    auto processingMultisessions(Connection forSub) noexcept -> bool;
    /**
     * 여기서 TNSD로 부터 들어온 데이터를 처리한다. list, change, alive 프로토콜 처리
     * @param sock 소켓
     * @param stream Stream
     * @return -1 : fail
     */
    auto processingTnsdData(int sock, Stream& stream) noexcept -> int;
    /**
     * epoll로 연결되어 있는 섹션에 역으로 데이터를 전체 송신
     * @param stream
     * @return bool true 전체송신 성공, false 하나라도 실패
     */
    auto sendStreamToMultisessions(const Stream& stream) noexcept -> bool;
private:
    enum{ RETRY_MAX=20, TNSD_ALIVE_INTERVAL=10 };
    int                         mTnsdSocket;
    Protocol::TREE              mMyPubSubType;
    doori::Connection           mTnsdConnection;
    doori::Epoll                mMultiSessions;
    Topic                 mICommTopic;
    std::vector<std::thread>    mBackgroundFuncs;
};

}
