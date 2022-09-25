#include "tnsd.h"

namespace doori{

Tnsd::Tnsd(const Dictionary& dictionary): mDic{dictionary} {
}

Tnsd::~Tnsd()
{
}

auto Tnsd::operator()() noexcept -> int
{
    mDic.logging();
    Data          data;
    Addr          localAddr{mDic.Value(Dictionary::TOKEN_INFO::TNSD_IP), mDic.Value(Dictionary::TOKEN_INFO::TNSD_PORT)};
    Endpoint      recvAt(localAddr);
    Connection    conn;
    conn.setFrom(recvAt);

    EpollEvents  eventContainer;
    eventContainer.setSize(100);

    std::function<int(int,Stream&)> processMessageInstance(std::bind(&Tnsd::processMessage, this, std::placeholders::_1, std::placeholders::_2));

    Epoll epoll(processMessageInstance);
    if (epoll.init(conn)==-1)
    {
        LOG(ERROR, "Epoll initConnection error");
        return -1;
    }

    while (true)
    {
        if (epoll.waitForEvents(eventContainer, 1000*10))
            LOG(DEBUG, "Event!");
        else
            LOG(DEBUG, "tnsd, timeout");

        for(auto it=eventContainer.cbegin();it!=eventContainer.cend();++it)
        {
            if ( epoll.isListener((*it).getFd()))
            {
                LOG(INFO, "from listener socket, add watcher");
                epoll.addWatcherAsConn();
            }
            else
            {
                LOG(INFO, "another socket action: ", (*it).getFd() );
                // Tnsd::processMessage private member function이여서, instance없이
                // 바로 사용 불가능하다. instance 작업후 사용하도록 변경함
                auto iRet = epoll.executeTask( (*it).getFd() );

                if( iRet == -2 ) //(*it).getFd() connection lost!
                    for(auto& i:mMangedMetaAddresses)
                        if ( i.second == (*it).getFd() ){
                            mMangedMetaAddresses.erase( i.first );
                            LOG(DEBUG, "Meta Address remove :", i.first);
                            break;
                        }
            }
        }
    }
}

///@brief 비정상처리시, 수신된 메시지를 그대로 송신에 보낸다.
///@note 이 함수에서, processing 에러일 경우, 에러메시지는 각각내부 함수에서 메시지를 만들고\ ///      에러코드는 리턴값에 따라 처리하도록 한다.
auto Tnsd::processMessage(int socket, Stream& stream) -> int
{
    auto ret=0;
    Data data, responseData;
    Stream responseStream;
    Protocol protocol;

    LOG(INFO, "Request Comment[", stream.toByteStream(),"]" );
    if ( data.fromString( stream.getString() ) == -1 )
    {
        LOG(ERROR, "unserialize error, check formated" );
        return -1;
    }
    
    if (protocol.fromData(data)<0)
    {
        LOG(ERROR, "protocol init error");
        return -1;
    }

    switch(protocol.MsgType())
    {
        case Protocol::TYPE::NOTIFY:
            LOG(DEBUG, Protocol::NOTIFY_MSG);
            ret = notify_processing(protocol, socket);
            break;
        case Protocol::TYPE::ALIVE:
            LOG(DEBUG, Protocol::ALIVE_MSG);
            ret = alive_processing(protocol);
            break;
        case Protocol::TYPE::REPORT:
            LOG(DEBUG, Protocol::REPORT_MSG);
            ret = report_processing(protocol);
            break;
        case Protocol::TYPE::LIST:
            LOG(DEBUG, Protocol::LIST_MSG);
            ret = list_processing(protocol);
            break;
        default:
            ret = -1;
            LOG(ERROR, Protocol::UNKOWN_MSG);
    }
    protocol.asResponser();
    if(ret!=0)
    {
        LOG(ERROR, "protocol processing error");
        protocol.MsgCode() = Protocol::STATUS_CODE::ERR;
    } else{
        LOG(DEBUG, "message processing completed");
        protocol.MsgCode() = Protocol::STATUS_CODE::OK;
    }

    responseStream.init( protocol.toData() );
    LOG(DEBUG, "Socket FD: ", socket, ", Answer Comment[", responseStream.toByteStream(), "]" );
    if (Connection::sendTo(socket, responseStream) < 0 ) {
        LOG(ERROR, "sendTo fail");
    }

    return 0;
}


///@brief Notify 수신후에, CHANGE 프로토콜을 만들어서, 해당 subscriber에게 송신하다.
auto Tnsd::notify_processing(Protocol& protocol, int socketfd ) -> int
{
    Topic topic{};
    Addr  leaf{};
    Branch<Addr> branch;

    topic          = protocol.TopicAccess();
    string ip      =protocol.Ip();
    string port    =protocol.Port();

    LOG(DEBUG, "TopicAccess: ", topic.getTopicName(), " ", "ip: ", ip, " ", "port: ", port);

    leaf = Addr(ip, port);
    switch(protocol.TreeAccess())
    {
        case Protocol::TREE::PUB:
            if(!m_PubTree.attachLeaf(topic, leaf )) {
                LOG(WARN, "Pub's Tree, Duplicated Leaf");
                protocol.MsgComment() = "duplicated Leaf";
            } else
                protocol.MsgComment() = "register leaf";

            /* CHANGE Protocol Sending Subscribers is interesting that topic.*/
            LOG(DEBUG, "CHANGE protocol send");
            change_Processing(topic);
            break;
        case Protocol::TREE::SUB:
            if(!m_SubTree.attachLeaf(topic, leaf )) {
                LOG(WARN, "Sub's Tree, Duplicated Leaf");
                protocol.MsgComment() = "duplicated Leaf";
            } else{
                protocol.MsgComment() = "register leaf";
                /* Register addressMetas, or not existing element accessed/written */
                mMangedMetaAddresses[ip+":"+port] = socketfd;
            }
            break;
        default:
            protocol.MsgComment() = "unknown Tree's Access-type";
            return -1;
    }
    walkTree();
    return 0;
}
///@todo if, topic에 해당하는 branch가 아예 존재하지 않을 경우, 에러 처리해야 함.
auto Tnsd::alive_processing(Protocol& protocol) -> int
{
    auto iRet = -1;
    Addr addr(protocol.Ip(), protocol.Port());
    Protocol::TREE treeType = protocol.TreeAccess();

    switch(treeType)
    {
        case Protocol::TREE::PUB:
            for(auto leaf: m_PubTree.getBranch(protocol.TopicAccess()).getLeaves())
            {
                if(leaf == addr) {
                    iRet = 0;
                    break;
                }
            }
            break;
        case Protocol::TREE::SUB:
            for(auto leaf: m_SubTree.getBranch(protocol.TopicAccess()).getLeaves())
            {
                if(leaf == addr) {
                    iRet = 0;
                    break;
                }
            }
            break;
        default:
            ;
    }
    protocol.MsgComment()=Protocol::OK_MSG;
    return iRet;
}

auto Tnsd::report_processing(Protocol& protocol) -> int
{
    protocol.MsgComment()=Protocol::OK_MSG;
    return 0;
}

auto Tnsd::list_processing(Protocol& protocol) -> int
{
    int ArrSumCnt = 0;
    string ip;
    string port;
    Topic topic{};

    topic.set(protocol.TopicAccess().getTopicName());

    switch(protocol.TreeAccess())
    {
        case Protocol::TREE::PUB:
            {
                auto& findPubBranch = m_PubTree.getBranch(topic);
                ArrSumCnt = findPubBranch.getLeaves().size();
                for(auto it=findPubBranch.getLeaves().cbegin();it!=findPubBranch.getLeaves().cend();++it)
                {
                    ip = (*it).Ip();
                    port = (*it).Port();
                    LOG(INFO, "Leaf :: ",ip, " : ", port );
                    protocol.appendSession(ip,port);
                }
            }
            break;
        case Protocol::TREE::SUB:
            {
                auto& findSubBranch = m_SubTree.getBranch(topic);
                ArrSumCnt = findSubBranch.getLeaves().size();
                for(auto it=findSubBranch.getLeaves().cbegin();it!=findSubBranch.getLeaves().cend();++it)
                {
                    ip = (*it).Ip();
                    port = (*it).Port();
                    LOG(INFO, "Leaf :: ",ip, " : ", port );
                    protocol.appendSession(ip,port);
                }
            }
            break;
        default:
            protocol.MsgComment() = Protocol::FATAL_ERR_MSG;
            return -1;
    }
    protocol.completeSession();

    if(ArrSumCnt == 0)
        protocol.MsgComment()="list it Nothing";
    else
        protocol.MsgComment()=Protocol::OK_MSG;

    return 0;
}

///@brief debugging
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

///@brief debugging
auto Tnsd::walkBranches(Branch< Addr>& branch) noexcept -> void
{
    LOG(DEBUG, "TopicAccess ---- : ", branch.getName());
    for(auto& m:branch.getLinkBranches())
    {
        walkBranches(m);
        for(auto& i : m.getLeaves())
        {
            LOG(DEBUG, i.Ip(), " : ", i.Port() );
        }
    }
}
///@todo make_unique 적절히 사용했는지 체크해야 함.\
/// make_unique<Tnsd>(*this) *this 복사가 맞는지 확인할것.
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
    if(!mDic.Value(Dictionary::TOKEN_INFO::LOG_NAME).empty()
    && !mDic.Value(Dictionary::TOKEN_INFO::LOG_PATH).empty() )
    {
        return (mDic.Value(Dictionary::TOKEN_INFO::LOG_PATH) + mDic.Value(Dictionary::TOKEN_INFO::LOG_NAME));
    } else
        return Application::LogFile();
}

