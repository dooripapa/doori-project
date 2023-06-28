// Copyright 2019 The doori-project Authors. All rights reserved.
// Use of this source code is governed by a GPL-3.0 license that can be found in the LICENSE file.
// See the AUTHORS file for names of contributors.
//
// Created by doori on 19. 7. 25.
//
#ifndef DOORI_PROJECT_TREE_H
#define DOORI_PROJECT_TREE_H

#include <string>
#include <functional>
#include <unordered_map>
#include <ctime>
#include <chrono>
#include "Branch.h"
#include "Tnsd/Topic.h"

using namespace std;
using namespace doori::api::Tnsd;

namespace doori::api::DataStructure{

    /**
     * FORCE 확인 없이 update/insert
     * CHECK 확인, 존재하면 fail
     */
    enum class INSERT{FORCE, CHECK};

    template <typename T>
    class Tree
    {
    public:

        Tree();
        Tree(const Tree& rhs);
        Tree(Tree&& rhs);

        /**
         * Tree객체에 데이터구조 나뭇가지를 붙인다.
         * @note INSERT 값이 FORCE이면 이미 존재하더라도, overwrite, CHECK 이면 실패를 리턴
         * @param dataKey : const ILeafKey&
         * @param branch : const Branch<T>&
         * @return true 성공, fail 실패
         */
        auto attachBranch(const ILeafKey& dataKey, const Branch<T>& branch, INSERT) noexcept -> bool;

        /**
         * Topic해당되는 나뭇가지를 제거함
         * @param dataKey const ILeafKey&
         * @return 성공적으로 제거되면 true, 제거된게 없다면 fail
         */
        auto removeBranch(const ILeafKey& dataKey) noexcept -> bool;

        /**
         * Topic해당되는 나뭇가지를 리턴함
         * @param dataKey const ILeafKey&
         * @return Branch<T>&
         */
        auto getBranch(const ILeafKey& dataKey) -> Branch<T>&;

        /**
         * Topic해당되는 나뭇가지에 잎을 attach함
         * @param dataKey  const ILeafKey&
         * @param leaf T
         * @return true 잎 attach성공, fail attach하지 못함
         */
        auto attachLeaf(const ILeafKey& dataKey, T leaf) -> bool;

        /**
         * 복사대입연산자
         * @param rhs
         * @return
         */
        auto operator=(const Tree& rhs) noexcept -> Tree<T>&;

        /**
         * 이동대입연산자
         * @param rhs
         * @return
         */
        auto operator=(Tree<T>&& rhs) noexcept -> Tree<T>&;

        /**
         * Branch 최초(조상)의 Root격 branch를 리턴함
         * @return
         */
        auto RootBranches() noexcept -> std::vector< Branch<T> >&;

    private:
        auto findRootBranches(const string& topic_name_segment, typename vector<Branch<T>>::iterator& itPos) -> bool;
        auto findRootBranches(const string& topic_name_segment) -> bool;
        auto copyFrom(Tree&& rhs) -> void;
        auto copyFrom(const Tree& rhs) -> void;
        std::vector< Branch<T> >   mRootBranches;
    };

}//namespace doori

#include "Tree.hpp"

#endif //DOORI_PROJECT_TREE_H
