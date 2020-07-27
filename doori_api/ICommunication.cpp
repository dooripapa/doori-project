// Copyright 2019 The doori-project Authors. All rights reserved.
// Use of this source code is governed by a GPL-3.0 license that can be found in the LICENSE file.
// See the AUTHORS file for names of contributors.
//
// Created by doori on 19. 7. 25.
//
#include "ICommunication.h"

namespace doori{

auto ICommunication::init(doori::Connection forTnsd , Topic topic, Protocol::TREE myType) noexcept -> bool{
    auto retryCnt = 0;
    mICommTopic = topic;
    LOG(INFO, "Tnsd Communication TopicAccess : ", mICommTopic.getTopicName());
    mTnsdConnection = std::move(forTnsd);
    do{
        if( (mTnsdSocket = mTnsdConnection.connectTo())<0) {
            LOG(ERROR, "failed to connect to Tnsd", mTnsdConnection.To().Address().Ip(), ":",
                mTnsdConnection.To().Address().Port());
            std::this_thread::sleep_for(std::chrono::seconds(5));
            if( ++retryCnt > RETRY_MAX ) {
                LOG(ERROR,"To retry is for Tnsd is limited.");
                return false;
            }
            continue;
        } else
            break;
    }while(1);

    mMyPubSubType = myType;

    // Tnsd에게 Alive 프로토콜을 interval 간격으로 보낸다.
    mBackgroundFuncs.push_back( std::thread(&ICommunication::processingAliveService, this) );

    return true;
}

auto ICommunication::sendNotifyProtocolToTnsd() noexcept -> bool{
    Protocol sendProtcol, recvProtocol;
    sendProtcol.asSender();
    recvProtocol.asResponser();
    sendProtcol.asNotify(mMyPubSubType,mICommTopic, mMultiSessions.getListener().From().Address() );

    if(!sendProtocolToTnsd(sendProtcol, recvProtocol))
    {
        LOG(ERROR, "failed to communicate TNSD");
        return false;
    }

    if(recvProtocol.MsgCode() == Protocol::STATUS_CODE::ERR ){
        LOG(INFO, "FAIL Message : ", recvProtocol.MsgComment() );
        return  false;
    }

    //
    // Tnsd 와 정상적으로 통신이 된다면, Tnsd 데이터를 처리할 콜백함수를 등록한다.
    // 단, doori::WATCHER TYPE은 ONLY RECEIVER 으로 수신만 처리한다.
    // sendWatchers() 함수 호출시 Tnsd쪽으로 데이터가 안 가도록 하기 doori::WATCHER 타입설정.
    //
    std::function<int(int,Stream&)> processTnsdData(std::bind(&ICommunication::processingTnsdData , this, std::placeholders::_1 , std::placeholders::_2));
    mMultiSessions.addUniqueWatcher(mTnsdSocket, doori::WATCHER::TYPE::RECEIVER, processTnsdData) ;

    return true;
}

auto ICommunication::processingMultisessions(doori::Connection forSub) noexcept -> bool {

    EpollEvents eventContainer;
    if(mMultiSessions.init(forSub)<0)
    {
        LOG(ERROR, "failed to init Multi-Session");
        return false;
    }
    eventContainer.setSize(10);
    mMultiSessions.moveEventContainer(std::move(eventContainer));

    mBackgroundFuncs.push_back( std::thread( &doori::Epoll::runningEventDelegateMethod, &mMultiSessions) );
    return true;
}

auto ICommunication::sendProtocolToTnsd(Protocol &sendProtocol, Protocol &responseProtocol) noexcept -> bool{
    Stream sendStream, recvStream;
    Data   answerData;

    sendStream = sendProtocol.toData();
    LOG(DEBUG, "Tnsd <<-- ::", sendStream.getString());
    if( mTnsdConnection.reply( sendStream, recvStream ) < 0 ) {
        LOG(ERROR, "TNSD Connection error");
        return false;
    }
    LOG(DEBUG, "Tnsd -->> ::", recvStream.getString());
    answerData.fromString(recvStream.getString());
    responseProtocol.fromData(answerData) ;
    return true;
}

///@brief TNSD에 protocol객체를 보낸다
auto ICommunication::sendProtocolToTnsd(Protocol &sendProtocol) noexcept -> bool{
    Stream sendStream;
    Data   answerData;

    sendStream = sendProtocol.toData();
    LOG(DEBUG, "Tnsd <<-- ::", sendStream.getString());
    if( mTnsdConnection.send( sendStream ) < 0 ) {
        LOG(ERROR, "TNSD Connection error");
        return false;
    }
    return true;
}

///@brief TNSD에게 살아 있음을 계속적으로 알린다.
///@todo 상태정보 동기화 작업.
auto ICommunication::sendAliveProtocolToTnsd(Topic topic) noexcept -> bool {
    Protocol sendProtcol;
    sendProtcol.asSender();
    sendProtcol.asAlive(mMyPubSubType, topic, mMultiSessions.getListener().From().Address() );

    if(!sendProtocolToTnsd(sendProtcol))
    {
        LOG(ERROR, "failed to communicate TNSD");
        return false;
    }
    return true;
}

auto ICommunication::sendListProtocolToTnsd() noexcept -> bool {
    Protocol sendProtcol, recvProtocol;
    sendProtcol.asSender();
    recvProtocol.asResponser();

    ////////////////////////////////////
    // Sub 이면 pub의 정보를
    // Pub 이면 sub의 정보를
    ////////////////////////////////////
    switch(mMyPubSubType)
    {
        case Protocol::TREE::PUB:
            sendProtcol.asList(Protocol::TREE::SUB ,mICommTopic);
            break;
        case Protocol::TREE::SUB:
            sendProtcol.asList(Protocol::TREE::PUB ,mICommTopic );
            break;
        default:
            LOG(FATAL, "My (PUB|SUB) not defined");
            return false;
    }

    if(!sendProtocolToTnsd(sendProtcol, recvProtocol))
    {
        LOG(ERROR, "failed to communicate TNSD");
        return false;
    }

    if(recvProtocol.MsgCode() == Protocol::STATUS_CODE::ERR )
    {
        LOG(INFO, "FAIL Message : ", recvProtocol.MsgComment() );
    }

    Data addrData;
    int idx=-1000;
    int CntMemberMax = recvProtocol.ArraySum();
    if( CntMemberMax > 0 )
    {
        auto it = recvProtocol.toData().find_if_Fid(idx);
        for(int i=0;i<CntMemberMax;i++)
        {
            doori::Connection conn;
            addrData.clear();
            addrData.fromString( (*it).getValueToString() );

            auto ipIter = addrData.find_if_Fid(static_cast<int>(Protocol::SEQ::IP));
            auto portIter = addrData.find_if_Fid(ipIter, static_cast<int>(Protocol::SEQ::PORT));

            LOG(DEBUG, "ip : ", (*ipIter).getValueToString(), ", port : ", (*portIter).getValueToString());

            switch(mMyPubSubType)
            {
                case Protocol::TREE::PUB:
                    //Nothing to do.
                    break;
                case Protocol::TREE::SUB:
                    conn.setTo( doori::Endpoint({(*ipIter).getValueToString(), (*portIter).getValueToString()}) );
                    auto socketFd = conn.connectTo();
                    if ( socketFd > 0 )
                        mMultiSessions.addWatcher(socketFd);
                    else
                        LOG(FATAL, "fail to connect To Publisher ip:", (*ipIter).getValueToString(), ":",(*portIter).getValueToString());
                    break;
            }
        }
    } else
        LOG(INFO, "Knock member is nobody" );
    return true;
}

ICommunication::~ICommunication() {
    LOG(INFO, "Connection doRelease.");
    mTnsdConnection.release();
    mMultiSessions.getListener().release();
}

///@brief
auto ICommunication::processingTnsdData(int sock, Stream& stream) noexcept -> int{
    auto ret = 0;
    Data data;
    Protocol protocol;

    LOG(INFO, "Tnsd -->> ::", stream.toByteStream() );
    if ( data.fromString( stream.getString() ) == -1 )
    {
        LOG(ERROR, "unserialize error, check formated" );
        return -1;
    }

    if (protocol.fromData(data)<0)
    {
        LOG(ERROR, "protocol init error");
        return -1;
    }

    switch(protocol.MsgType())
    {
        case Protocol::TYPE::NOTIFY:
            LOG(DEBUG, Protocol::NOTIFY_MSG);
            break;
        case Protocol::TYPE::CHANGE:
            LOG(DEBUG, Protocol::CHANGE_MSG);
            if(mMyPubSubType == Protocol::TREE::SUB)
                if(!sendListProtocolToTnsd())
                    ret = -1;
            break;
        case Protocol::TYPE::ALIVE:
            LOG(DEBUG, Protocol::ALIVE_MSG);
            break;
        case Protocol::TYPE::REPORT:
            LOG(DEBUG, Protocol::REPORT_MSG);
            break;
        case Protocol::TYPE::LIST:
            LOG(DEBUG, Protocol::LIST_MSG);
            break;
        default:
            ret = -1;
            LOG(ERROR, Protocol::UNKOWN_MSG);
    }
    return ret;
}

auto ICommunication::processingAliveService() noexcept -> bool {

    ///continuely send ALIVE protocol.
    while(1)
    {
        std::this_thread::sleep_for(std::chrono::seconds(TNSD_ALIVE_INTERVAL));
        sendAliveProtocolToTnsd(mICommTopic);
    }
}

ICommunication::ICommunication(std::function<int(int, Stream &)> delegation) : mMultiSessions(delegation)
{
}

ICommunication::ICommunication(ICommunication&& rhs) : mMultiSessions(std::move(rhs.mMultiSessions)) {
    mTnsdSocket        =std::move(rhs.mTnsdSocket);
    mTnsdConnection    =std::move(rhs.mTnsdConnection);
    mICommTopic        =std::move(rhs.mICommTopic);
    mMyPubSubType      =std::move(rhs.mMyPubSubType);
    mBackgroundFuncs   =std::move(rhs.mBackgroundFuncs);
}

auto ICommunication::operator=(ICommunication &&rhs) noexcept -> ICommunication & {
    if(this==&rhs)
        return *this;

    mTnsdSocket        =std::move(rhs.mTnsdSocket);
    mTnsdConnection    =std::move(rhs.mTnsdConnection);
    mICommTopic        =std::move(rhs.mICommTopic);
    mMyPubSubType      =std::move(rhs.mMyPubSubType);
    mBackgroundFuncs   =std::move(rhs.mBackgroundFuncs);
    mMultiSessions     =std::move(rhs.mMultiSessions);

    return *this;
}

auto ICommunication::sendStreamToMultisessions(const Stream &stream) noexcept -> bool {
    return mMultiSessions.sendWatchers(stream);
}

}//namespace doori
