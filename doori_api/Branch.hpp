// Copyright 2019 The doori-project Authors. All rights reserved.
// Use of this source code is governed by a GPL-3.0 license that can be found in the LICENSE file.
// See the AUTHORS file for names of contributors.
//
// Created by doori on 19. 7. 25.
//
#include <algorithm>
#include "Branch.h"

namespace doori{

//
// Created by doori on 19. 5. 1.
//
template<typename T>
Branch<T>::Branch() {  }

template <typename T>
Branch<T>::Branch(const Branch<T>& branch) noexcept {
    copyFrom(branch);
}

template<typename T>
Branch<T>::Branch(Branch && branch) noexcept {
    copyFrom(std::move(branch));
}

template<typename T>
Branch<T>::Branch(const std::string& topic_name) noexcept {
    mTopicName=topic_name;
}

///@brief
///@attention 이함수를 호출로 수정되었을 경우, updateAmendTime() 함수를 호출하여 시간동기화를 맞춰야 한다.
template <typename T>
constexpr auto Branch<T>::getLeaves() noexcept -> const std::vector<T>&
{
    return mLeaves;
}

///@brief T가 leaves존재하는지 여부
///@return false : 없음
///@return true : 존재함
///@todo T가 operator== 제공하지 않는다면, 컴파일 시점에 에러 출력 로직필요함
template <typename T>
constexpr auto Branch<T>::isThereLeaf(const T& t) noexcept -> bool
{
    auto lambda=
            [=](T member)
            {
                return (member==t);
            };
    return (find_if(mLeaves.begin(), mLeaves.end(), lambda) == mLeaves.end());
}

template <typename T>
constexpr auto Branch<T>::setTopic(const string& topic) noexcept -> void
{
    mTopicName = topic;
}

template <typename T>
auto Branch<T>::getTopic() const noexcept -> const string&
{
    return mTopicName;
}

///@todo std::vector<doori_branch<T>>&  타입을 리턴하면 안됨. 캡슐화 붕괴.
template <typename T>
auto Branch<T>::getLinkBranches() noexcept -> std::vector<Branch<T>>&
{
    return mLinkBranches;
}

template <typename T>
constexpr auto Branch<T>::attachLeaf(const T& t) noexcept -> void
{
    mLeaves.push_back(t);
}

template <typename T>
constexpr auto Branch<T>::attachLeaf(T&& t) noexcept -> void
{
    mLeaves.push_back(std::move(t));
}

template <typename T>
constexpr auto Branch<T>::operator=(const Branch<T>& rhs) noexcept -> Branch<T>&
{
    if( this ==&rhs  )
        return *this;

    copyFrom( rhs );
    return *this;
}

template <typename T>
constexpr auto Branch<T>::operator=(Branch<T>&& rhs) noexcept -> Branch<T>&
{
    if( this ==&rhs  )
        return *this;

    copyFrom( std::move(rhs) );
    return *this;

}

template <typename T>
constexpr auto Branch<T>::operator==(const Branch<T> &rhs) const noexcept -> bool
{
    return (mTopicName == rhs.mTopicName);
}

///@brief branch에서 branch로 연결된으로부터 같은 topic_name을 서치한다.
///@param topic_name 같은 branch를 위한 인자값
template <typename T>
constexpr auto Branch<T>::findLinkBranches(const std::string& topic_name, typename vector<Branch<T>>::iterator& iter) noexcept -> bool
{
    auto lambda=
            [=](const Branch<T>& branch)
            {
                return (branch.getTopic()==topic_name);
            };

    iter = find_if(mLinkBranches.begin(), mLinkBranches.end(), lambda);
    return !(iter == mLinkBranches.end());
}


template <typename T>
constexpr auto Branch<T>::findLinkBranches(const std::string& topic_name) noexcept -> bool
{
    auto lambda=
            [=](const Branch<T>& branch)
            {
                return (branch.getTopic()==topic_name);
            };

    auto iter = find_if(mLinkBranches.begin(), mLinkBranches.end(), lambda);
    return !(iter == mLinkBranches.end());
}

///@brief 자신의 Branch(나뭇가지)에 다른 나뭇가지를 연결한다.
template <typename T>
constexpr auto Branch<T>::link(const Branch<T>& branch) noexcept -> bool
{
    //link할 나뭇가지의 속성(TopicAccess) 동일한 나뭇가지 존재여부 확인
    if ( !findLinkBranches(branch.getTopic()) )
    {
        mLinkBranches.push_back(branch);
        return true;
    }
    return false;
}

template <typename T>
constexpr auto Branch<T>::link(Branch<T>&& branch) noexcept -> bool
{
    if ( !findLinkBranches(branch.getTopic()) )
    {
        mLinkBranches.push_back(move(branch));
        return true;
    }
    return false;
}

template <typename T>
auto Branch<T>::copyFrom(const Branch<T>& rhs) noexcept -> void
{
    mTopicName = rhs.mTopicName;
    mLeaves = rhs.mLeaves;
}

template <typename T>
auto Branch<T>::copyFrom(Branch<T>&& rhs) noexcept -> void
{
    mTopicName = std::move(rhs.mTopicName);
    mLeaves = std::move(rhs.mLeaves);
}

template<typename T>
constexpr auto Branch<T>::dropLeaf(const T& t) noexcept -> bool {
    auto lambda=
            [=](T member)
            {
                return (member==t);
            };
    auto it = find_if(mLeaves.begin(), mLeaves.end(), lambda);
    if(it == mLeaves.end())
        return false;
    else
        mLeaves.erase(it);
    return true;
}

}
