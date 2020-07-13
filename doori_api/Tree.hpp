// Copyright 2019 The doori-project Authors. All rights reserved.
// Use of this source code is governed by a GPL-3.0 license that can be found in the LICENSE file.
// See the AUTHORS file for names of contributors.
//
// Created by doori on 19. 7. 25.
//

#include "Tree.h"

namespace doori{

template <typename T>
auto Tree<T>::copyFrom(const Tree<T>& rhs) -> void
{
    mRootBranches = std::move(rhs.mRootBranches);
}

template <typename T>
auto Tree<T>::copyFrom(Tree<T>&& rhs) -> void
{
    mRootBranches = std::move(rhs.mRootBranches);
}

template <typename T>
Tree<T>::Tree() {

}

template <typename T>
Tree<T>::Tree(const Tree<T>& rhs)
{
    copyFrom(rhs);
}

template <typename T>
Tree<T>::Tree(Tree<T>&& rhs)
{
    copyFrom(std::move(rhs));
}

///@brief doori_topic 와 같은 branch를 찾기
///@details
///@date
///@param searchVector search대상 vector
///@param topic_name_segment topic name 부분
///@return tuple<bool, typename std::vector< doori_branch<T>>::iterator > 찾으면 첫인자는 true, 못찾으면 false, iterator는 대상 branch
///@see
template <typename T>
auto Tree<T>::findRootBranches(const string& topic_name_segment, typename vector<Branch<T>>::iterator& itPos) -> bool
{
    auto it=find_if(
             mRootBranches.begin()
            ,mRootBranches.end()
            ,[=](const Branch<T>& branch){
                return (branch.getName()==topic_name_segment);
            }
    );
    auto ret=!(it==mRootBranches.end()); //찾으면 true, or false
    itPos=it;

    return ret;
}
template <typename T>
auto Tree<T>::findRootBranches(const string& topic_name_segment) -> bool
{
    auto it=find_if(
             mRootBranches.begin()
            ,mRootBranches.end()
            ,[=](const Branch<T>& branch){
                return (branch.getName() == topic_name_segment);
            }
    );
    return !(it==mRootBranches.end()); //찾으면 true, or false
}

///@brief 인자로 받은 doori_topic을 이용하여, branch를 탐색, 인자로 받은 branch를 Link함
///@details
///@date
///@param topic doori_topic 객체
///@param branch Link를 대상 branch
///@return true Link작업 완료
///@return false Duplicated Link된 branch로 인해 작업중단
///@see
template <typename T>
auto Tree<T>::attachBranch(const Topic& topic, const Branch<T>& branch, INSERT mode) noexcept -> bool
{
    auto ret=false;
    typename vector<Branch<T>>::iterator iter, linkIter;
    for(auto i=0;i<topic.getDepthSize();++i)
    {
        if(i==0) {
            ret=findRootBranches( topic.getTopicName(i) ,iter );
            if(!ret) {
                Branch<T> aBranch(topic.getTopicName(i));
                mRootBranches.push_back(aBranch);
                findRootBranches( topic.getTopicName(i) ,iter );
            }
        } else{
            ret = (*iter).findLinkBranches( topic.getTopicName(i), linkIter );
            if(!ret) {
                Branch<T> aBranch(topic.getTopicName(i));
                (*iter).getLinkBranches().push_back(aBranch);
                (*iter).findLinkBranches( topic.getTopicName(i), linkIter );
            }
            iter=linkIter;
        }
    }
    //해당 Branch에서 Linked Branches로부터 해당 중복된 branch가 있는지 확인함
    ret = (*iter).findLinkBranches(branch.getName(), linkIter);
    if( ret ) {
        if (mode==INSERT::CHECK)
            return false;
        else {
            (*iter).getLinkBranches().erase(linkIter);
            (*iter).getLinkBranches().push_back(branch);
        }
    }
    else
        (*iter).getLinkBranches().push_back(branch);

    return true;
}

///@brief doori_topic 한개의 branch를 삭제, 이와 연결된 모든 branches도 삭제됨
///@details
///@date
///@param topic doori_topic 객체
///@return true 지웠다면 true
///@return false 지운게 없다면 false
///@see
template <typename T>
auto Tree<T>::removeBranch(const Topic& topic) noexcept -> bool
{
    int i;
    typename vector<Branch<T>>::iterator iter, linkIter;
    auto ret=false;
    auto depthMax = topic.getDepthSize();
    for(i=0;i<depthMax;++i)
    {
        if(i==0)
            ret=findRootBranches( topic.getTopicName(i), iter );
        else
            ret=(*iter).findLinkBranches( topic.getTopicName(i), linkIter );

        if(ret) {
            if(0<i && i<depthMax-1)
                iter = linkIter;
            continue;
        }
        else
            break;
    }
    if(i==topic.getDepthSize() && ret){
        (*iter).getLinkBranches().erase( linkIter );
        LOG(INFO, "remove Branch TopicAccess:", topic.getTopicName());
        return true;
    }
    return false;
}

///@brief doori_topic에 해당하는  branch를 리턴함, 해당 branch가 존재하지 않으면 \
///       생성된 branch 를 리턴함
///@param topic doori_topic, whole topic string.(not segment topic name)
template <typename T>
auto Tree<T>::getBranch(const Topic& topic) -> Branch<T>&
{
    typename vector<Branch<T>>::iterator iter, linkIter;
    auto ret=false;
    for(auto i=0;i<topic.getDepthSize();++i)
    {
        if(i==0) {
            ret=findRootBranches(topic.getTopicName(i), iter);
            //없으면 새로 branch 추가
            if(!ret) {
                Branch<T> aBranch(topic.getTopicName(i));
                mRootBranches.push_back(aBranch);
                findRootBranches(topic.getTopicName(i), iter);
                continue;
            }
        } else{
            ret=(*iter).findLinkBranches( topic.getTopicName(i), linkIter );
            //없으면 새로 branch 추가
            if(!ret) {
                Branch<T> aBranch(topic.getTopicName(i));
                (*iter).getLinkBranches().push_back(aBranch);
                (*iter).findLinkBranches(topic.getTopicName(i), linkIter);
            }
            iter=linkIter;
        }
    }
    return (*iter);
}

///@brief doori_topic의  해당되는 branch에 leaf을 attach
///@param topic doori_topic
///@param leaf  doori_tree에 attach할 객체
template <typename T>
auto Tree<T>::attachLeaf(const Topic& topic, T leaf) -> bool
{
    //auto& ...
    //이렇게 해야, var 변수의 변경은, 값의 변경이 아닌, 참조의 값의 변환이 된다.
    auto& var = getBranch(topic);

    if(!var.isThereLeaf(leaf))
        return false;

    var.attachLeaf(leaf);
    return true;
}

template <typename T>
auto Tree<T>::operator=(const Tree<T>& rhs) noexcept -> Tree<T>&
{
    if( this ==&rhs  )
        return *this;

    copyFrom( rhs);
    return *this;
}
template <typename T>
auto Tree<T>::operator=(Tree<T>&& rhs) noexcept -> Tree<T>&
{
    if( this ==&rhs  )
        return *this;

    copyFrom( std::move(rhs) );
    return *this;
}

template<typename T>
auto Tree<T>::RootBranches() noexcept -> vector<Branch<T>> & {
    return mRootBranches;
}

}//namespace doori
