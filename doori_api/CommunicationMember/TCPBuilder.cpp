//
// Created by jaeseong on 23. 1. 31.
//

#include "TCPBuilder.h"
#include "TcpApi.h"
#include "Common/Log.h"

namespace doori::CommunicationMember {

    TCPBuilder::~TCPBuilder() {

    }

    unique_ptr<IIPCTopology> TCPBuilder::GetTopology() {
        return make_unique<Socket>( *dynamic_cast<Socket*>(&mSocket) );
    }

    TCPBuilder::TCPBuilder(Socket &socket) : mSocket(socket), mTcpApi(socket), mServer(false), mClient(false){

    }

    [[maybe_unused]] void TCPBuilder::From(string ip, string port) {
        mServer = true;
        mBindingIp = ip;
        mBindingPort = port;
    }

    [[maybe_unused]] void TCPBuilder::To(string ip, string port) {
        mClient = true;
        mRemoteIp = ip;
        mRemotePort = port;
    }

    int TCPBuilder::Establish() {

        // 서버역할, accept 요청오기를 기다림.
        if ( mServer && !mClient )
        {

            Socket accptSocket{};

            auto ret = SetServer(accptSocket, mBindingIp, mBindingPort, 10, 10);

            if (ret == -1)
            {
                LOG(ERROR, "SetServer error");
                return ret;
            }

            mSocket = accptSocket;

            return 0;

        }
        // 클라이언트 역할. 연결요청만 함.
        else if ( !mServer && mClient )
        {
            auto connSocket = mTcpApi.Connect(mRemoteIp, mRemotePort, 10);
            if(!mTcpApi.Status())
            {
                LOG(ERROR, "Connect error");
                return -1;
            }

            mSocket = connSocket;

            return 0;

        }
        // 특정ip port 바인딩 후, 원격지에 연결요청함
        else if ( mServer && mClient )
        {
            Socket connSocket{};

            auto ret = BindingClient(connSocket, mBindingIp, mBindingPort, mRemoteIp, mRemotePort, 10);

            if (ret == -1)
            {
                LOG(ERROR, "SetServer error");
                return ret;
            }

            mSocket = connSocket;

            return 0;
        }
        // 어떤것도 설정하지 않았음.
        else
        {
            LOG(ERROR, "Dont know Whether Server or Client");
            return -1;
        }
    }

    int TCPBuilder::SetServer(Socket& server, string ip, string port, int backlogNum, int timeout) {

        mTcpApi.Bind(ip, port);
        if(!mTcpApi.Status())
        {
            LOG(ERROR, "Binding error");
            return -1;
        }

        mTcpApi.Listen(backlogNum);
        if(!mTcpApi.Status())
        {
            LOG(ERROR, "Listen error");
            return -1;
        }

        mTcpApi.SetReuseOpt(ip, port);
        if(!mTcpApi.Status())
        {
            LOG(ERROR, "SetReuseOpt error");
            return -1;
        }

        mTcpApi.SetTimeoutOpt(timeout);
        if(!mTcpApi.Status())
        {
            LOG(ERROR, "SetReuseOpt error");
            return -1;
        }

        auto accptSocket = mTcpApi.Accept();

        server = accptSocket;

        return 0;

    }

    int TCPBuilder::BindingClient(Socket& client, string bindingIp, string bindingPort, string remoteIp, string remotePort, int timeout) {

        mTcpApi.Bind(bindingIp, bindingPort);
        if(!mTcpApi.Status())
        {
            LOG(ERROR, "Binding error");
            return -1;
        }

        mTcpApi.SetReuseOpt(bindingIp, bindingPort);
        if(!mTcpApi.Status())
        {
            LOG(ERROR, "SetReuseOpt error");
            return -1;
        }

        auto connSocket = mTcpApi.Connect(remoteIp, remotePort, timeout);
        if(!mTcpApi.Status())
        {
            LOG(ERROR, "Connect error");
            return -1;
        }

        client = connSocket;

        return 0;
    }

} // doori