// Copyright 2019 The doori-project Authors. All rights reserved.
// Use of this source code is governed by a GPL-3.0 license that can be found in the LICENSE file.
// See the AUTHORS file for names of contributors.
//
// Created by doori on 19. 7. 25.
//
#include <algorithm>
#include "DataStructure/Branch.h"

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
    mName=topic_name;
}

template <typename T>
constexpr auto Branch<T>::GetLeaves() noexcept -> const std::vector<T>&
{
    return mLeaves;
}

template <typename T>
constexpr auto Branch<T>::IsThereLeaf(const T& t) noexcept -> bool
{
    auto lambda=
            [=](T member)
            {
                return (member==t);
            };
    return (find_if(mLeaves.begin(), mLeaves.end(), lambda) == mLeaves.end());
}

template <typename T>
constexpr auto Branch<T>::SetName(const std::string &topic) noexcept -> void
{
    mName = topic;
}

template <typename T>
auto Branch<T>::GetName() const noexcept -> const string&
{
    return mName;
}

template <typename T>
auto Branch<T>::GetLinkBranches() noexcept -> std::vector<Branch<T>>&
{
    return mLinkBranches;
}

template <typename T>
constexpr auto Branch<T>::AttachLeaf(const T& t) noexcept -> void
{
    mLeaves.push_back(t);
}

template <typename T>
constexpr auto Branch<T>::AttachLeaf(T&& t) noexcept -> void
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
    return (mName == rhs.mName);
}

template <typename T>
constexpr auto Branch<T>::FindLinkBranches(const std::string& topic_name, typename vector<Branch<T>>::iterator& iter) noexcept -> bool
{
    auto lambda=
            [=](const Branch<T>& branch)
            {
                return (branch.GetName() == topic_name);
            };

    iter = find_if(mLinkBranches.begin(), mLinkBranches.end(), lambda);
    return !(iter == mLinkBranches.end());
}


template <typename T>
constexpr auto Branch<T>::FindLinkBranches(const std::string& topic_name) noexcept -> bool
{
    auto lambda=
            [=](const Branch<T>& branch)
            {
                return (branch.GetName() == topic_name);
            };

    auto iter = find_if(mLinkBranches.begin(), mLinkBranches.end(), lambda);
    return !(iter == mLinkBranches.end());
}

template <typename T>
constexpr auto Branch<T>::Link(const Branch &branch) noexcept -> bool
{
    //link할 나뭇가지의 속성(TopicAccess) 동일한 나뭇가지 존재여부 확인
    if ( !FindLinkBranches(branch.GetName()) )
    {
        mLinkBranches.push_back(branch);
        return true;
    }
    return false;
}

template <typename T>
constexpr auto Branch<T>::Link(Branch &&branch) noexcept -> bool
{
    if ( !FindLinkBranches(branch.GetName()) )
    {
        mLinkBranches.push_back(move(branch));
        return true;
    }
    return false;
}

template <typename T>
auto Branch<T>::copyFrom(const Branch<T>& rhs) noexcept -> void
{
    mName = rhs.mName;
    mLeaves = rhs.mLeaves;
}

template <typename T>
auto Branch<T>::copyFrom(Branch<T>&& rhs) noexcept -> void
{
    mName = std::move(rhs.mName);
    mLeaves = std::move(rhs.mLeaves);
}

template<typename T>
constexpr auto Branch<T>::DropLeaf(const T& t) noexcept -> bool {
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
