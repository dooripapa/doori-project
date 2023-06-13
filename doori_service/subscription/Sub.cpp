//
// Created by doori on 19. 10. 10..
//

#include "Sub.h"

namespace doori{

Sub::Sub(const Dictionary &dic) : mSubDic(dic){
}

auto Sub::operator()() noexcept -> int {
    Subscriber subscriber(
    [](int fd, Stream& stream)
                {
                    LOG(DEBUG, "From FD :", fd);
                    LOG(DEBUG, "Data stream :", stream.toByteStream() );
                    return 0;
                }
    );

    Connection tnsdConnection;
    Addr tns(mSubDic.Value(Dictionary::TOKEN_INFO::TNSD_IP),
             mSubDic.Value(Dictionary::TNSD_PORT));
    Addr sourceAddr(mSubDic.Value(Dictionary::TOKEN_INFO::BINDING_IP_FOR_TNSD),
                    mSubDic.Value(Dictionary::TOKEN_INFO::BINDING_PORT_FOR_TNSD));

    Endpoint tnsd; tnsd.setAddress(tns);
    Endpoint sourcer; sourcer.setAddress(sourceAddr);
    tnsdConnection.setFrom(sourcer);
    tnsdConnection.setTo(tnsd);

    Connection subConnection;
    Endpoint subEndpoint{{mSubDic.Value(Dictionary::TOKEN_INFO::BINDING_IP_FOR_MULTISESSION),
                                 mSubDic.Value(Dictionary::BINDING_PORT_FOR_MULTISESSION)} };
    subConnection.setFrom(subEndpoint);

    Topic topic;
    topic.set(mSubDic.Value(Dictionary::TOKEN_INFO::MY_TOPIC));
    if(!subscriber.Init(tnsdConnection, topic, Protocol::TREE::SUB))
    {
        LOG(ERROR, "failed to connect to MiddleSide");
        return -1;
    }
    subscriber.startSubscriber(subConnection );

    /*Tnsd에 나의 관심대상을 등록한다.*/
    if( !subscriber.sendNotifyProtocolToTnsd() ) {
        LOG(DEBUG, "fail to list up");
        return -1;
    }

    subscriber.sendListProtocolToTnsd();
    LOG(DEBUG, "Knock TopicAccess! : ", topic.GetKeyName() );

    // Never, Terminated.
    subscriber.onSubscribing();

    return 0;
}

auto Sub::clone() const noexcept -> std::unique_ptr<Application> {
    return std::make_unique<Sub>(*this);
}

auto Sub::ProcessName() noexcept -> std::string {
    return "Sub";
}

auto Sub::Daemonize() noexcept -> bool {
    return true;
}

auto Sub::LogFile() noexcept -> std::string {
    if(!mSubDic.Value(Dictionary::TOKEN_INFO::LOG_NAME).empty()
       && !mSubDic.Value(Dictionary::TOKEN_INFO::LOG_PATH).empty() )
    {
        return (mSubDic.Value(Dictionary::TOKEN_INFO::LOG_PATH) + mSubDic.Value(Dictionary::TOKEN_INFO::LOG_NAME));
    } else
        return Application::LogFile();
}

auto Sub::LogLevel() noexcept -> Log::LEVEL {
    if(!mSubDic.Value(Dictionary::TOKEN_INFO::LOG_LEVEL).empty() )
        return Log::convertToLevel(mSubDic.Value(Dictionary::TOKEN_INFO::LOG_LEVEL));

    return Application::LogLevel();
}

auto Sub::Terminate() noexcept -> int {
    LOG(INFO, "Sub is terminated");
    return 0;
}

}//namespace doori
