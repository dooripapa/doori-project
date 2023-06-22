#pragma once
#include "DataStructure/Tree.h"
#include "DataStructure/Branch.h"
#include "Process/Application.h"
#include "Tnsd/NodeInfo.h"
#include <thread>
#include <unordered_map>

using namespace doori;

namespace doori::service::Tnsd{

    class Tnsd : public Process::Application
    {
    public:
        Tnsd() = delete;
        explicit Tnsd(const Data::Dictionary& dictionary);
        ~Tnsd();
        auto operator=(const Tnsd& rhs) = delete;
        auto operator()() noexcept -> int;
        [[nodiscard]] auto clone() const noexcept -> std::unique_ptr<Application>;
        auto ProcessName() noexcept -> std::string override;
        auto Daemonize() noexcept -> bool override ;
        auto LogFile() noexcept -> std::string override;
        auto LogLevel() noexcept -> Common::Log::LEVEL override;
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
        DataStructure::Tree<NodeInfo> m_PubTree;
        DataStructure::Tree<NodeInfo> m_SubTree;

        Data::Dictionary mDic;

        std::unordered_map<std::string, int> mMangedMetaAddresses;
    };
};


