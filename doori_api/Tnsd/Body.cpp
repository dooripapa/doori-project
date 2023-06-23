//
// Created by jaeseong on 23. 6. 8.
//

#include "Body.h"

namespace doori::api::Tnsd {
    long Body::GetLength() const {
        return mJson.serialize().length();
    }

    vector<char> Body::ToStream() {
        auto json = mJson.serialize();
        return vector<char>{begin(json), end(json)};
    }

    void Body::InternalError(string error) {
        mJson.clear();
        mJson["error"] = error;
    }

    void Body::Notify(const NodeInfo& myNodeInfo) {
        mJson.clear();
        mJson["topic"] = myNodeInfo.GetTopic();
        mJson["side"] = myNodeInfo.GetSide();
        mJson["ip"] = myNodeInfo.GetIp();
        mJson["port"] = myNodeInfo.GetPort();
    }

    void Body::Anwser(string hash16) {
        mJson.clear();
        mJson["key"] = hash16;
    }

    void Body::Anwser(vector<NodeInfo> publisherList) {

        mJson.clear();
        Data::Json nodeInfoJson{};

        auto iter = publisherList.cbegin();
        for (int i = 0 ; iter != publisherList.cend() ; iter++, i++) {
            nodeInfoJson["topic"] = iter->GetTopic();
            nodeInfoJson["side"] = iter->GetSide();
            nodeInfoJson["ip"] = iter->GetIp();
            nodeInfoJson["port"] = iter->GetPort();

            mJson.append(to_string(i), nodeInfoJson);
        }
    }

    void Body::Change(NodeInfo publisher) {

        mJson.clear();
        mJson["topic"] = publisher.GetTopic();
        mJson["side"] = publisher.GetSide();
        mJson["ip"] = publisher.GetIp();
        mJson["port"] = publisher.GetPort();
    }

    void Body::Close(NodeInfo myNodeInfo) {

        mJson.clear();
        mJson["topic"] = myNodeInfo.GetTopic();
        mJson["side"] = myNodeInfo.GetSide();
        mJson["ip"] = myNodeInfo.GetIp();
        mJson["port"] = myNodeInfo.GetPort();
    }

    void Body::Publish(Data::Json json) {

        mJson.clear();

        mJson = json;
    }

    void Body::Alive(string hash16) {

        mJson.clear();

        mJson["key"] = hash16;
    }

    void Body::Report(Data::Json json) {

        mJson.clear();

        mJson = json;
    }

    int Body::FromStream(string buffer) {

        if(!mJson.unserialize(buffer))
            return -1;

        return 0;
    }
} // Tnsd