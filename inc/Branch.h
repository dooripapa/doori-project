// Copyright 2019 The doori-project Authors. All rights reserved.
// Use of this source code is governed by a GPL-3.0 license that can be found in the LICENSE file.
// See the AUTHORS file for names of contributors.
//
// Created by doori on 19. 7. 25.
//
#pragma once
#include <vector>
#include <ctime>
#include <chrono>
#include "Topic.h"

namespace doori{

template <typename T>
class Branch
{
public:
    Branch();
    Branch(const Branch&) noexcept ;
    Branch(Branch&&) noexcept ;
    Branch(const std::string&) noexcept ;

    constexpr auto findLinkBranches(const std::string& topic_segment, typename vector<Branch<T>>::iterator&) noexcept -> bool;
    constexpr auto findLinkBranches(const std::string& topic_segment) noexcept -> bool;
    constexpr auto setName(const std::string& topic_segment) noexcept -> void;
    auto getName() const noexcept -> const std::string&;
    auto getLinkBranches() noexcept -> std::vector<Branch<T>>&;
    constexpr auto link(const Branch&) noexcept -> bool;
    constexpr auto link(Branch&&) noexcept -> bool;
    constexpr auto getLeaves() noexcept -> const std::vector<T>&;
    constexpr auto isThereLeaf(const T&) noexcept -> bool;
    constexpr auto dropLeaf(const T&) noexcept -> bool;
    constexpr auto attachLeaf(const T&) noexcept -> void;
    constexpr auto attachLeaf(T&&) noexcept -> void;

    constexpr auto operator=(const Branch& rhs) noexcept -> Branch&;
    constexpr auto operator=(Branch&& rhs) noexcept -> Branch&;
    constexpr auto operator==(const Branch& rhs) const noexcept -> bool;
private:
    auto copyFrom(const Branch& rhs) noexcept -> void;
    auto copyFrom(Branch&& rhs) noexcept -> void;
    std::string mTopicName;
    std::vector<Branch<T>> mLinkBranches;
    std::vector<T> mLeaves;
};


}//namespace doori

#include "Branch.hpp"
