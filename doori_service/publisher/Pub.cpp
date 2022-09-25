//
// Created by doori on 19. 10. 10..
//

#include "Pub.h"

namespace doori{

auto Pub::operator()() noexcept -> int {
    Publisher publisher(
            [](int fd, Stream& stream) {
                LOG(DEBUG, "From FD :", fd);
                LOG(DEBUG, "Data stream :", stream.toByteStream() );
                return 0;
            }
    );
    Connection tnsdConn;
    Connection multiRequestReceiver;

    Addr destTnsd(mPubDic.Value(Dictionary::TNSD_IP),
                  mPubDic.Value(Dictionary::TNSD_PORT));
    Addr sourceTnsd(mPubDic.Value(Dictionary::TOKEN_INFO::BINDING_IP_FOR_TNSD),
                    mPubDic.Value(Dictionary::TOKEN_INFO::BINDING_PORT_FOR_TNSD));

    Endpoint tnsd; tnsd.setAddress(destTnsd);
    Endpoint sourcer; sourcer.setAddress(sourceTnsd);

    Endpoint multiReceiverEndpoint{{mPubDic.Value(Dictionary::TOKEN_INFO::BINDING_IP_FOR_MULTISESSION),
                                           mPubDic.Value(Dictionary::TOKEN_INFO::BINDING_PORT_FOR_MULTISESSION)} };

    tnsdConn.setTo(tnsd);
    tnsdConn.setFrom(sourcer);

    multiRequestReceiver.setFrom(multiReceiverEndpoint);

    Topic topic;
    topic.set(mPubDic.Value(Dictionary::TOKEN_INFO::MY_TOPIC));
    if(!publisher.init(tnsdConn,topic,Protocol::TREE::PUB))
    {
        LOG(ERROR, "failed to connect to Tnsd");
        return -1;
    }

    publisher.startPublisher(multiRequestReceiver);

    /*Tnsd에 나의 관심대상을 등록한다.*/
    if( !publisher.sendNotifyProtocolToTnsd() ) {
        LOG(DEBUG, "fail to list up");
        return -1;
    }

    Data data;
    DataSegment segment;
    segment.set(123, 123456789L);
    data.append( segment );
    data.walk();

    Stream stream(data);

    while(1)
    {
        publisher.publish( stream );
        std::this_thread::sleep_for(std::chrono::seconds(5));
    }
    return -1;
}

auto Pub::clone() const noexcept -> std::unique_ptr<doori::Application> {
    return std::make_unique<Pub>(*this);
}

auto Pub::ProcessName() noexcept -> std::string {
    return "Pub";
}

auto Pub::Daemonize() noexcept -> bool {
    return true;
}

auto Pub::LogFile() noexcept -> std::string {
    if(!mPubDic.Value(Dictionary::TOKEN_INFO::LOG_NAME).empty()
       && !mPubDic.Value(Dictionary::TOKEN_INFO::LOG_PATH).empty() )
    {
        return (mPubDic.Value(Dictionary::TOKEN_INFO::LOG_PATH) + mPubDic.Value(Dictionary::TOKEN_INFO::LOG_NAME));
    } else
        return doori::Application::LogFile();
}

auto Pub::LogLevel() noexcept -> Log::LEVEL {
    if(!mPubDic.Value(Dictionary::TOKEN_INFO::LOG_LEVEL).empty() )
        return Log::convertToLevel(mPubDic.Value(Dictionary::TOKEN_INFO::LOG_LEVEL));

    return doori::Application::LogLevel();
}

Pub::Pub(const Dictionary &dic) : mPubDic(dic){
}

auto Pub::Terminate() noexcept -> int {
    LOG(INFO, "Pub is terminated");
    return 0;
}

}//namespace doori
