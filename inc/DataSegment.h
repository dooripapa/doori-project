// Copyright 2019 The doori-project Authors. All rights reserved.
// Use of this source code is governed by a GPL-3.0 license that can be found in the LICENSE file.
// See the AUTHORS file for names of contributors.
//
// Created by doori on 19. 7. 25.
//
#pragma once
#include "Log.h"

namespace doori{

//forward declaration, only when using pointer, ref defined
class Data;

class DataSegment
{
public:
    enum class TYPE:char{ I='I', L='L', F='F', D='D', C='C', B='B', S='S', R='R', N='N' };
	DataSegment();
	virtual ~DataSegment();
	DataSegment(const DataSegment& rhs);
	DataSegment(DataSegment&& rhs);
	DataSegment(int fid, int value);
	DataSegment(int fid, long value);
	DataSegment(int fid, float value);
	DataSegment(int fid, double value);
	DataSegment(int fid, char value);
	DataSegment(int fid, bool value);
	DataSegment(int fid, std::string& value);
	DataSegment(int fid, const std::string& value);
	template<int N>
	DataSegment(int fid, char const(&value)[N]);
	DataSegment(int fid, Data& dooridata);

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
	auto set(int fid, Data& dooridata) -> void;

	auto operator=(const DataSegment& rhs) -> DataSegment&;
	auto operator=(DataSegment&& rhs) -> DataSegment&;
	auto operator<(const DataSegment& rhs) const -> bool;

	auto fromString(std::string& formattedStr) -> int;
	auto getType() const -> char;
	auto getFid() const -> int;
	auto getValueToString() const -> std::string;
	auto sortDooridata() -> void;
protected:
private:
	auto deleteDooridata() -> void;
	auto copyFrom(const DataSegment& rhs) -> void;
	auto copyFrom(DataSegment&& rhs) -> void;
	unsigned int				mFid;
	TYPE					    mType;
	int							mInt;
	long						mLong;
	float						mFloat;
	double						mDouble;
	char						mChar;
	bool						mBool;
	std::string					mStr;
	Data*					mDooridata; //doori_data는 DataSegment 합
};

}

#include "DataSegment.hpp"
