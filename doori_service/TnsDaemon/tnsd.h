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

namespace doori::service::Tnsd{

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
    private:
        auto walkTree() noexcept -> void;
        auto walkBranches(DataStructure::Branch<NodeInfo>&) noexcept -> void;
        auto processMessage(Communication::Socket socket) -> int;
        DataStructure::Tree<NodeInfo> m_PubTree;
        DataStructure::Tree<NodeInfo> m_SubTree;

        Data::Dictionary mDic;

        std::unordered_map<std::string, int> mMangedMetaAddresses;

        /**
         * 내부처리
         * Pub이면, 해당 Topic에 관심이 있는 개발 Sub에게 Change 송신
         * Sub이면, Publisher list와 Token 정보를 보냄.
         * @param side Publisher, Subscriber
         * @param topic
         * @return
         */
        int notify(SIDE side, Topic topic);
    };
};


