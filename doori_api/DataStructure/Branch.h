// Copyright 2019 The doori-project Authors. All rights reserved.
// Use of this source code is governed by a GPL-3.0 license that can be found in the LICENSE file.
// See the AUTHORS file for names of contributors.
//
// Created by doori on 20. 7. 10.
//
#pragma once
#include <vector>
#include <ctime>
#include <chrono>

namespace doori{

template <typename T>
class Branch
{
public:
    /**
     * 기본생성자
     */
    Branch();
    /**
     * 복사생성자
     */
    Branch(const Branch&) noexcept ;
    /**
     * 이동생성자
     */
    Branch(Branch&&) noexcept ;
     /**
      * Branch 초기화합니다.
      * @param branchName : const std::string& 나뭇가지의 unique 이름을 지정함
      */
    Branch(const std::string& branchName) noexcept ;
    /**
     * 자신의 나뭇가지에 연결된 나뭇가지를 찾습니다.
     * @param branchName : const std::string& 찾고자 하는 나뭇가지 이름
     * @param branch : vector<Branch<T>>::iterator& 찾았을 경우, 나뭇가지 참조할 수 있는 값.
     * @return bool : 성공 실패
     */
    constexpr auto FindLinkBranches(const std::string& branchName, typename vector<Branch<T>>::iterator& branch) noexcept -> bool;
    /**
     * 자신의 나뭇가지에 연결된 나뭇가지를 찾습니다.
     * @param branchName : const std::string& 찾고자 하는 나뭇가지 이름
     * @return bool : 성공 실패
     */
    constexpr auto FindLinkBranches(const std::string& branchName) noexcept -> bool;
    /**
     * 자신의 나뭇가지의 unique 이름을 명시.
     * @param name 나뭇가지 unique 이름
     * @return void
     */
    constexpr auto SetName(const std::string& name) noexcept -> void;
    /**
     * 자신의 나뭇가지의 이름을 리턴함
     * @return const std::string& 자신의 나뭇가지 이름
     */
    auto GetName() const noexcept -> const std::string&;
    /**
     * 자신의 나뭇가지에 또 다른 나뭇가지가 연결되어 있다면, 참조값을 리턴함
     * @todo 참조값 리턴으로, 캡슐화 붕괴. 방안고려해야 함.
     * @return std::vector<Branch<T>>& 연결된 나뭇가지 리스트
     */
    auto GetLinkBranches() noexcept -> std::vector<Branch<T>>&;
    /**
     * 자신의 나뭇가지에 나뭇가지를 붙입니다.(Link)
     * 동일한 나뭇가지가 있다면 실패합니다.
     * @param branch : const Branch& 나뭇가지
     * @return bool : 성공, 실패
     */
    constexpr auto Link(const Branch& branch) noexcept -> bool;
    /**
     * 자신의 나뭇가지에 나뭇가지를 붙입니다.(Link)
     * 동일한 나뭇가지가 있다면 실패합니다.
     * @param branch : Branch&& 나뭇가지
     * @return bool : 성공, 실패
     */
    constexpr auto Link(Branch&&) noexcept -> bool;
    /**
     * 자신의 나뭇가지에 잎 리스트를 리턴합니다.
     * @return const std::vector<T>& 잎의 리스트 참조할 수 있는 값
     */
    constexpr auto GetLeaves() noexcept -> const std::vector<T>&;
    /**
     * 자신의 나뭇가지에 동일한 잎이 존재하는지 여부
     * @todo T가 operator== 없다면, 컴파일 시점에 명확한 오류 출력.
     * @return bool : ture 존재, false 없음
     */
    constexpr auto IsThereLeaf(const T&) noexcept -> bool;
    /**
     * 자신의 나뭇가지에 해당 잎을 제거합니다.
     * @return bool : true 제거 성공, false 제거 실패
     */
    constexpr auto DropLeaf(const T&) noexcept -> bool;
    /**
     * 자신의 나뭇가지에 해당 잎을 붙입니다. 복사연산자
     * @todo 중복된 잎은 처리 불가능하도록. 가능하도록 할 경우, DropLeaf 함수도 이에 맞게 수정되어야 함.
     * @return
     */
    constexpr auto AttachLeaf(const T&) noexcept -> void;
    /**
     * 자신의 나뭇가지에 해당 잎을 붙입니다. 이동연산자
     * @return
     */
    constexpr auto AttachLeaf(T&&) noexcept -> void;
    /**
     * 복사대입연산자
     * @param rhs : const Branch&
     * @return 자신의 참조값
     */
    constexpr auto operator=(const Branch& rhs) noexcept -> Branch&;
    /**
     * 이동대입연산자
     * @param rhs : Branch&&
     * @return 자신의 참조값
     */
    constexpr auto operator=(Branch&& rhs) noexcept -> Branch&;
    /**
     * 비교연산자
     * @param rhs : const Branch&
     * @return bool : true 같다, false 다르다
     */
    constexpr auto operator==(const Branch& rhs) const noexcept -> bool;
private:
    auto copyFrom(const Branch& rhs) noexcept -> void;
    auto copyFrom(Branch&& rhs) noexcept -> void;
    std::string mName;
    std::vector<Branch<T>> mLinkBranches;
    std::vector<T> mLeaves;
};


}//namespace doori

#include "Branch.hpp"
