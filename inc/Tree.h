// Copyright 2019 The doori-project Authors. All rights reserved.
// Use of this source code is governed by a GPL-3.0 license that can be found in the LICENSE file.
// See the AUTHORS file for names of contributors.
//
// Created by doori on 19. 7. 25.
//
#pragma once
//forward declaration
#include <string>
#include <functional>
#include <unordered_map>
#include <ctime>
#include <chrono>
#include "Branch.h"
#include "Topic.h"

namespace doori{

template <typename T> //T:Leaf object
class Tree
{
public:
    enum class INSERT{FORCE, CHECK};
    Tree();
	Tree(const Tree& rhs);
	Tree(Tree&& rhs);
	auto attachBranch(const Topic& topic, const Branch<T>& branch, INSERT) noexcept -> bool;
    auto removeBranch(const Topic& topic) noexcept -> bool;
	auto getBranch(const Topic& topic) -> Branch<T>&;
	auto attachLeaf(const Topic& topic, T leaf) -> bool;
	auto operator=(const Tree& rhs) noexcept -> Tree<T>&;
    auto operator=(Tree<T>&& rhs) noexcept -> Tree<T>&;
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
