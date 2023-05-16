//
// Created by jaeseong on 23. 1. 31.
//

#include "TCPBuilder.h"
#include "TcpApi.h"
#include "Common/Log.h"

namespace doori::CommunicationMember {

    TCPBuilder::~TCPBuilder() {

    }

    /*
     * CLIENT이면 소켓만 생성, 바인딩할 필요가 없음, 목적지IP, PORT만 존재하면 됨
     * SERVER이면 수신 해야 함으로, 특정IP, PORT로 바인딩되어야 함
     */
    int TCPBuilder::BindFrom() {
        auto sock = -1;
        string err = "";

        sock = TcpApi::CreateSocket();
        if (sock < 0)
            throw std::runtime_error("TcpApi::CreateSocket()");

        switch (mType) {
            case CLIENT:
                break;
            case SERVER:
                sock = TcpApi::Bind(sock, mSourceIp, mSourcePort);
                if (sock < 0) {
                    err = "TcpApi::Bind";
                    break;
                }
                sock = TcpApi::SetReuseOpt(sock, mSourceIp, mSourcePort);
                err = "TcpApi::SetReuseOpt";
                break;
        }

        if (sock < 0)
            throw std::runtime_error(err);

        mSourceFd = sock;

        return sock;
    }

    /*
     * CLIENT이면 특정IP, PORT로 바인딩한다.
     * SERVER이면 이미 바인딩 했기 때문에 아무것도 하지 않음
     */
    int TCPBuilder::BindTo() {
        auto sock = -1;
        string err = "";

        sock = TcpApi::CreateSocket();
        if (sock < 0)
            throw std::runtime_error("TcpApi::CreateSocket()");

        switch (mType) {
            case CLIENT:
                if( !mSourceIp.empty() && !mSourcePort.empty() )
                {
                    sock = TcpApi::Bind(mSourceFd, mSourceIp, mSourcePort);
                    if (sock < 0) {
                        err = "TcpApi::Bind";
                        break;
                    }
                    sock = TcpApi::SetReuseOpt(mSourceFd, mSourceIp, mSourcePort);
                    err = "TcpApi::SetReuseOpt";
                }
                break;
            case SERVER:
                break;
        }

        if (sock < 0)
            throw std::runtime_error(err);

        return mSourceFd;
    }

    int TCPBuilder::EstablishTopologies() {
        int sock = -1;
        int connSock = -1;
        string err = "";
        switch (mType) {
            case CLIENT:
                connSock = TcpApi::Connect(mSourceFd, mDestIp, mDestPort);
                err = "TcpApi::Connect";
                break;
            case SERVER:
                sock = TcpApi::Listen(mSourceFd, kBackLog);
                if (sock < 0) {
                    err = "TcpApi::Listen";
                    break;
                }
                mListenFd = sock;
                connSock = TcpApi::Accept(mListenFd);
                err = "TcpApi::Accept";
                break;
        }
        if (connSock < 0)
            throw std::runtime_error(err);

        return connSock;
    }

    TCPBuilder::TCPBuilder(TOPOLOGY_TYPE type, string ip, string port) : mListenFd(-1){
        mType = type;
        switch (type) {
            case CLIENT:
                mDestIp = ip;
                mDestPort = port;
                mSourceIp.clear();
                mSourceIp.clear();
                break;
            case SERVER:
                mDestIp.clear();
                mDestPort.clear();
                mSourceIp = ip;
                mSourcePort = port;
                break;
        }
    }

    int TCPBuilder::GetListenRsc() {
        return mListenFd;
    }

    TCPBuilder::TCPBuilder(string destination_ip, string destination_port, string source_ip, string source_port)
            : mType(CLIENT), mListenFd(-1), mDestIp(destination_ip), mDestPort(destination_port), mSourceIp(source_ip),
              mSourcePort(source_port) {
    }

} // doori