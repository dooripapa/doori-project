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
#include "DataSegment.h"

namespace doori::api::Data{

class Data
{
public:
    Data();
	Data(const Data& rhs);
	Data(Data&& rhs);
	Data(const DataSegment& dataSegment);
	Data(DataSegment&& dataSegment);
	virtual ~Data();
	auto clear() noexcept -> void;
	auto append(const DataSegment& dataSegment) -> void;
	auto append(DataSegment&& dataSegment) -> void;
	auto sort()	-> void;
	auto walk( std::ostream& outPut ) -> void;
	auto walk( ) const -> void;
	auto toString() const -> std::string;
	auto fromString(std::string doori_data_shaped_string) -> int;
	auto operator=(const Data& rhs)-> Data&;
	auto operator=(Data&& rhs)-> Data&;
	auto begin() noexcept -> vector<DataSegment>::iterator;
	auto end() noexcept -> vector<DataSegment>::iterator;
	auto cbegin() noexcept -> const vector<DataSegment>::iterator;
	auto cend() noexcept -> const vector<DataSegment>::iterator;
	auto find_if_Fid(int fid) -> vector<DataSegment>::iterator;
	auto find_if_Fid(vector<DataSegment>::iterator it, int fid) -> vector<DataSegment>::iterator;
protected:
private:
	auto copyFrom(const Data& rhs) noexcept ->void;
	auto copyFrom(Data&& rhs) noexcept ->void;
	std::vector<DataSegment> segments;
};

}
