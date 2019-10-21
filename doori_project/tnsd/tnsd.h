#pragma once
#include "Epoll.h"
#include "Tree.h"
#include "Addr.h"
#include "Data.h"
#include "Connection.h"
#include "Stream.h"
#include "Protocol.h"
#include "Branch.h"
#include "Application.h"
#include <thread>
#include <unordered_map>

namespace doori{

class Tnsd : public Application
{
public:
    Tnsd() = delete;
    explicit Tnsd(const Dictionary& dictionary);
    ~Tnsd();
    auto operator=(const Tnsd& rhs) = delete;
    auto operator()() noexcept -> int;
    [[nodiscard]] auto clone() const noexcept -> std::unique_ptr<Application>;
    auto ProcessName() noexcept -> std::string override;
    auto Daemonize() noexcept -> bool override ;
    auto LogFile() noexcept -> std::string override;
    auto LogLevel() noexcept -> Log::LEVEL override;
    auto Terminate() noexcept -> int override;
private:
    auto walkTree() noexcept -> void;
    auto walkBranches(Branch<Addr>&) noexcept -> void;
    auto processMessage(int socket, Stream& stream) -> int;
    auto notify_processing (Protocol& protocol, int) -> int;
    auto alive_processing  (Protocol& protocol) -> int;
    auto report_processing (Protocol& protocol) -> int;
    auto list_processing   (Protocol& protocol) -> int;
    auto change_Processing (const Topic& topic) -> void;
    Tree< Addr > m_PubTree;
    Tree< Addr > m_SubTree;

    Dictionary mDic;

    std::unordered_map<std::string, int> mMangedMetaAddresses;
};

}//namespace doori
