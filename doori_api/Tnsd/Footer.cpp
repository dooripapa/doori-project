//
// Created by jaeseong on 23. 6. 8.
//

#include "Footer.h"

namespace doori {
    namespace Tnsd {
        vector<char> Footer::Get() {
            return vector<char>( {mStream.cbegin(), mStream.cend()});
        }

        Footer::Footer(string stream) : mStream{stream}{

        }

        long Footer::GetLength() {
            return mStream.length();
        }
    } // doori
} // Tnsd