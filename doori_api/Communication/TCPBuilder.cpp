//
// Created by jaeseong on 23. 1. 31.
//

#include "TCPBuilder.h"
#include "TcpApi.h"
#include "Common/Log.h"

namespace doori::api::Communication{

    TCPBuilder::~TCPBuilder() {

    }

    unique_ptr<IIPCTopology> TCPBuilder::GetTopology() {
        return make_unique<Socket>( mEstablishSocket );
    }

    TCPBuilder::TCPBuilder(Socket &socket) : mTcpApi(socket), mServer(false), mClient(false){

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
            LOG(INFO, "go on server");

            auto fd = SetServer(mBindingIp, mBindingPort, 10, 10);

            if (fd == -1)
            {
                LOG(ERROR, "SetServer error");
                return -1;
            }

            LOG(INFO, "Accept Socket[", fd, "]" );

            mEstablishSocket = Socket{fd, SOCK_STATUS::INIT};
            mEstablishSocket.SetBitwise(SOCK_STATUS::ESTABLISED);

            return 0;

        }
        // 클라이언트 역할. 연결요청만 함.
        else if ( !mServer && mClient )
        {
            LOG(INFO, "Not is binding && Client");

            mTcpApi.InitEndpoint();
            if(!mTcpApi.Status())
            {
                LOG(ERROR, "InitEndpoint error");
                return -1;
            }

            mTcpApi.Connect(mRemoteIp, mRemotePort, 10);
            if(!mTcpApi.Status())
            {
                LOG(ERROR, "Connect error");
                return -1;
            }

            auto connectedSocket = mTcpApi.GetSocket();
            mEstablishSocket = Socket{connectedSocket.GetFd(), SOCK_STATUS::INIT};
            mEstablishSocket.SetBitwise(SOCK_STATUS::ESTABLISED);

            return 0;

        }

        // 특정ip port 바인딩 후, 원격지에 연결요청함
        else if ( mServer && mClient )
        {
            LOG(INFO, "On binding && Client");

            auto fd = BindingClient(mBindingIp, mBindingPort, mRemoteIp, mRemotePort, 10);

            if (fd == -1)
            {
                LOG(ERROR, "BindingClient error");
                return -1;
            }

            mEstablishSocket = Socket{fd, SOCK_STATUS::INIT};
            mEstablishSocket.SetBitwise(SOCK_STATUS::ESTABLISED);

            return 0;
        }
        // 어떤것도 설정하지 않았음.
        else
        {
            LOG(ERROR, "Dont know Whether Server or Client");
            return -1;
        }
    }

    int TCPBuilder::SetServer(string ip, string port, int backlogNum, int timeout) {

        mTcpApi.InitEndpoint();

        if(!mTcpApi.Status())
        {
            LOG(ERROR, "InitEndpoint error");
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
            LOG(ERROR, "SetTimeoutOpt error");
            return -1;
        }

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

        auto accptSocket = mTcpApi.Accept();
        if(!mTcpApi.Status())
        {
            LOG(ERROR, "Accept error");
            return -1;
        }

        return accptSocket;

    }

    int
    TCPBuilder::BindingClient(string bindingIp, string bindingPort, string remoteIp, string remotePort, int timeout) {

        mTcpApi.InitEndpoint();
        if(!mTcpApi.Status())
        {
            LOG(ERROR, "InitEndpoint error");
            return -1;
        }

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

        mTcpApi.Connect(remoteIp, remotePort, timeout);
        if(!mTcpApi.Status())
        {
            LOG(ERROR, "Connect error");
            return -1;
        }

        auto connSocket = mTcpApi.GetSocket();

        return connSocket.GetFd();
    }

} // doori