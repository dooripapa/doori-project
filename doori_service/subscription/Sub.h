//
// Created by doori on 19. 10. 10..
//

#pragma once


#include <Application.h>
#include <Subscriber.h>

namespace doori{

class Sub : public Application{
public:
    Sub() = delete;
    explicit Sub(const Dictionary& dic);
    virtual auto operator()() noexcept -> int;
    [[nodiscard]] virtual auto clone() const noexcept -> std::unique_ptr<Application> override;
    auto ProcessName() noexcept -> std::string override;
    auto Daemonize() noexcept -> bool override;
    auto LogFile() noexcept -> std::string override;
    auto LogLevel() noexcept -> Log::LEVEL override;
    auto Terminate() noexcept -> int override;
private:
    Dictionary mSubDic;
};

}//namespace doori
