// Copyright 2019 The doori-project Authors. All rights reserved.
// Use of this source code is governed by a GPL-3.0 license that can be found in the LICENSE file.
// See the AUTHORS file for names of contributors.
//
// Created by doori on 19. 7. 25.
//
#pragma	once
#include "DataStream/Data.h"
#include "TnsdDistrict/Topic.h"
#include "CommunicationMember/Addr.h"


namespace doori::TnsdDistrict{

class Protocol_backup
{
public:
    enum class TREE{ PUB, SUB, NaN };
    enum class TYPE{NOTIFY, LIST, ALIVE, REPORT, CHANGE, NaN};
    enum class STATUS_CODE:unsigned int{ OK=0, ERR=1, NaN=2 };
    enum class FLOW{SEND, RESPONSE, NaN};

    enum class SEQ : int{
         TREE               =-1
        ,TOPIC              =-2
        ,IP                 =-3
        ,PORT               =-4
        ,REPORT             =-5
        ,MSG_CODE           =-6
        ,MSG_COMMENT        =-7
        ,MSG_TYPE           =-9
        ,ARR_SUM            =-999
        ,ARR_START          =-1000
    };
    static const string FATAL_ERR_MSG;
    static const string OK_MSG;
    static const string UNKOWN_MSG;
    static const string NOTIFY_MSG;
    static const string LIST_MSG;
    static const string CHANGE_MSG;
    static const string REPORT_MSG;
    static const string ALIVE_MSG;
    static const string PUB_MSG;
    static const string SUB_MSG;

    auto fromData(const Data &data) noexcept -> bool;
    auto toData() noexcept -> Data&;
    auto asNotify(TREE type, const Topic& topic, const doori::Addr&) noexcept -> void;
    auto asSender() noexcept -> void;
    auto asResponser() noexcept -> void;
    auto asAlive(TREE type, const Topic& topic, doori::Addr addr) noexcept -> void;
    auto asReport(TREE type, const Topic& topic) noexcept -> void;
    auto asList(TREE type, const Topic& topic) noexcept -> void;
    auto asChange(TREE type, const Topic& topic) noexcept -> void;

    auto appendSession(std::string ip, std::string port) noexcept -> void;
    auto completeSession() noexcept -> void;

    auto MsgType() noexcept  -> TYPE&;
    auto MsgCode() noexcept -> STATUS_CODE&;
    auto MsgComment() noexcept -> string&;
    auto ArraySum() noexcept -> int&;
    auto TreeAccess() noexcept -> TREE&;
    auto TopicAccess() noexcept -> Topic&;
    auto ReportComment() noexcept -> string&;
    auto Ip() noexcept -> string&;
    auto Port() noexcept -> string&;
private:
    auto convertToStr(TREE type) noexcept -> string;
    auto convertToStr(TYPE type) noexcept -> string;
    template <typename T>
    auto convertToStr(const T& t) noexcept -> string;
    auto convertMessageToType(const string& messageType) noexcept -> TYPE;
    auto convertTreeAccessToType(const string& treeAccessType) noexcept -> TREE ;
    auto convertStatusCodeToType(const string& statusCode) noexcept -> STATUS_CODE;

    Data mProtocolData;

    FLOW mFlowType=FLOW::NaN;
    TREE m1;
    Topic m2;
    string m3;
    string m4;
    string m5;
    STATUS_CODE m6=STATUS_CODE::NaN;
    string m7;
    TYPE m9;
    long m10=0L;
    int m999=0;
    std::vector<pair<string,string>> m1xxxList;
};


template <typename T>
auto Protocol_backup::convertToStr(const T &t) noexcept -> string {
    string  typeStr;
    if (is_same<TREE, T>::value)
    {
        switch(t)
        {
            case TREE::PUB:
                return string(PUB_MSG);
            case TREE::SUB:
                return string(SUB_MSG);
            default:
                return string(UNKOWN_MSG);
        }
    }

    if (is_same<TYPE, T>::value)
    {
        switch(t)
        {
            case TYPE::ALIVE:
                return string(ALIVE_MSG);
            case TYPE::REPORT:
                return string(REPORT_MSG);
            case TYPE::CHANGE:
                return string(CHANGE_MSG);
            case TYPE::LIST:
                return string(LIST_MSG);
            case TYPE::NOTIFY:
                return string(NOTIFY_MSG);
            default:
                return string(UNKOWN_MSG);
        }
    }
}

}//namespace doori
