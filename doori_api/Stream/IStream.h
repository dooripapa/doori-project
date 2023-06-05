//
// Created by jaeseong on 23. 6. 5.
//

#ifndef DOORI_PROJECT_ISTREAM_H
#define DOORI_PROJECT_ISTREAM_H


#include "IHeader.h"
#include "IBody.h"
#include "IFooter.h"

namespace doori::Stream {

    class IStream {
    public:
        IStream(IHeader& header, IBody& body, IFooter& footer) : mHeader(header), mBody(body), mFooter(footer){};
        /**
         * Byte를 리턴합니다.
         * @return
         */
        virtual vector<char> GetStream() = 0;

        virtual IHeader& GetHeader() {return mHeader;};
        virtual IBody& GetBody() {return mBody;};
        virtual IFooter& GetFooter() {return mFooter;};
    private:
        IHeader& mHeader;
        IBody& mBody;
        IFooter& mFooter;
    };
};

#endif //DOORI_PROJECT_ISTREAM_H
