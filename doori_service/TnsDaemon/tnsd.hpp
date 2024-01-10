#include <cassert>
#include "tnsd.h"

namespace doori::service::TnsDaemon{

    template<typename T_IPCTopologyNode>
    Tnsd<T_IPCTopologyNode>::Tnsd(const Data::Dictionary& dictionary): mDic{dictionary} {
    }

    template<typename T_IPCTopologyNode>
    Tnsd<T_IPCTopologyNode>::~Tnsd()
    {
    }

    template<typename T_IPCTopologyNode>
    auto Tnsd<T_IPCTopologyNode>::operator()() noexcept -> int
    {
        Communication::Socket socket{};

        Communication::TcpApi tcpApi{socket};

        tcpApi.InitEndpoint();
        if(!tcpApi.Status())
        {
            LOG(ERROR, "fail to initialize Socket{}");
            return -1;
        }

        auto bindingIp = mDic.Value(Tnsd::TNSD_IP);
        auto bindingPort = mDic.Value(Tnsd::TNSD_PORT);

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

        std::function<int(api::Communication::Socket)> ProcessMessage = std::bind(&Tnsd<T_IPCTopologyNode>::processMessage, this,
                                                              std::placeholders::_1);

        epollApi.InitEpoll(ProcessMessage);
        if(!epollApi.Status()) {
            LOG(ERROR, "fail to InitEpoll()");
            return -1;
        }

        epollApi.RunningForeground(10, 10);

    }

    ///@brief 비정상처리시, 수신된 메시지를 그대로 송신에 보낸다.
    ///@note 이 함수에서, receiver 에러일 경우, 에러메시지는 각각내부 함수에서 메시지를 만들고\
    ///에러코드는 리턴값에 따라 처리하도록 한다.
    template<typename T_IPCTopologyNode>
    auto Tnsd<T_IPCTopologyNode>::processMessage(Communication::Socket socket) -> int
    {
        string lengthBuffer;
        string container;

        //Tnsd StreamProtocol Header, Body
        api::Tnsd::Header tnsdHeader{};
        api::Tnsd::Body<Data::Json> tnsdBody{};

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

        api::Stream::StreamTemplate< api::Tnsd::Header, api::Tnsd::Body<Data::Json> > streamTemplate{tnsdHeader, tnsdBody};

        streamTemplate.FromStream(container);

        auto json = tnsdBody.GetBody();

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
                LOG(DEBUG, "don't know Protocol");
        }

        if(ret) {
            LOG(ERROR, "error[", ret, "]");
            return -1;
        }

