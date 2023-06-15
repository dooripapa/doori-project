// Copyright 2019 The doori-project Authors. All rights reserved.
// Use of this source code is governed by a GPL-3.0 license that can be found in the LICENSE file.
// See the AUTHORS file for names of contributors.
//
// Created by doori on 19. 7. 25.
//
#pragma once
#include <bitset>
#include <vector>
#include <algorithm>    // std::sort
#include "FidSegment.h"

namespace doori::api::Data{

class Fid
{
public:
    Fid();
	Fid(const Fid& rhs);
	Fid(Fid&& rhs);
	Fid(const FidSegment& dataSegment);
	Fid(FidSegment&& dataSegment);
	virtual ~Fid();
	auto clear() noexcept -> void;
	auto append(const FidSegment& dataSegment) -> void;
	auto append(FidSegment&& dataSegment) -> void;
	auto sort()	-> void;
	auto walk( std::ostream& outPut ) -> void;
	auto walk( ) const -> void;
	auto toString() const -> std::string;
	auto fromString(std::string doori_data_shaped_string) -> int;
	auto operator=(const Fid& rhs)-> Fid&;
	auto operator=(Fid&& rhs)-> Fid&;
	auto begin() noexcept -> vector<FidSegment>::iterator;
	auto end() noexcept -> vector<FidSegment>::iterator;
	auto cbegin() noexcept -> const vector<FidSegment>::iterator;
	auto cend() noexcept -> const vector<FidSegment>::iterator;
	auto find_if_Fid(int fid) -> vector<FidSegment>::iterator;
	auto find_if_Fid(vector<FidSegment>::iterator it, int fid) -> vector<FidSegment>::iterator;
protected:
private:
	auto copyFrom(const Fid& rhs) noexcept ->void;
	auto copyFrom(Fid&& rhs) noexcept ->void;
	std::vector<FidSegment> segments;
};

}
