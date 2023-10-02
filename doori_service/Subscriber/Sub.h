//
// Created by doori on 19. 10. 10..
//

#pragma once
#include "Tnsd/Header.h"
#include "Tnsd/Body.h"
#include "Stream/StreamTemplate.h"
#include "Data/Json.h"
#include "Process/Application.h"
#include "Data/Dictionary.h"
#include "Communication/Socket.h"
#include "Communication/TcpApi.h"
#include "Communication/EpollApi.h"
#include "Process/Runner.h"

namespace doori::service::Subscriber{

class Sub : public api::Process::Application{
public:
    Sub() = delete;
    explicit Sub(api::Data::Dictionary dic);
    virtual auto operator()() noexcept -> int override;
    [[nodiscard]] virtual auto clone() const noexcept -> std::unique_ptr<Application> override;
    virtual auto ProcessName() noexcept -> std::string override;
    virtual auto Daemonize() noexcept -> bool override;
    virtual auto LogFile() noexcept -> std::string override;
    virtual auto LogLevel() noexcept -> api::Common::Log::LEVEL override;
    virtual auto Terminate() noexcept -> int override;

    enum {
        TNSD_IP = 1
        ,TNSD_PORT = 2
        ,LOG_NAME = 11
        ,LOG_PATH = 12
        ,LOG_LEVEL= 13
        ,SUB_IP = 111
        ,SUB_PORT = 112
        ,TOPIC = 113
    };

private:

    int sendNotifyProtocol();

    int connectTnsd();

    auto processMessage(api::Communication::Socket socket) -> int;

    api::Data::Dictionary mSubDic;

    api::Communication::Socket mTnsdSocket;
};

}//namespace doori
