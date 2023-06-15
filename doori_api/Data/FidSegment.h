// Copyright 2019 The doori-project Authors. All rights reserved.
// Use of this source code is governed by a GPL-3.0 license that can be found in the LICENSE file.
// See the AUTHORS file for names of contributors.
//
// Created by doori on 19. 7. 25.
//
#pragma once
#include "Common/Log.h"

namespace doori::api::Data{

//forward declaration, only when using pointer, ref defined
class Fid;

class FidSegment
{
public:
    enum class TYPE:char{ I='I', L='L', F='F', D='D', C='C', B='B', S='S', R='R', N='N' };
	FidSegment();
	virtual ~FidSegment();
	FidSegment(const FidSegment& rhs);
	FidSegment(FidSegment&& rhs);
	FidSegment(int fid, int value);
	FidSegment(int fid, long value);
	FidSegment(int fid, float value);
	FidSegment(int fid, double value);
	FidSegment(int fid, char value);
	FidSegment(int fid, bool value);
	FidSegment(int fid, std::string& value);
	FidSegment(int fid, const std::string& value);
	template<int N>
	FidSegment(int fid, char const(&value)[N]);
	FidSegment(int fid, Fid& dooridata);

	auto set(int fid, int value) -> void;
	auto set(int fid, long value) -> void;
	auto set(int fid, float value) -> void;
	auto set(int fid, double value) -> void;
	auto set(int fid, char value) -> void;
	auto set(int fid, bool value) -> void;
	auto set(int fid, std::string& value) -> void;
	auto set(int fid, const std::string& value) -> void;
	template<int N>
	auto set(int fid, char const(&value)[N]) -> void;
	auto set(int fid, Fid& dooridata) -> void;

	auto operator=(const FidSegment& rhs) -> FidSegment&;
	auto operator=(FidSegment&& rhs) -> FidSegment&;
	auto operator<(const FidSegment& rhs) const -> bool;

	auto fromString(std::string& formattedStr) -> int;
	auto getType() const -> char;
	auto getFid() const -> int;
	auto getValueToString() const -> std::string;
	auto sortDooridata() -> void;
protected:
private:
	auto deleteDooridata() -> void;
	auto copyFrom(const FidSegment& rhs) -> void;
	auto copyFrom(FidSegment&& rhs) -> void;
	unsigned int				mFid;
	TYPE					    mType;
	int							mInt;
	long						mLong;
	float						mFloat;
	double						mDouble;
	char						mChar;
	bool						mBool;
	std::string					mStr;
	Fid*					mDooridata; //doori_data는 FidSegment 합
};

}

#include "FidSegment.hpp"
