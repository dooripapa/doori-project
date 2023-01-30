//
// Created by jaeseong on 20. 7. 13..
//

#include "ProcessSupport/Getopt.h"
#include "CommunicationMember/Addr.h"
#include "CommunicationMember/Endpoint.h"
#include "CommunicationMember/Connection.h"



int main(int argc, char** argv) {
    doori::LOG_CREATE("/tmp/pong.log", doori::Log::LEVEL::D);
    doori::Getopt opt{argc, argv};
    std::string ip = opt.getOptValue("--ip");
    if(ip.length() == 0)
    {
        std::cout<<"--ip not setting"<<std::endl;
        return -1;
    }

    std::string port = opt.getOptValue("--port");
    if(port.length() == 0)
    {
        std::cout<<"--port not setting"<<std::endl;
        return -1;
    }

    doori::Address address;
    address.Ip=ip;
    address.Port=port;

    doori::Addr addr;
    addr.setAddress(address);

    doori::Endpoint endpoint;
    endpoint.setAddress(addr);

    doori::Connection conn;
    conn.setFrom( endpoint );

    int fd = conn.waitFor();
    if(fd<0)
        std::cout<<"waitFor() error"<<std::endl;

    doori::Stream   buffer;

    while(1) {
        auto iRet = conn.recvFrom(fd, buffer);
        if( iRet == -2 )
            return 0;
        iRet = conn.sendTo(fd, buffer);
        if( iRet == -2 )
            return 0;
    }
    return 0;
}