auto Tnsd::LogLevel() noexcept -> Log::LEVEL {
    if(!mDic.Value(Dictionary::TOKEN_INFO::LOG_LEVEL).empty() )
        return Log::convertToLevel(mDic.Value(Dictionary::TOKEN_INFO::LOG_LEVEL));

    return Application::LogLevel();
}

///@todo Tnsd 종료시 이 함수 호촐되어야 한다. 현재 호출되지 않고 있음.
auto Tnsd::Terminate() noexcept -> int {
    LOG(INFO, "Tnsd is terminated");
    return 0;
}

///@brief Notify protocol 요청이 들어옴. 해당 Topic을 관심있는 Sub에게 Change가 되었다는 알려줌
///@todo doori_connection::sendTo(), loop error 처리를 어떻게 할 것인가 고민할덧.
auto Tnsd::change_Processing(const Topic &topic) -> void{
    string      strTopic;
    Topic depTopic;
    Stream    responseStream;
    Protocol  protocol;
    for(auto i=0;i<topic.getDepthSize();++i)
    {
        if(i>0) {
            strTopic += ".";
            strTopic += topic.getTopicName(i);
        } else
            strTopic = topic.getTopicName(i);

        depTopic.set(strTopic);
        auto branch = m_SubTree.getBranch( depTopic );

        for( const Addr& leaf : branch.getLeaves() ) {
            // 이렇게 operator[] 호출되면, insert가 되면서, element 초기화되면서, 0을 리턴.
            //auto sockfd = mMangedMetaAddresses[leaf.Ip()+":"+leaf.Port()];

            LOG(DEBUG, "Change protocol send Address :", leaf.Ip(), ":", leaf.Port() );
            auto got = mMangedMetaAddresses.find(leaf.Ip()+":"+leaf.Port());
            if(got==mMangedMetaAddresses.end())
            {
                LOG(DEBUG, "Not find in Managed MetaAddresses");
                for(const auto& i:mMangedMetaAddresses)
                    LOG(DEBUG, "Change protocol :: ",i.first,":" ,i.second);
            }
            else
            {
                auto sockFd = got->second;
                protocol.asSender();
                protocol.asChange( Protocol::TREE::PUB, depTopic );
                responseStream.init( protocol.toData() );
                LOG(DEBUG, "Subscriber[", sockFd,"]","<<-- ::" , responseStream.toByteStream(), "]" );
                if (Connection::sendTo(sockFd, responseStream) < 0 ) {
                    LOG(ERROR, "fail to send CHANGE protocol fd[", sockFd, "]");
                }
            }
        }
    }
}

}//namespace doori
