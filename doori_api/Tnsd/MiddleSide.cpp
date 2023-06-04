// Copyright 2019 The doori-project Authors. All rights reserved.
// Use of this source code is governed by a GPL-3.0 license that can be found in the LICENSE file.
// See the AUTHORS file for names of contributors.
//
// Created by doori on 19. 7. 25.
//
#include "MiddleSide.h"

namespace doori::Tnsd{

    auto MiddleSide::Init(string ip, string port, Topic topic, FLOW_SIDE myType) noexcept -> void {
        auto retryCnt = 0;
        mICommTopic = topic;
        LOG(INFO, "MiddleSide MiddleSide TopicAccess : ", mICommTopic.getTopicName());

        do{
            if((mTnsdSocket = CommunicationMember::TcpApi::Connect(ip, port, 10)) < 0) {
                LOG(ERROR, "failed to connect to MiddleSide", ip, ":", port);
                std::this_thread::sleep_for(std::chrono::seconds(5));
                if( ++retryCnt > RETRY_MAX ) {
                    LOG(ERROR,"To retry is for MiddleSide is limited.");
                    return false;
                }
                continue;
            } else
                break;
        }while(1);

        mMyPubSubType = myType;

        // Tnsd에게 Alive 프로토콜을 interval 간격으로 보낸다.
        mBackgroundFuncs.push_back( std::thread(&MiddleSide::processingAliveService, this) );

        return true;
    }

    auto MiddleSide::sendNotifyProtocolToTnsd() noexcept -> bool{
        Protocol_backup sendProtcol, recvProtocol;
        sendProtcol.asSender();
        recvProtocol.asResponser();
        sendProtcol.asNotify(mMyPubSubType,mICommTopic, mMultiSessions.GetListener().From().Address() );

        if(!sendProtocolToTnsd(sendProtcol, recvProtocol))
        {
            LOG(ERROR, "failed to communicate TNSD");
            return false;
        }

        if(recvProtocol.MsgCode() == Protocol_backup::STATUS_CODE::ERR ){
            LOG(INFO, "FAIL Message : "," recvProtocol.MsgComment()");
            return  false;
        }

        //
        // MiddleSide 와 정상적으로 통신이 된다면, MiddleSide 데이터를 처리할 콜백함수를 등록한다.
        // 단, doori::WATCHER TYPE은 ONLY RECEIVER 으로 수신만 처리한다.
        // SendWatchers() 함수 호출시 Tnsd쪽으로 데이터가 안 가도록 하기 doori::WATCHER 타입설정.
        //
        std::function<int(int,string&)> processTnsdData(std::bind(&MiddleSide::processingTnsdData , this, std::placeholders::_1 , std::placeholders::_2));
        mMultiSessions.AddUniqueWatcher(mTnsdSocket, CommunicationMember::WATCHER::TYPE::RECEIVER, processTnsdData) ;

        return true;
    }

    auto MiddleSide::processingMultisessions(int socket) noexcept -> bool {

        CommunicationMember::EpollEvents eventContainer;
        if(mMultiSessions.Init(socket) < 0)
        {
            LOG(ERROR, "failed to Init Multi-Session");
            return false;
        }
        eventContainer.setSize(10);
        mMultiSessions.MoveEventContainer(std::move(eventContainer));

        mBackgroundFuncs.push_back( std::thread( &CommunicationMember::Epoll::RunningEventDelegateMethod, &mMultiSessions) );
        return true;
    }

    auto MiddleSide::sendProtocolToTnsd(Protocol_backup &sendProtocol, Protocol_backup &responseProtocol) noexcept -> bool{
        Stream sendStream, recvStream;
        Data   answerData;

        sendStream = sendProtocol.toData();
        LOG(DEBUG, "MiddleSide <<-- ::", sendStream.getString());
        if( mTnsdConnection.reply( sendStream, recvStream ) < 0 ) {
            LOG(ERROR, "TNSD Connection error");
            return false;
        }
        LOG(DEBUG, "MiddleSide -->> ::", recvStream.getString());
        answerData.fromString(recvStream.getString());
        responseProtocol.fromData(answerData) ;
        return true;
    }

    ///@brief TNSD에 protocol객체를 보낸다
    auto MiddleSide::sendProtocolToTnsd(Protocol_backup &sendProtocol) noexcept -> bool{
        Stream sendStream;
        Data   answerData;

        sendStream = sendProtocol.toData();
        LOG(DEBUG, "MiddleSide <<-- ::", sendStream.getString());
        if( mTnsdConnection.send( sendStream ) < 0 ) {
            LOG(ERROR, "TNSD Connection error");
            return false;
        }
        return true;
    }

    ///@brief TNSD에게 살아 있음을 계속적으로 알린다.
    ///@todo 상태정보 동기화 작업.
    auto MiddleSide::sendAliveProtocolToTnsd(Topic topic) noexcept -> bool {
        Protocol_backup sendProtcol;
        sendProtcol.asSender();
        sendProtcol.asAlive(mMyPubSubType, topic, mMultiSessions.GetListener().From().Address() );

        if(!sendProtocolToTnsd(sendProtcol))
        {
            LOG(ERROR, "failed to communicate TNSD");
            return false;
        }
        return true;
    }

