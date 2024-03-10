//
// Created by jaeseong on 23. 6. 8.
//

#ifndef DOORI_PROJECT_UTIL_H
#define DOORI_PROJECT_UTIL_H

#include <vector>
#include <cstring>
#include <type_traits>
#include <concepts>

namespace doori::api::Common{

    /*
     * 구조체 타입만 허용됨.
     * 단, is_class_v 는 클래스를 허용함. 구조체도 클래스 범위에 포함됨.
     * is_pointer_v는 포안터 여부를 체크함.
     */
    template<typename T>
    concept StructConcept = std::is_class_v<T> && !std::is_pointer_v<T> && !std::is_same_v<T, std::string>;

    template<StructConcept T>
    std::vector<char> ToBytes(const T t) {

        //메모리 할당
        std::vector<char> charVector(sizeof(T));

        //복사
        std::memcpy(charVector.data(), &t, sizeof(T));

        return charVector;
    }
}

#endif //DOORI_PROJECT_UTIL_H
