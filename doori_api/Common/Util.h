//
// Created by jaeseong on 23. 6. 8.
//

#ifndef DOORI_PROJECT_UTIL_H
#define DOORI_PROJECT_UTIL_H

#include <vector>
#include <cstring>
#include <type_traits>
#include <concepts>

template<typename T>
concept StructConcept = std::is_class_v<T> && !std::is_pointer_v<T>;

template<typename T>
concept StringConcept = std::is_same_v<std::remove_cvref_t<T>, std::string> && !std::is_pointer_v<std::remove_cvref_t<T>>;

template<typename T>
requires StructConcept<T>
std::vector<char> ToBytes(const T& t) {

    //메모리 할당
    std::vector<char> charVector(sizeof(T));

    //복사
    std::memcpy(charVector.data(), &t, sizeof(T));

    return charVector;
}

template <int N>
void CopyToArray(char (&array)[N], string str)
{
    // 문자열을 null 종료 문자열로 복사합니다.
    std::strncpy(array, str.c_str(), sizeof(array) - 1);
    array[sizeof(array)- 1] = '\0';
}
#endif //DOORI_PROJECT_UTIL_H
