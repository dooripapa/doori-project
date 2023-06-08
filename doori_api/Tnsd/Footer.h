//
// Created by jaeseong on 23. 6. 8.
//

#ifndef DOORI_PROJECT_FOOTER_H
#define DOORI_PROJECT_FOOTER_H

#include <string>
#include "Stream/IFooter.h"

namespace doori {
    namespace Tnsd {

        class Footer : public Stream::IFooter{

        public:
            Footer(string stream = {0x02}); //ETX

            vector<char> Get() override;

            long GetLength() override;

        private:
            string mStream;

        };

    } // doori
} // Tnsd

#endif //DOORI_PROJECT_FOOTER_H
