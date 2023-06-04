// Copyright 2019 The doori-project Authors. All rights reserved.
// Use of this source code is governed by a GPL-3.0 license that can be found in the LICENSE file.
// See the AUTHORS file for names of contributors.
//
// Created by doori on 19. 7. 25.
//
#pragma once

#include <thread>
#include <condition_variable>
#include "Topic.h"
#include "DataStructure/Tree.h"
#include "CommunicationMember/TcpApi.h"
#include "CommunicationMember/Socket.h"


namespace doori::Tnsd{

    enum struct FLOW_SIDE {
        PUB
        ,SUB
    };

    class MiddleSide : public Common::Error
    {
    public:

        MiddleSide() = delete;
        MiddleSide(const MiddleSide&) = delete;
        MiddleSide(MiddleSide&& rhs) = delete;
        MiddleSide& operator==(const MiddleSide&) = delete;
        MiddleSide& operator==(MiddleSide&&) = delete;
        virtual ~MiddleSide();

        /**
         * Tnsd에 접속정보, 자신의 유형의 대한 정보를 초기화합니다.
         * @param ip tnsd 목적지ip
         * @param port tnsd 목적지port
         * @param topic topic객체
         * @param side Pub 또는 Sub
         */
        MiddleSide(string ip, string port, Topic topic, FLOW_SIDE side);

        /**
         * MiddleSide 접속합니다.
         */
        void Connect();

        /**
         * MiddleSide Notify 프로토콜을 송신
         */
        void Notify();

        /**
         * Tnsd에 Anwser 프로토콜을 송신
         */
        void Anwser();

    protected:
        CommunicationMember::Socket mSocket;


    private:
        enum{ RETRY_MAX=20, TNSD_ALIVE_INTERVAL=10 };
        string& mTnsdIp;
        string& mTnsdPort;
        Topic& mTopic;
        FLOW_SIDE& mSide;
    };

}
