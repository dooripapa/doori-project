//
// Created by jaeseong on 23. 6. 5.
//

#ifndef DOORI_PROJECT_STREAMTEMPLATE_H
#define DOORI_PROJECT_STREAMTEMPLATE_H


#include "IHeader.h"
#include "IBody.h"
#include <vector>
#include "Common/Error.h"
#include "Common/Util.h"

namespace doori::Stream {

    class StreamTemplate : Common::Error{
    public:
        StreamTemplate(IHeader& header, IBody& body);
        /**
         * Bytes를 리턴합니다.
         * @return vector<char>
         */
        vector<char> MakeStream() const;
        void ParserStream(vector<char>);
    private:
        IHeader& mHeader;
        IBody& mBody;
    };
};

#endif //DOORI_PROJECT_STREAMTEMPLATE_H
