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

/**
 * @todo 설계구조 심각한 버그로 이 객체는 사용불가함. 전체를 변경해야 됨.
 * @note 아래와 같이 변경할것.
 * ^<fid>,<IFDBSR>=Value| ...$
 * ^<fid>,<IFDBSR>=Value|<fid>,<IFDBSR>=Value$
 * ^1,I=[1,2,3,4,5,...]$ --> init type array
 * ^1,C=[a,2,c,4,d,...]$ --> char array
 * ^1,R=^23,C=A|24,S=dooripapa$$ --> char array
 */
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