        return 0;
    }

    template<typename T_IPCTopologyNode>
    auto Tnsd<T_IPCTopologyNode>::walkTree() noexcept -> void
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

    template<typename T_IPCTopologyNode>
    auto Tnsd<T_IPCTopologyNode>::walkBranches(DataStructure::Branch< api::Tnsd::NodeInfo<T_IPCTopologyNode> >& branch) noexcept -> void
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

    template<typename T_IPCTopologyNode>
    auto Tnsd<T_IPCTopologyNode>::clone() const noexcept -> std::unique_ptr<Application> {
        return std::make_unique<Tnsd>(*this);
    }


    template<typename T_IPCTopologyNode>
    auto Tnsd<T_IPCTopologyNode>::ProcessName() noexcept -> std::string{
        return string("tnsd");
    }

    template<typename T_IPCTopologyNode>
    auto Tnsd<T_IPCTopologyNode>::Daemonize() noexcept -> bool {
        return false;
    }

    template<typename T_IPCTopologyNode>
    auto Tnsd<T_IPCTopologyNode>::LogFile() noexcept -> std::string {
        if(!mDic.Value(Tnsd::LOG_NAME).empty()
           && !mDic.Value(Tnsd::LOG_PATH).empty() )
        {
            return (mDic.Value(Tnsd::LOG_PATH) + mDic.Value(Tnsd::LOG_NAME));
        } else
            return Application::LogFile();
    }

    template<typename T_IPCTopologyNode>
    auto Tnsd<T_IPCTopologyNode>::LogLevel() noexcept -> Common::Log::LEVEL {
        if(!mDic.Value(Tnsd::LOG_LEVEL).empty() )
            return Common::Log::convertToLevel(mDic.Value(Tnsd::LOG_LEVEL));

        return Application::LogLevel();
    }

    template<typename T_IPCTopologyNode>
    auto Tnsd<T_IPCTopologyNode>::Terminate() noexcept -> int {
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
    template<typename T_IPCTopologyNode>
    long Tnsd<T_IPCTopologyNode>::processNotifyProtocol(Data::Json json, Communication::Socket socket) {

        SIDE side;

        auto strTopic = json["topic"].ToString();
        auto strSide = json["side"].ToString();
        auto strIp = json["ip"].ToString();
        auto strPort = json["port"].ToString();

        LOG(DEBUG, "Topic:", strTopic);
        LOG(DEBUG, "Side:" , strSide);
        LOG(DEBUG, "IP:"   , strIp);
        LOG(DEBUG, "Port:" , strPort);

        Topic topic{};

        // . 구분자값이 들어간 string을 topic객체로 구조화합니다.
        topic.set(strTopic);

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

        NodeInfo<T_IPCTopologyNode> nodeInfo{strIp, strPort};

        nodeInfo.SetIPCTopologyNode(socket);

        bool canAttach = true;

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

        api::Tnsd::Header tnsdHeader{};
        api::Tnsd::Body<Data::Json> tnsdBody{};

        api::Stream::StreamTemplate< api::Tnsd::Header, api::Tnsd::Body<Data::Json> > streamTemplate{tnsdHeader, tnsdBody};

        // PUB이면 Sub Tree에서 해당되는 Leaf에게 Change Protocol 를 보낸다.
        // SUB이면 Pub Tree로부터 접속정보수집 후. Answer Protocol를 보낸다. Subscriber들은 각자 연결 요청을 한다.
        api::DataStructure::Branch< NodeInfo<T_IPCTopologyNode> > branch;

        api::Data::Json publisher;
        api::Data::Json_value jsonArray;

        switch (side) {
            case SIDE::PUB:
                LOG(INFO, "PUB FD[",socket.GetFd(),"] Topic[",topic.GetKey(),"]");
                branch = m_SubTree.getBranch(topic);

                LOG(INFO, "Branch's Leaves cnt[",branch.GetLeaves().size(),"]");
                if(branch.GetLeaves().size() > 0) {

                    for(const auto& leaf :branch.GetLeaves()) {
                        auto subNode = leaf.GetIPCTopologyNode();

                        // CHANGE-Protocol
                        tnsdBody.Change(topic.GetKey(), "PUB", leaf.GetIp(), leaf.GetPort());

                        auto changeProtocolBytes = streamTemplate.ToStream();

                        //send CHANGE-Protocol bytestream
                        subNode.Send({begin(changeProtocolBytes), end(changeProtocolBytes)});
                    }
                }
                break;
            case SIDE::SUB:
                LOG(INFO, "SUB FD[",socket.GetFd(),"] Topic[",topic.GetKey(),"]");
                branch = m_PubTree.getBranch(topic);

                LOG(INFO, "Branch's Leaves cnt[",branch.GetLeaves().size(),"]");
                if(branch.GetLeaves().size() > 0) {

                    for(const auto& leaf :branch.GetLeaves()) {
                        publisher["ip"] = leaf.GetIp();
                        publisher["port"] = leaf.GetPort();

                        // [
                        //  {"ip":"...", "port":"..."}
                        // ,{"ip":"...", "port":"..."}
                        // ,{"ip":"...", "port":"..."}
                        // ...
                        // ]
                        jsonArray.append(publisher);
                    }

                    auto publisherArrayListInfo = jsonArray.ToString();

                    std::size_t hashValue = std::hash<std::string>{}(publisherArrayListInfo);

                    std::ostringstream oss;

                    oss << std::hex << hashValue;

                    //ANWSER-Protocol
                    tnsdBody.Anwser(oss.str(), publisherArrayListInfo);

                    auto answerProtocolBytes = streamTemplate.ToStream();

                    //send ANSWER-Protocol bytestream
                    socket.Send({begin(answerProtocolBytes), end(answerProtocolBytes)});
                }
                break;
        }

        return 0;
    }
}