    auto MiddleSide::sendListProtocolToTnsd() noexcept -> bool {
        Protocol_backup sendProtcol, recvProtocol;
        sendProtcol.asSender();
        recvProtocol.asResponser();

        ////////////////////////////////////
        // Sub 이면 pub의 정보를
        // Pub 이면 sub의 정보를
        ////////////////////////////////////
        switch(mMyPubSubType)
        {
            case Protocol_backup::TREE::PUB:
                sendProtcol.asList(Protocol_backup::TREE::SUB , mICommTopic);
                break;
            case Protocol_backup::TREE::SUB:
                sendProtcol.asList(Protocol_backup::TREE::PUB , mICommTopic );
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

        if(recvProtocol.MsgCode() == Protocol_backup::STATUS_CODE::ERR )
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

                auto ipIter = addrData.find_if_Fid(static_cast<int>(Protocol_backup::SEQ::IP));
                auto portIter = addrData.find_if_Fid(ipIter, static_cast<int>(Protocol_backup::SEQ::PORT));

                LOG(DEBUG, "ip : ", (*ipIter).getValueToString(), ", port : ", (*portIter).getValueToString());

                switch(mMyPubSubType)
                {
                    case Protocol_backup::TREE::PUB:
                        //Nothing to do.
                        break;
                    case Protocol_backup::TREE::SUB:
                        conn.setTo( doori::Endpoint({(*ipIter).getValueToString(), (*portIter).getValueToString()}) );
                        auto socketFd = conn.connectTo();
                        if ( socketFd > 0 )
                            mMultiSessions.AddWatcher(socketFd);
                        else
                            LOG(FATAL, "fail to connect To PubSide ip:", (*ipIter).getValueToString(), ":",(*portIter).getValueToString());
                        break;
                }
            }
        } else
            LOG(INFO, "Knock member is nobody" );
        return true;
    }

    MiddleSide::~MiddleSide() {
        LOG(INFO, "Connection doRelease.");
        mTnsdConnection.release();
        mMultiSessions.GetListener().release();
    }

    ///@brief
    auto MiddleSide::processingTnsdData(int sock, Stream& stream) noexcept -> int{
        auto ret = 0;
        Data data;
        Protocol_backup protocol;

        LOG(INFO, "MiddleSide -->> ::", stream.toByteStream() );
        if ( data.fromString( stream.getString() ) == -1 )
        {
            LOG(ERROR, "unserialize error, check formated" );
            return -1;
        }

        if (protocol.fromData(data)<0)
        {
            LOG(ERROR, "protocol Init error");
            return -1;
        }

        switch(protocol.MsgType())
        {
            case Protocol_backup::TYPE::NOTIFY:
                LOG(DEBUG, Protocol_backup::NOTIFY_MSG);
                break;
            case Protocol_backup::TYPE::CHANGE:
                LOG(DEBUG, Protocol_backup::CHANGE_MSG);
                if(mMyPubSubType == Protocol_backup::TREE::SUB)
                    if(!sendListProtocolToTnsd())
                        ret = -1;
                break;
            case Protocol_backup::TYPE::ALIVE:
                LOG(DEBUG, Protocol_backup::ALIVE_MSG);
                break;
            case Protocol_backup::TYPE::REPORT:
                LOG(DEBUG, Protocol_backup::REPORT_MSG);
                break;
            case Protocol_backup::TYPE::LIST:
                LOG(DEBUG, Protocol_backup::LIST_MSG);
                break;
            default:
                ret = -1;
                LOG(ERROR, Protocol_backup::UNKOWN_MSG);
        }
        return ret;
    }

    auto MiddleSide::processingAliveService() noexcept -> bool {

        ///continuely send ALIVE protocol.
        while(1)
        {
            std::this_thread::sleep_for(std::chrono::seconds(TNSD_ALIVE_INTERVAL));
            sendAliveProtocolToTnsd(mICommTopic);
        }
    }

    MiddleSide::MiddleSide(std::function<int(int, string&)> delegation) : mMultiSessions(delegation)
    {
    }

    MiddleSide::MiddleSide(MiddleSide&& rhs) : mMultiSessions(std::move(rhs.mMultiSessions)) {
        mTnsdSocket        =std::move(rhs.mTnsdSocket);
        mTnsdConnection    =std::move(rhs.mTnsdConnection);
        mICommTopic        =std::move(rhs.mICommTopic);
        mMyPubSubType      =std::move(rhs.mMyPubSubType);
        mBackgroundFuncs   =std::move(rhs.mBackgroundFuncs);
    }

    auto MiddleSide::operator=(MiddleSide &&rhs) noexcept -> MiddleSide & {
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

    auto MiddleSide::sendStreamToMultisessions(const string &stream) noexcept -> bool {
        return mMultiSessions.SendWatchers(stream);
    }

    MiddleSide::MiddleSide(string ip, string port, Topic topic, FLOW_SIDE side) : mTnsdIp{ip}, mTnsdPort{port}, mTopic{topic}, mSide{side}{

    }

}//namespace doori
