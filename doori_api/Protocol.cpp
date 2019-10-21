// Copyright 2019 The doori-project Authors. All rights reserved.
// Use of this source code is governed by a GPL-3.0 license that can be found in the LICENSE file.
// See the AUTHORS file for names of contributors.
//
// Created by doori on 19. 7. 25.
//
#include "Protocol.h"

namespace doori{

const string Protocol::FATAL_ERR_MSG="fatal error";
const string Protocol::OK_MSG="O.K.";
const string Protocol::UNKOWN_MSG="NaN";
const string Protocol::NOTIFY_MSG="notify";
const string Protocol::LIST_MSG="list";
const string Protocol::CHANGE_MSG="change";
const string Protocol::REPORT_MSG="report";
const string Protocol::ALIVE_MSG="alive";
const string Protocol::PUB_MSG="pub";
const string Protocol::SUB_MSG="sub";

///@brief doori_data 값을 받아, doori_protocol 초기화
///@todo -1xxx fid값을, vector 전환해야 됨
auto Protocol::fromData(const Data &data) noexcept -> bool {
    bool bRet=true;

    Topic topic;
    mProtocolData.clear();
    mProtocolData = data;

    for(auto it=mProtocolData.cbegin(); it!=mProtocolData.cend();++it)
    {
        if ((*it).getFid() == static_cast<int>(SEQ::TREE) )
            m1 = convertTreeAccessToType( (*it).getValueToString());
        if ((*it).getFid() == static_cast<int>(SEQ::TOPIC) )
            if(!m2.set( (*it).getValueToString()))
                bRet = false;
        if ((*it).getFid() == static_cast<int>(SEQ::IP) )
            m3 = (*it).getValueToString();
        if ((*it).getFid() == static_cast<int>(SEQ::PORT) )
            m4 = (*it).getValueToString();
        if ((*it).getFid() == static_cast<int>(SEQ::REPORT) )
            m5 = (*it).getValueToString();
        if ((*it).getFid() == static_cast<int>(SEQ::MSG_CODE) )
            m6 = convertStatusCodeToType( (*it).getValueToString() );
        if ((*it).getFid() == static_cast<int>(SEQ::MSG_COMMENT) )
            m7 = (*it).getValueToString();
        if ((*it).getFid() == static_cast<int>(SEQ::MSG_TYPE) )
        {
            m9 = convertMessageToType( (*it).getValueToString() ) ;
            if(m9 == TYPE::NaN)
            {
                bRet = false;
                LOG(ERROR, "PROTOCOL type is NaN");
            }
        }
        if ((*it).getFid() == static_cast<int>(SEQ::ARR_SUM) )
            m999 = atoi( (*it).getValueToString().c_str() ) ;
        if ( (*it).getFid() == static_cast<int>(SEQ::ARR_START) )
        {
            Data data;
            data.clear();
            data.fromString( (*it).getValueToString() );

            auto itSession=data.find_if_Fid( static_cast<int>(SEQ::IP) );
            string ip = itSession->getValueToString();
            string port = data.find_if_Fid(itSession, static_cast<int>(SEQ::PORT) )->getValueToString();

            m1xxxList.push_back({ip, port});
        }
    }
    if(m999!=m1xxxList.size()) {
        bRet = false;
        LOG(ERROR, "The number of 1xxx is not matching column's 999 value");
    }
    return bRet;
}

///@brief Message Type별로 구성되는 메시지내용이 각각 다르다. \
///       응답용 protocol, 송신용 protocol 메시지내용은 구별된다.
///@return doori_data& 멤버객체를 리턴함
auto Protocol::toData() noexcept -> Data & {

    int idx= static_cast<int>(SEQ::ARR_START);
    mProtocolData.clear();

    if(mFlowType==FLOW::NaN)
    {
        LOG(ERROR, "PROTOCOL TYPE(SEND|REPONSE) is not defined");
        mProtocolData.clear();
        mProtocolData.append({static_cast<int>(SEQ::MSG_CODE), static_cast<int>(STATUS_CODE::ERR) });
        mProtocolData.append({static_cast<int>(SEQ::MSG_COMMENT),FATAL_ERR_MSG});
        return mProtocolData;
    }
    mProtocolData.append({static_cast<int>(SEQ::TREE)       , convertToStr(m1)});
    mProtocolData.append({static_cast<int>(SEQ::TOPIC)      , m2.getTopicName()});
    mProtocolData.append({static_cast<int>(SEQ::MSG_CODE)   , static_cast<int>(m6) });
    mProtocolData.append({static_cast<int>(SEQ::MSG_COMMENT), m7});
    mProtocolData.append({static_cast<int>(SEQ::MSG_TYPE)   , convertToStr(m9)});
    switch(m9)
    {
        case TYPE::NOTIFY:
        case TYPE::ALIVE:
            if(mFlowType==FLOW::SEND) {
                mProtocolData.append({static_cast<int>(SEQ::IP)  , m3});
                mProtocolData.append({static_cast<int>(SEQ::PORT), m4});
            }
            break;
        case TYPE::REPORT:
            if(mFlowType==FLOW::SEND) {
                mProtocolData.append({static_cast<int>(SEQ::REPORT) , m5});
            }
            break;
        case TYPE::CHANGE:
            if(mFlowType==FLOW::SEND) {
                mProtocolData.append({static_cast<int>(SEQ::TOPIC), m2.getTopicName()});
            }
            break;
        case TYPE::LIST:
            if(mFlowType==FLOW::RESPONSE) {
                mProtocolData.append({static_cast<int>(SEQ::ARR_SUM), m999});
                if (m999 > 0) {
                    Data session;
                    for (auto i:m1xxxList) {
                        session.append( {static_cast<int>(SEQ::IP)  , i.first});
                        session.append( {static_cast<int>(SEQ::PORT), i.second});
                    }
                    mProtocolData.append({idx--, session});
                    session.clear();
                }
            }
            break;
        case TYPE::NaN:
        default:
            if(mFlowType==FLOW::RESPONSE) {
                mProtocolData.clear();
                mProtocolData.append({static_cast<int>(SEQ::MSG_CODE)   , static_cast<int>(STATUS_CODE::ERR)});
                mProtocolData.append({static_cast<int>(SEQ::MSG_COMMENT), FATAL_ERR_MSG});
            }
            ;
    }
    return mProtocolData;
}

auto Protocol::MsgType() noexcept -> TYPE& {
    return m9;
}

///@brief list protocol 호출후에, 송신받았던 총 배열의 수를 리턴함
auto Protocol::ArraySum() noexcept -> int& {
    return m999;
}
auto Protocol::TreeAccess() noexcept -> TREE& {
    return m1;
}
auto Protocol::ReportComment() noexcept -> string& {
    return m5;
}
auto Protocol::TopicAccess() noexcept -> Topic & {
    return m2;
}


auto
Protocol::convertToStr(TYPE type) noexcept -> string
{
    switch(type)
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

auto
Protocol::convertToStr(TREE type) noexcept -> string
{
    string  typeStr;
    switch(type)
    {
        case TREE::PUB:
            typeStr = PUB_MSG;
            break;
        case TREE::SUB:
            typeStr = SUB_MSG;
            break;
        default:
            typeStr=UNKOWN_MSG;
    }
    return typeStr;
}


auto
Protocol::asNotify(TREE type, const Topic& topic, const doori::Addr& addr) noexcept
-> void
{
    mProtocolData.clear();
    m9 = TYPE::NOTIFY;
    m1 = type;
    m2 = topic;
    m3 = addr.Ip();
    m4 = addr.Port();
}

auto
Protocol::asAlive(TREE type, const Topic& topic, doori::Addr addr) noexcept
-> void
{
    mProtocolData.clear();
    m9 = TYPE::ALIVE;
    m1 = type;
    m2 = topic;
    m3 = addr.Ip();
    m4 = addr.Port();
}

auto
Protocol::asReport(TREE type, const Topic& topic) noexcept
-> void   
{
    mProtocolData.clear();
    m9 = TYPE::REPORT;
    m1 = type;
    m2 = topic;
}

auto
Protocol::asList(TREE type, const Topic& topic) noexcept -> void
{
    mProtocolData.clear();
    m9 = TYPE::LIST;
    m1 = type;
    m2 = topic;
}

auto
Protocol::asChange(TREE type, const Topic& topic) noexcept -> void
{
    mProtocolData.clear();
    m9 = TYPE::CHANGE;
    m1 = type;
    m2 = topic;
}

/**
*@brief protocol "list" 의 결과물을 만들기 위한 api
*@notice
        이 함수를 호출할때마다, sequence 추가(적재된다.)
*/
auto
Protocol::appendSession(std::string ip, std::string port) noexcept -> void {
    m999=0;
    pair<string,string> aPair{ip, port};
    m1xxxList.push_back(aPair);
    return;
}

/**
*@brief protocol "list" 의 결과물을 만들기 위한 api
*@notice
        함수 appendLIST 완료후, 이 함수로 완료한다.
*/
auto
Protocol::completeSession()  noexcept -> void {
    m999 = m1xxxList.size();
}

auto
Protocol::convertMessageToType(const string& messageType) noexcept -> TYPE
{
    if (messageType == NOTIFY_MSG){
        return TYPE::NOTIFY;
    }
    else if(messageType == LIST_MSG){
        return TYPE::LIST;
    }
    else if(messageType == REPORT_MSG){
        return TYPE::REPORT;
    }
    else if(messageType == ALIVE_MSG){
        return TYPE::ALIVE;
    }
    else if(messageType == CHANGE_MSG){
        return TYPE::CHANGE;
    }
    else
        return TYPE::NaN;
}

auto Protocol::convertTreeAccessToType(const string& treeAccessType) noexcept -> TREE
{
    if (treeAccessType == PUB_MSG){
        return TREE::PUB;
    }
    else if(treeAccessType == SUB_MSG){
        return TREE::SUB;
    }
    else
        return TREE::NaN;
}

auto Protocol::Ip() noexcept -> string & {
    return m3;
}

auto Protocol::Port() noexcept -> string & {
    return m4;
}

auto Protocol::asSender() noexcept -> void {
    mFlowType=FLOW::SEND;
}

auto Protocol::asResponser() noexcept -> void {
    mFlowType=FLOW::RESPONSE;
}

auto Protocol::MsgCode() noexcept -> STATUS_CODE& {
    return m6;
}

auto Protocol::MsgComment() noexcept -> string & {
    return m7;
}

auto Protocol::convertStatusCodeToType(const string& statusCode) noexcept -> STATUS_CODE
{
    if (statusCode == "0"){
        return STATUS_CODE::OK;
    }
    else if(statusCode == "1"){
        return STATUS_CODE::ERR;
    }
    else
        return STATUS_CODE::NaN;
}

}//namespace doori
