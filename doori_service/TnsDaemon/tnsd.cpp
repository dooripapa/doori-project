#include <cassert>
#include "tnsd.h"

namespace doori::service::Tnsd{

    Tnsd::Tnsd(const Data::Dictionary& dictionary): mDic{dictionary} {
    }

    Tnsd::~Tnsd()
    {
    }

    auto Tnsd::operator()() noexcept -> int
    {
        mDic.logging();

        Communication::Socket socket{};

        Communication::TcpApi tcpApi{socket};

        tcpApi.InitEndpoint();
        if(!tcpApi.Status())
        {
            LOG(ERROR, "fail to initialize Socket{}");
            return -1;
        }

        auto bindingIp = mDic.Value(Data::Dictionary::TOKEN_INFO::TNSD_IP);
        auto bindingPort = mDic.Value(Data::Dictionary::TOKEN_INFO::TNSD_PORT);

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

        Communication::EpollApi epollApi{listenSocket };

        epollApi.InitEpoll();
        if(!epollApi.Status()) {
            LOG(ERROR, "fail to InitEpoll()");
            return -1;
        }

        int (*ProcessMessage)(Communication::Socket);

        epollApi.RunningForeground(10, 10, ProcessMessage);

    }

    ///@brief 비정상처리시, 수신된 메시지를 그대로 송신에 보낸다.
    ///@note 이 함수에서, processing 에러일 경우, 에러메시지는 각각내부 함수에서 메시지를 만들고\
    ///에러코드는 리턴값에 따라 처리하도록 한다.
    auto Tnsd::processMessage(Communication::Socket socket) -> int
    {
        string lengthBuffer;
        string container;

        //Tnsd StreamProtocol Header, Body
        doori::api::Tnsd::Header tnsdHeader{};
        doori::api::Tnsd::Body tnsdBody{};

        //길이값 8자리를 파싱.
        auto ret = socket.Recv(lengthBuffer, 8);
        if(ret <= 0) {
            LOG(ERROR, "fail to Recv for Length");
            return -1;
        }

        auto utilLength = stoi(lengthBuffer, 0, 10);

        //데이터를 끝까지 수신합니다.
        ret = socket.Recv(container, utilLength);
        if(ret <= 0) {
            LOG(ERROR, "fail to Recv til size[", utilLength, "]");
            return -1;
        }

        doori::api::Stream::StreamTemplate< doori::api::Tnsd::Header, doori::api::Tnsd::Body > streamTemplate{tnsdHeader, tnsdBody};

        streamTemplate.FromStream(container);

        auto json = tnsdBody.GetJson();

        switch(tnsdHeader.GetProtocol())
        {
            case api::Tnsd::PROTOCOL::NOTIFY:
                ret=processNotifyProtocol(json, socket);
                break;
            case api::Tnsd::PROTOCOL::ANWSER:
                break;
            case api::Tnsd::PROTOCOL::CHANGE:
                break;
            case api::Tnsd::PROTOCOL::ALIVE:
                break;
            case api::Tnsd::PROTOCOL::CLOSE:
                break;
            case api::Tnsd::PROTOCOL::PUBLISH:
                break;
            case api::Tnsd::PROTOCOL::REPORT:
                break;
            case api::Tnsd::PROTOCOL::INTERNAL_ERROR:
            default:
                LOG(DEBUG, "");
        }

        if(ret) {
            LOG(ERROR, "error[", ret, "]");
            return -1;
        }

        return 0;
    }

    auto Tnsd::walkTree() noexcept -> void
    {
        LOG(DEBUG, "Pub Tree --------");
        for(auto& branch:m_PubTree.RootBranches())
        {
            walkBranches(branch);
        }
        LOG(DEBUG, "Pub Tree ---END-----");
        LOG(DEBUG, "Sub Tree --------");
        for(auto& branch:m_SubTree.RootBranches())
        {
            walkBranches(branch);
        }
        LOG(DEBUG, "Sub Tree ---END-----");
    }

    auto Tnsd::walkBranches(DataStructure::Branch< api::Tnsd::NodeInfo >& branch) noexcept -> void
    {
        LOG(DEBUG, "TopicAccess ---- : ", branch.GetName());
        for(auto& m: branch.GetLinkBranches())
        {
            walkBranches(m);
            for(auto& i : m.GetLeaves())
            {
                LOG(DEBUG, i.GetIp(), " : ", i.GetPort() );
            }
        }
    }

    auto Tnsd::clone() const noexcept -> std::unique_ptr<Application> {
        return std::make_unique<Tnsd>(*this);
    }


    auto Tnsd::ProcessName() noexcept -> std::string{
        return string("tnsd");
    }

    auto Tnsd::Daemonize() noexcept -> bool {
        return false;
    }

    auto Tnsd::LogFile() noexcept -> std::string {
        if(!mDic.Value(Data::Dictionary::TOKEN_INFO::LOG_NAME).empty()
           && !mDic.Value(Data::Dictionary::TOKEN_INFO::LOG_PATH).empty() )
        {
            return (mDic.Value(Data::Dictionary::TOKEN_INFO::LOG_PATH) + mDic.Value(Data::Dictionary::TOKEN_INFO::LOG_NAME));
        } else
            return Application::LogFile();
    }

    auto Tnsd::LogLevel() noexcept -> Common::Log::LEVEL {
        if(!mDic.Value(Data::Dictionary::TOKEN_INFO::LOG_LEVEL).empty() )
            return Common::Log::convertToLevel(mDic.Value(Data::Dictionary::TOKEN_INFO::LOG_LEVEL));

        return Application::LogLevel();
    }

    auto Tnsd::Terminate() noexcept -> int {
        LOG(INFO, "MiddleSide is terminated");
        return 0;
    }


    /**
     * Pub이면, 해당 Topic에 관심이 있는 개발 Sub에게 Change 송신
     * Sub이면, Publisher list와 Token 정보를 보냄.
     * @param side Publisher, Subscriber
     * @param topic
     * @return
     */
    long Tnsd::processNotifyProtocol(Data::Json json, Communication::Socket socket) {

        SIDE side;

        auto strTopic = json["topic"].ToString();
        auto strSide = json["side"].ToString();
        auto strIp = json["ip"].ToString();
        auto strPort = json["port"].ToString();

        Topic topic{strTopic};

        if( strSide ==  "PUB") {
            side = SIDE::PUB;
        }
        else if ( strSide ==  "SUB"){
            side = SIDE::SUB;
        }
        else
        {
            LOG(ERROR, "Notify");
            return -1;
        }

        NodeInfo<Communication::Socket> nodeInfo{topic, side, strIp, strPort};

        nodeInfo.SetIPC(socket);

        bool canAttach = false;
        //등록
        switch (side) {
            case SIDE::PUB:
                canAttach = m_PubTree.attachLeaf(topic, nodeInfo);
                break;
            case SIDE::SUB:
                canAttach = m_SubTree.attachLeaf(topic, nodeInfo);
                break;
        }

        if(!canAttach) {
            LOG(ERROR, "fail to attach ILeaf");
            return -1;
        }

        // PUB이면 Sub Tree에서 해당되는 Leaf에게 Change를 보낸다.
        switch (side) {
            case SIDE::PUB:
                auto branch = m_SubTree.getBranch(topic);
                if(branch.GetLeaves().size() > 0)
                {
                    for(auto leaf :branch.GetLeaves())
                    {

                    }
                }
                break;
            case SIDE::SUB:
                break;
        }

        return 0;
    }
}

