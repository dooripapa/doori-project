// Copyright 2019 The doori-project Authors. All rights reserved.
// Use of this source code is governed by a GPL-3.0 license that can be found in the LICENSE file.
// See the AUTHORS file for names of contributors.
//
// Created by doori on 19. 7. 25.
//
#pragma once

#include <thread>
#include <condition_variable>
#include "Endpoint.h"
#include "Topic.h"
#include "Protocol.h"
#include "Connection.h"
#include "Epoll.h"
#include "Tree.h"
#include "Topic.h"

namespace doori{

///@class TNSD와 통신하는 interface class 객체
class ICommunication
{
public:
    ICommunication() = delete;
    ICommunication(const ICommunication&) = delete;
    explicit ICommunication(std::function<int(int, Stream &)> delegation);
    ICommunication(ICommunication&& rhs);
    virtual ~ICommunication();
    auto init(doori::Connection forTnsd, Topic topic, Protocol::TREE myType) noexcept -> bool;
    auto sendNotifyProtocolToTnsd() noexcept -> bool;
    auto sendListProtocolToTnsd() noexcept -> bool;
    auto operator=(const ICommunication& rhs) noexcept = delete;
    auto operator=(ICommunication&& rhs) noexcept -> ICommunication&;
protected:
    auto sendProtocolToTnsd(Protocol &, Protocol &) noexcept -> bool;
    auto sendProtocolToTnsd(Protocol &) noexcept -> bool;
    auto sendAliveProtocolToTnsd(Topic topic) noexcept -> bool;
    auto processingAliveService() noexcept -> bool;
    auto processingMultisessions(doori::Connection forSub) noexcept -> bool;
    auto processingTnsdData(int sock, Stream& stream) noexcept -> int;
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
