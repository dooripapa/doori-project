//
// Created by doori on 19. 10. 10..
//

#pragma once
#include "Process/Application.h"
#include "Data/Dictionary.h"
#include "Communication/Socket.h"
#include "Communication/TcpApi.h"
#include "Communication/EpollApi.h"
#include "Process/Runner.h"

namespace doori::service::Publisher{

class Pub : public api::Process::Application{
public:
    Pub() = delete;
    explicit Pub(const api::Data::Dictionary& dic);
    virtual auto operator()() noexcept -> int;
    [[nodiscard]] virtual auto clone() const noexcept -> std::unique_ptr<Application> override;
    auto ProcessName() noexcept -> std::string override;
    auto Daemonize() noexcept -> bool override;
    auto LogFile() noexcept -> std::string override;
    auto LogLevel() noexcept -> api::Common::Log::LEVEL override;
    auto Terminate() noexcept -> int override;

    auto processMessage(api::Communication::Socket socket) -> int;
    enum {
        TNSD_IP = 1
        ,TNSD_PORT = 2
        ,LOG_NAME = 11
        ,LOG_PATH = 12
        ,LOG_LEVEL= 13
        ,PUB_IP = 111
        ,PUB_PORT = 112
    };
private:
    api::Data::Dictionary mPubDic;
};

}//namespace doori
