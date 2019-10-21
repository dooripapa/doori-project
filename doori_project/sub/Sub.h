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
    virtual auto ProcessName() noexcept -> std::string override;
    virtual auto Daemonize() noexcept -> bool override;
    virtual auto LogFile() noexcept -> std::string override;
    virtual auto LogLevel() noexcept -> Log::LEVEL override;
    virtual auto Terminate() noexcept -> int override;
private:
    Dictionary mSubDic;
};

}//namespace doori
