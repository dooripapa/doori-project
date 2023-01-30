//
// Created by jaeseong on 20. 7. 13..
//

#include "ProcessSupport/Getopt.h"
#include "CommunicationMember/Addr.h"
#include "CommunicationMember/Endpoint.h"
#include "CommunicationMember/Connection.h"
#include <ctime>
#include <sys/time.h>

int main(int argc, char** argv) {
    doori::LOG_CREATE("/tmp/ping.log", doori::Log::LEVEL::D);
    doori::Getopt opt{argc, argv};
    std::string ip = opt.getOptValue("--ip");
    if(ip.length() == 0)
    {
        std::cout<<"--ip not setting"<<std::endl;
        return -1;
    }
    std::cout<<"--ip : "<< ip << std::endl;

    std::string port = opt.getOptValue("--port");
    if(port.length() == 0)
    {
        std::cout<<"--port not setting"<<std::endl;
        return -1;
    }
    std::cout<<"--port : "<< port << std::endl;

    doori::Address address;
    address.Ip=ip;
    address.Port=port;

    doori::Addr addr;
    addr.setAddress(address);

    doori::Endpoint endpoint;
    endpoint.setAddress(addr);

    doori::Connection conn;
    conn.setTo( endpoint );

    conn.connectTo();

    doori::Stream   recvBuffer;
    doori::Data     sendData;
    doori::DataSegment segment;

    struct timeval tp;
    gettimeofday(&tp, NULL);
    long int ms = tp.tv_sec*1000 + tp.tv_usec/1000;

    sendData.append({1, ip});
    sendData.append({2, ms});

    conn.reply(doori::Stream(sendData), recvBuffer);

    conn.release();

    gettimeofday(&tp, NULL);
    ms = tp.tv_sec*1000 + tp.tv_usec/1000;

    doori::Data resultData;
    resultData.fromString(recvBuffer.getString());
    auto it = resultData.find_if_Fid(2);
    if( it == resultData.end() )
    {
        std::cout <<"not find"<<std::endl;
        return 0;
    }

    /**
     * calculate.
     */
    auto result = ms - std::stoll(it->getValueToString().c_str(),0, 0);
    std::cout << "ip : " << ip << "   difference:" << result << std::endl;


    return 0;
}
