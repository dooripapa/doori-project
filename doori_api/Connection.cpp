// Copyright 2019 The doori-project Authors. All rights reserved.
// Use of this source code is governed by a GPL-3.0 license that can be found in the LICENSE file.
// See the AUTHORS file for names of contributors.
//
// Created by doori on 19. 7. 25.
//
#include "Connection.h"

using namespace std;

namespace doori{

auto Connection::release()-> void
{
	if (mBindSock != -1)
	{
	    close(mBindSock);
        mBindSock = -1;
	}
	if (mConnSock != -1 )
	{
		shutdown(mConnSock, SHUT_RDWR);
        mConnSock = -1;
	}
}

/**
*@brief 소켓으로 해당길이까지 데이터 수신
*@details
*@date
*@param
*@return -1 : unkonwn errorSt
         -2 : socket closed
*/
int Connection::waitDataUtill(int iSocketfd, string& emptyBuff, ssize_t requestedLen )
{
	/*
	* malloc은 주어진 크기도 보다 더 크게 잡는다
	*/
	char errorStr[1024]={0};
	char *pcContainer = (char*)malloc( requestedLen+1 );
	if( !pcContainer )
	{
		LOG(ERROR, "malloc fail");
		return -1;
	}
	memset( pcContainer, 0x00, sizeof( requestedLen+1 ) );
	char *pcPos = pcContainer;

	ssize_t	readedLen = 0;
	size_t	accumLen = requestedLen;

	while ( (readedLen = read(iSocketfd, pcPos, accumLen)) < requestedLen )
	{
		if( readedLen == -1 )
		{
			LOG(ERROR, "cant read data requested: ", strerror_r(errno, errorStr, sizeof(errorStr) ));
			if( pcContainer )
				free( pcContainer );
			return -1;
		}

		if (readedLen == 0)
		{
			LOG(INFO, "Socket FD : [", iSocketfd, "]", "closed");
			if( pcContainer )
				free( pcContainer );
			return -2;
		}

		LOG(DEBUG, "pcPos:",pcPos);

		pcPos += readedLen;
		accumLen -= readedLen;
		if( !accumLen )
			break;
	}
	*(pcContainer+requestedLen) = 0x00;
	emptyBuff.assign( pcContainer );

	if( pcContainer )
		free( pcContainer );

	return 0;
}

auto Connection::connectTo() -> int
{
    char	errorStr[1024]={0};
	int	    iRet = 0;

	if(!mDest.Set())
    {
	    LOG(ERROR, "not defined Destination, need to() function ");
	    return -1;
    }

    doori::Addr addr = mDest.Address();
	if(!mSource.Set()) {
        mBindSock = socket(addr.Domain(), SOCK_STREAM, 0);
        if(mBindSock < 0 )
        {
            LOG(ERROR, "Tcp socket fd, fail to open", strerror_r(errno, errorStr, sizeof(errorStr)));
            return -1;
        }
	} else{
        doori::Addr source_addr = mSource.Address();
        if (processBind(mBindSock, source_addr) < 0)
        {
            LOG(ERROR, "bind error");
            close(mBindSock);
            mBindSock = -1;
            return -1;
        }
	}

	iRet = connect(mBindSock , (struct sockaddr*)&( addr.getInetAddr() ) , sizeof(struct sockaddr_in) );
	if( iRet < 0 )
	{
		LOG(ERROR, "Tcp socket fd, fail to connect:", strerror_r(errno, errorStr, sizeof(errorStr)) );
        close(mBindSock);
        mBindSock = -1;
		return -1;
	}
	mConnSock = mBindSock;
	return mConnSock;
}

auto
Connection::send(const Stream& stream) -> int
{
	auto ret=0;
	ret=processSend(mConnSock, stream);
	if (ret < 0)
		LOG(ERROR, "send fail : [", mConnSock, "]");

	return ret;
}

int
Connection::sendTo(int socketFd, const Stream& stream)
{
	auto ret=0;
	ret=processSend(socketFd, stream);
	if (ret < 0)
		LOG(ERROR, "sendTo fail : [", socketFd, "]");

	return ret;
}

auto Connection::onListening() -> int
{
	auto ret=0;
	char errorStr[1024]={0};
	doori::Addr addr = mSource.Address();

	ret=processBind(mBindSock, addr);
	if (ret < 0)
	{
		LOG(ERROR, "onListening error");
		return ret;
	}

	if(listen(mBindSock, 1 ) < 0 )
	{
		LOG(ERROR, "Tcp socket fd, fail to listen:", strerror_r(errno, errorStr, sizeof(errorStr)));
		return -1;
	}
	return mBindSock;
}

int Connection::processBind(int& socketFd, const doori::Addr& addr)
{
	char errorStr[1024]={0};
	socketFd = socket(addr.Domain(), SOCK_STREAM, 0);
	if( socketFd < 0 )
	{
		LOG(ERROR, "Tcp socket fd, fail to open");
		return -1;
	}

	if( setsockopt(socketFd, SOL_SOCKET, SO_REUSEADDR, (void*)&( addr.getInetAddr() ) , sizeof( struct sockaddr_in )) < 0 )
	{
		LOG(ERROR, "Tcp socket fd, fail to setsockopt");
		return -1;
	}

	if( bind( socketFd, (struct sockaddr*)&( addr.getInetAddr() ), sizeof(struct sockaddr_in) ) < 0 )
	{
		LOG(ERROR, "Tcp socket fd, fail to bind:", strerror_r(errno, errorStr, sizeof(errorStr)));
		return -1;
	}
	return 0;
}

auto Connection::onAccepting() -> int
{
    if(!mSource.Set())
    {
        LOG(ERROR, "waitFor, need from()");
        return -1;
    }

	doori::Addr addr = mSource.Address();
	char errorStr[1024]={0};
	int	iRet = 0;
	socklen_t len = sizeof(struct sockaddr_in);
	if((iRet = accept(mBindSock, (struct sockaddr*)&( addr.getInetAddr() ), &len )) < 0 )
	{
		LOG(ERROR, "Tcp socket fd, fail to accept:", strerror_r(errno, errorStr, sizeof(errorStr)));
		return -1;
	}
	return iRet;
}

auto
Connection::waitFor( ) -> int
{
    if(!mSource.Set())
    {
        LOG(ERROR, "waitFor, need from()");
        return -1;
    }

	doori::Addr addr = mSource.Address();
	char errorStr[1024]={0};
	auto ret = 0;

	ret=processBind(mBindSock, addr);
	if (ret <0)
	{
		LOG(ERROR, "waitFor, binding error");
		return -1;
	}

	if(listen(mBindSock, 1 ) < 0 )
	{
		LOG(ERROR, "Tcp socket fd, fail to listen:", strerror_r(errno, errorStr, sizeof(errorStr)));
		return -1;
	}

	socklen_t len = sizeof(struct sockaddr_in);
	if((ret = accept(mBindSock, (struct sockaddr*)&( addr.getInetAddr() ), &len )) < 0 )
	{
		LOG(ERROR, "Tcp socket fd, fail to accept:", strerror_r(errno, errorStr, sizeof(errorStr)));
		return -1;
	}
    mConnSock = ret;
	return mConnSock;
}
///@see STX context start (0x02)\
        ETX context end (0x03)\
        STX LLLLLEEEEEEEE $|...doori_data...|$ ETX \
           123456789|123456789|123456789|
auto Connection::recv(Stream& container) -> int
{
	auto ret = 0;
	ret = processRecv(mConnSock, container);
	if (ret < 0)
		LOG(ERROR, "recv");

	return ret;
}

int Connection::recvFrom(int recvSocket, Stream& container)
{
	auto ret = 0;
	ret = processRecv(recvSocket, container);
	if (ret < 0)
		LOG(ERROR, "processRecv error :", ret);

	return ret;
}

auto
Connection::reply(const Stream& contents, Stream& response_contents) -> int
{
	auto ret=0;
	ret = processSend(mConnSock, contents);
	if (ret<0)
	{
		LOG(ERROR, "fail to send");
		return -1;
	}
	ret = processRecv(mConnSock, response_contents);
	if (ret<0)
	{
		LOG(ERROR, "fail to recv");
		return -1;
	}
	return 0;
}

/**
*@brief  소켓 send 처리하는 본체함수
*@details
*@date
*@param
*@return
	-3 : system error
	-2 : resource error
	-1 : user error
*@bug 
*@see
*/
int
Connection::processSend(int connected_socketFd, const Stream& contents)
{
	char errorStr[1024]={0};
	ssize_t len = 0;

	ssize_t requestLen = contents.toByteStream().length();
	if ( requestLen == 0 )
	{
		LOG(ERROR, "Contents length for Sending is zero");
		return -1;
	}
	ssize_t sendLen = requestLen;

	char *pcDataPos = (char*)malloc(contents.toByteStream().length()+1);
	if( !pcDataPos )
	{
		LOG(FATAL, "malloc error");
		return -2;
	}

	memcpy( pcDataPos, contents.toByteStream().c_str(), requestLen );
	*(pcDataPos+requestLen) = 0x00;

	while( (len+=write( connected_socketFd, pcDataPos, sendLen )) < requestLen )
	{
		if( len == -1 )
		{
			LOG(ERROR, "send door_stream to Destination, fail to write()", strerror_r(errno, errorStr, sizeof(errorStr) ) );
			if( pcDataPos )
				free(pcDataPos);
			return -2;
		}
		pcDataPos += len;
		sendLen -= len;
		if (!sendLen)
			break;
	}
	if( pcDataPos )
		free(pcDataPos);
	return 0;
}

///@see STX context start (0x02) \
        ETX context end (0x03) \
        STX LLLLLEEEEEEEE $|...doori_data...|$ ETX \
           123456789|123456789|123456789|
int Connection::processRecv(int connected_socketFd, Stream& recv_buffer)
{
	int iRet = 0;
	//헤더의 길이를 읽는다
	//STX가 존재하지 않으면 세션 종료
	ssize_t	bodyLen = 0;
	string	lenStr;

	do
	{
		// 길이가 저장된 버퍼을 우선 저장
		if( (iRet=waitDataUtill( connected_socketFd, lenStr, 6 )) < 0 )
		{
			LOG(ERROR, "waitDataUtill error : [", iRet, "]");
			break;
		}
		// 앞에 코드값 확인
		if( lenStr.at(0) != 0x02  )
		{
			LOG(ERROR, "STX not existing.");
			iRet = -1;
			break;
		}

		// stoi 사용할때 주의할점, 앞에 시작은 숫자문자열로 시작되어야 함
		string strTempLen(lenStr, 1, 6-1);
		string bodyStr;
		bodyLen = stoi( strTempLen );
		if( (iRet=waitDataUtill(connected_socketFd, bodyStr, bodyLen)) < 0 )
		{
			LOG(ERROR, "waitDataUtill error : [", iRet, "]");
			break;
		}
		// 바디부분 마지막문자타입 체크, 정합성체크
		if( bodyStr.at(bodyLen-1) != 0x03 )
		{
			LOG(ERROR, "ETX not existing.");
			iRet = -1;
			break;
		}

		//body 부분에 인코딩값 가져오기
		//-1 : 0x03's length
		string  encordingInfo( bodyStr, 0, 8 );
		string	dooriDataStr ( bodyStr, 8, bodyLen - encordingInfo.length() - 1 );
		if( recv_buffer.setString( encordingInfo, dooriDataStr ) < 0 )
		{
			LOG(ERROR, "setStream() fail.");
			return -1;
		}
	} while (false);

	if (iRet == -2)
		LOG(INFO, "Socket closed");

	return iRet;
}

auto Connection::setFrom(Endpoint source_endpoint) noexcept -> void {
    mSource = source_endpoint;
}

auto Connection::setTo(Endpoint dest_endpoint) noexcept -> void {
    mDest = dest_endpoint;
}

auto Connection::From() noexcept -> Endpoint&{
    return mSource;
}

auto Connection::To() noexcept -> Endpoint& {
    return mDest;
}


}
