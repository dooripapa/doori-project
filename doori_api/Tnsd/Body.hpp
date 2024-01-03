//
// Created by jaeseong on 23. 6. 8.
//

#include "Body.h"

namespace doori::api::Tnsd {

    template<typename T>
    long Body<T>::GetLength() const {
        return mData.serialize().length();
    }

    template<typename T>
    vector<char> Body<T>::ToStream() {
        auto json = mData.serialize();
        return vector<char>{begin(json), end(json)};
    }

    template<typename T>
    void Body<T>::InternalError(string error) {
        mData.clear();
        mData["error"] = error;
    }

    template<typename T>
    void Body<T>::Notify(string topic, string side, string ip, string port) {
        mData.clear();
        mData["topic"] = topic;
        mData["side"] = side;
        mData["ip"] = ip;
        mData["port"] = port;
    }

    template<typename T>
    void Body<T>::Anwser(string hash16) {
        mData.clear();
        mData["key"] = hash16;
    }

    template<typename T>
    void Body<T>::Anwser(string hash16, string info) {
        mData.clear();
        mData["key"] = hash16;
        mData["info"] = info;
    }


    template<typename T>
    void Body<T>::Change(string topic, string side, string ip, string port) {

        mData.clear();
        mData["topic"] = topic;
        mData["side"] = side;
        mData["ip"] = ip;
        mData["port"] = port;
    }

    template<typename T>
    void Body<T>::Close(string topic, string side, string ip, string port) {

        mData.clear();
        mData["topic"] = topic;
        mData["side"] = side;
        mData["ip"] = ip;
        mData["port"] = port;
    }

    template<typename T>
    void Body<T>::Publish(T json) {

        mData.clear();

        mData = json;
    }

    template<typename T>
    void Body<T>::Alive(string hash16) {

        mData.clear();

        mData["key"] = hash16;
    }

    template<typename T>
    void Body<T>::Report(T json) {

        mData.clear();

        mData = json;
    }

    template<typename T>
    int Body<T>::FromStream(const string& buffer) {

        if(!mData.unserialize(buffer))
            return -1;

        return 0;
    }

    template<typename T>
    const T &Body<T>::GetBody() const {
        return mData;
    }

} // Tnsd