//
// Created by jaeseong on 23. 2. 7.
//

#include "TCPConnection.h"

namespace doori {
    namespace CommunicationMember {
        int TCPConnection::WaitFor(DataStream::IStream &iStream) {

            if(mSourceFd < 0)
                return -1;



            return 0;
        }

        int TCPConnection::SendTo(DataStream::IStream iStream) {
            return 0;
        }

        TCPConnection::TCPConnection(int From, int To) : mSourceFd{From}, mDestFd{To} {

        }
    } // doori
} // CommunicationMember