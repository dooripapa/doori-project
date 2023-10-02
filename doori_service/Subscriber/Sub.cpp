//
// Created by doori on 19. 10. 10..
//

#include "Sub.h"

#include <utility>

namespace doori::service::Subscriber{

    auto Sub::operator()() noexcept -> int {

        api::Communication::Socket socket{};

        api::Communication::TcpApi tcpApi{socket};

        tcpApi.InitEndpoint();
        if(!tcpApi.Status())
        {
            LOG(ERROR, "fail to initialize Socket{}");
            return -1;
        }

        auto bindingIp = mSubDic.Value(Sub::SUB_IP);
        auto bindingPort = mSubDic.Value(Sub::SUB_PORT);

        tcpApi.SetReuseOpt(bindingIp, bindingPort);
        if(!tcpApi.Status()) {
            LOG(ERROR, "fail to SetReuseOpt");
            return -1;
        }

        tcpApi.SetTimeoutOpt(5);
        if(!tcpApi.Status()) {
            LOG(ERROR, "fail to SetTimeoutOpt");
            return -1;
        }

        tcpApi.Bind(bindingIp, bindingPort);
        if(!tcpApi.Status()) {
            LOG(ERROR, "fail to Bind");
            return -1;
        }

        tcpApi.Listen(10);
        if(!tcpApi.Status()) {
            LOG(ERROR, "fail to Listen");
            return -1;
        }

        auto listenSocket = tcpApi.GetSocket();

        api::Communication::EpollApi epollApi{listenSocket };

        std::function<int(api::Communication::Socket)> process = std::bind(&Sub::processMessage, this,
                                                                           std::placeholders::_1);

        epollApi.InitEpoll(process);
        if(!epollApi.Status()) {
            LOG(ERROR, "fail to InitEpoll()");
            return -1;
        }

        //for Subscriber
        epollApi.RunningBackground(10, 10);

        //Tnsd 연결요청
        if(connectTnsd() < 0){
            LOG(ERROR,"Error");
            return -1;
        }

        //Tnsd Notify Protocol 송신
        if( sendNotifyProtocol() < 0) {
            LOG(ERROR, "failed to send NOTIFY-PROTOCOL");
            return -1;
        }

        //Tnsd 연결요청을 Epoll List에 등록
        epollApi.AddFdIntoEpollList(mTnsdSocket, process);

        //Epoll background 종료될때까지 대기.
        epollApi.JoinBackground();

        return 0;
    }

    auto Sub::clone() const noexcept -> std::unique_ptr<api::Process::Application> {
        return std::make_unique<Sub>(*this);
    }

    auto Sub::ProcessName() noexcept -> std::string {
        return "Sub";
    }

    auto Sub::Daemonize() noexcept -> bool {
        return false;
    }

    auto Sub::LogFile() noexcept -> std::string {
        if(!mSubDic.Value( Sub::LOG_NAME).empty() && !mSubDic.Value( Sub::LOG_PATH).empty() ) {
            return (mSubDic.Value( Sub::LOG_PATH ) + mSubDic.Value( Sub::LOG_NAME ));
        } else
            return api::Process::Application::LogFile();
    }

    auto Sub::LogLevel() noexcept -> api::Common::Log::LEVEL {
        if(!mSubDic.Value(Sub::LOG_LEVEL).empty()) {
            return api::Common::Log::convertToLevel(mSubDic.Value(Sub::LOG_LEVEL));
        }
        else {
            return api::Process::Application::LogLevel();
        }
    }

    Sub::Sub(api::Data::Dictionary dic) : mSubDic(std::move(dic)){
    }

    auto Sub::Terminate() noexcept -> int {
        LOG(INFO, "Pub is terminated");
        return 0;
    }

    auto Sub::processMessage(api::Communication::Socket socket) -> int {

        Tnsd::Header header;
        Tnsd::Body<Data::Json> body;

        //StreamTemplate Header, Body로 구성됨.
        Stream::StreamTemplate< api::Tnsd::Header, api::Tnsd::Body<api::Data::Json> > streamTemplate{header, body};

        string container;

        socket.Recv(container, 8);

        auto tilDataSize = stoi(container, 0, 10 );

        socket.Recv(container, tilDataSize);

        streamTemplate.FromStream(container);

        auto json = body.GetBody();

        switch(header.GetProtocol())
        {
            case api::Tnsd::PROTOCOL::NOTIFY:
                LOG(INFO,"Notify");
                break;
            case api::Tnsd::PROTOCOL::ANWSER:
                LOG(INFO,"Anwser");
                break;
            case api::Tnsd::PROTOCOL::CHANGE:
                LOG(INFO,"Change");
                break;
            case api::Tnsd::PROTOCOL::ALIVE:
                LOG(INFO,"Alive");
                break;
            case api::Tnsd::PROTOCOL::CLOSE:
                LOG(INFO,"Close");
                break;
            case api::Tnsd::PROTOCOL::PUBLISH:
                LOG(INFO,"Publish");
                break;
            case api::Tnsd::PROTOCOL::REPORT:
                LOG(INFO,"Report");
                break;
            case api::Tnsd::PROTOCOL::INTERNAL_ERROR:
            default:
                LOG(ERROR,"Internal Error");
                return -1;
        }


        return 0;
    }

    int Sub::connectTnsd() {

        auto tnsdDestinationIp = mSubDic.Value(Sub::TNSD_IP);
        auto tnsdDestinationPort = mSubDic.Value(Sub::TNSD_PORT);

        LOG(INFO,"Tnsd IP:",tnsdDestinationIp, " Tnsd Port:",tnsdDestinationPort);

        //Tnsd 연결시도

        api::Communication::TcpApi tcpApi{mTnsdSocket};

        tcpApi.InitEndpoint();
        if(!tcpApi.Status())
        {
            LOG(ERROR, "fail to initialize Socket{}");
            return -1;
        }

        tcpApi.SetReuseOpt(tnsdDestinationIp, tnsdDestinationPort);
        if(!tcpApi.Status()) {
            LOG(ERROR, "fail to SetReuseOpt");
            return -1;
        }

        tcpApi.SetTimeoutOpt(5);
        if(!tcpApi.Status()) {
            LOG(ERROR, "fail to SetTimeoutOpt");
            return -1;
        }

        tcpApi.Connect(tnsdDestinationIp, tnsdDestinationPort, 10);
        if(!tcpApi.Status()) {
            LOG(ERROR,"fail to connect tnsd");
            return -1;
        }

        return 0;
    }

    int Sub::sendNotifyProtocol() {

        auto myTopic = mSubDic.Value(Sub::TOPIC);
        auto myBindingIp = mSubDic.Value(Sub::SUB_IP);
        auto myBindingPort = mSubDic.Value(Sub::SUB_PORT);

        LOG(INFO,"TOPIC:",myTopic);

        Tnsd::Header header;
        Tnsd::Body<Data::Json> body;

        //StreamTemplate Header, Body로 구성됨.
        Stream::StreamTemplate< Tnsd::Header, Tnsd::Body<Data::Json> > streamTemplate{header, body};

        header.SetProtocol(Tnsd::PROTOCOL::NOTIFY);
        body.Notify(myTopic, "SUB", myBindingIp, myBindingPort);

        auto byteStream = streamTemplate.ToStream();

        if(mTnsdSocket.Send({begin(byteStream), end(byteStream)}) < 0) {
            LOG(ERROR,"fail to Send()");
            return -1;
        }

        return 0;
    }

}//namespace doori
