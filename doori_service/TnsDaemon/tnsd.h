#pragma once
#include <thread>
#include <unordered_map>
#include "DataStructure/Tree.h"
#include "DataStructure/Branch.h"
#include "Process/Application.h"
#include "Communication/TcpApi.h"
#include "Communication/EpollApi.h"
#include "Communication/Socket.h"
#include "Stream/StreamTemplate.h"
#include "Tnsd/NodeInfo.h"
#include "Tnsd/Header.h"
#include "Tnsd/Body.h"

using namespace doori;

namespace doori::service::TnsDaemon{

    template<typename T_IPCTopologyNode>
    class Tnsd : public Process::Application
    {
    public:
        Tnsd() = delete;
        explicit Tnsd(const Data::Dictionary& dictionary);
        ~Tnsd();
        auto operator=(const Tnsd& rhs) = delete;
        auto operator()() noexcept -> int;

        /**
         * @todo make_unique 적절히 사용했는지 체크해야 함.\
         * @return
         */
        [[nodiscard]] auto clone() const noexcept -> std::unique_ptr<Application>;
        auto ProcessName() noexcept -> std::string override;
        auto Daemonize() noexcept -> bool override ;
        auto LogFile() noexcept -> std::string override;
        auto LogLevel() noexcept -> Common::Log::LEVEL override;

        /**
         * @todo MiddleSide 종료시 이 함수 호촐되어야 한다. 현재 호출되지 않고 있음.
         * @return
         */
        auto Terminate() noexcept -> int override;

        enum {
            TNSD_IP = 1
            ,TNSD_PORT = 2
            ,LOG_NAME = 11
            ,LOG_PATH = 12
            ,LOG_LEVEL = 13
        };
    private:
        auto walkTree() noexcept -> void;
        auto walkBranches(DataStructure::Branch< NodeInfo<T_IPCTopologyNode> >&) noexcept -> void;
        auto processMessage(Communication::Socket socket) -> int;
        DataStructure::Tree< NodeInfo<T_IPCTopologyNode> > m_PubTree;
        DataStructure::Tree< NodeInfo<T_IPCTopologyNode> > m_SubTree;

        Data::Dictionary mDic;

        std::unordered_map<std::string, int> mMangedMetaAddresses;

        long processNotifyProtocol(Data::Json json, Communication::Socket socket);
    };
};


#include "tnsd.hpp"

