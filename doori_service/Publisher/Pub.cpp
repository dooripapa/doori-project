//
// Created by doori on 19. 10. 10..
//

#include "Pub.h"

namespace doori::service::Publisher{

    auto Pub::operator()() noexcept -> int {

        api::Communication::Socket socket{};

        api::Communication::TcpApi tcpApi{socket};

        tcpApi.InitEndpoint();
        if(!tcpApi.Status())
        {
            LOG(ERROR, "fail to initialize Socket{}");
            return -1;
        }

        auto bindingIp = mPubDic.Value(Pub::PUB_IP);
        auto bindingPort = mPubDic.Value(Pub::PUB_PORT);

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

        epollApi.InitEpoll();
        if(!epollApi.Status()) {
            LOG(ERROR, "fail to InitEpoll()");
            return -1;
        }

        int (*processMessage)(api::Communication::Socket);

        epollApi.RunningForeground(10, 10, processMessage);

    }

    auto Pub::clone() const noexcept -> std::unique_ptr<api::Process::Application> {
        return std::make_unique<Pub>(*this);
    }

    auto Pub::ProcessName() noexcept -> std::string {
        return "Pub";
    }

    auto Pub::Daemonize() noexcept -> bool {
        return false;
    }

    auto Pub::LogFile() noexcept -> std::string {
        if(!mPubDic.Value( Pub::LOG_NAME).empty() && !mPubDic.Value( Pub::LOG_PATH).empty() ) {
            return (mPubDic.Value( Pub::LOG_PATH ) + mPubDic.Value( Pub::LOG_NAME ));
        } else
            return api::Process::Application::LogFile();
    }

    auto Pub::LogLevel() noexcept -> api::Common::Log::LEVEL {
        if(!mPubDic.Value(Pub::LOG_LEVEL).empty()) {
            return api::Common::Log::convertToLevel(mPubDic.Value(Pub::LOG_LEVEL));
        }
        else {
            return api::Process::Application::LogLevel();
        }
    }

    Pub::Pub(const api::Data::Dictionary &dic) : mPubDic(dic){
    }

    auto Pub::Terminate() noexcept -> int {
        LOG(INFO, "Pub is terminated");
        return 0;
    }

    auto Pub::processMessage(api::Communication::Socket socket) -> int {
        return 0;
    }

}//namespace doori
