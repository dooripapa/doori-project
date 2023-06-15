// Copyright 2019 The doori-project Authors. All rights reserved.
// Use of this source code is governed by a GPL-3.0 license that can be found in the LICENSE file.
// See the AUTHORS file for names of contributors.
//
// Created by doori on 19. 7. 25.
//
#include "FidSegment.h"
#include "Fid.h"

using namespace std;

namespace doori::api::Data{
/****************************
  @comment  : 생성자, 복제 생성자
  @return   : 
  @argument :
  ****************************/ 
FidSegment::FidSegment()
{
	mFid			= numeric_limits<decltype(mFid	  )>::quiet_NaN();    
	mType			= TYPE::N;
	mInt			= numeric_limits<decltype(mInt   )>::quiet_NaN();       
	mLong			= numeric_limits<decltype(mLong  )>::quiet_NaN();      
	mFloat			= numeric_limits<decltype(mFloat )>::quiet_NaN();     
	mDouble			= numeric_limits<decltype(mDouble)>::quiet_NaN();    
	mChar			= numeric_limits<decltype(mChar  )>::quiet_NaN();    
	mBool			= numeric_limits<decltype(mBool  )>::quiet_NaN();    
	mDooridata = nullptr;
}

FidSegment::FidSegment(const FidSegment& rhs)
{
	copyFrom( rhs );
}

FidSegment::FidSegment(FidSegment&& rhs)
{
	copyFrom( move(rhs) );

	if( rhs.mType == TYPE::R )
		rhs.mType = TYPE::N;
}

FidSegment::~FidSegment()
{
	if( mType == TYPE::R )
		delete mDooridata;
}

FidSegment::FidSegment(int fid, int value)
{
	mFid = fid;
	mType = TYPE::I;
	mInt = value;
}
FidSegment::FidSegment(int fid, long value)
{
	mFid = fid;
	mType = TYPE::L;
	mLong = value;
}
FidSegment::FidSegment(int fid, float value)
{
	mFid = fid;
	mType = TYPE::F;
	mFloat = value;
}
FidSegment::FidSegment(int fid, double value)
{
	mFid = fid;
	mType = TYPE::D;
	mDouble = value;
}
FidSegment::FidSegment(int fid, char value)
{
	mFid = fid;
	mType = TYPE::C;
	mChar = value;
}
FidSegment::FidSegment(int fid, bool value)
{
	mFid = fid;
	mType = TYPE::B;
	mBool = value;
}
FidSegment::FidSegment(int fid, string& value)
{
	mFid = fid;
	mType = TYPE::S;
	if ( value.empty() )
		mStr = string("NaN");
	else
		mStr = value;
}
FidSegment::FidSegment(int fid, const string& value)
{
	mFid = fid;
	mType = TYPE::S;
	mStr = value;
}
FidSegment::FidSegment(int fid, Fid& dooridata)
{
	mFid = fid;
	mType = TYPE::R;
	mDooridata = new Fid(dooridata );
}

auto FidSegment::deleteDooridata() -> void
{
	if( mType == TYPE::R )
	{
		mType = TYPE::N;
		delete mDooridata;
		mDooridata = nullptr;
	}
	return;
}

auto FidSegment::set(int fid, int value) -> void
{
	deleteDooridata();
	mFid = fid;
	mType = TYPE::I;
	mInt = value;
}
auto FidSegment::set(int fid, long value) -> void
{
	deleteDooridata();
	mFid = fid;
	mType = TYPE::L;
	mLong = value;
}
auto FidSegment::set(int fid, float value) -> void
{
	deleteDooridata();
	mFid = fid;
	mType = TYPE::F;
	mFloat = value;
}
auto FidSegment::set(int fid, double value) -> void
{
	deleteDooridata();
	mFid = fid;
	mType = TYPE::D;
	mDouble = value;
}
auto FidSegment::set(int fid, char value) -> void
{
	deleteDooridata();
	mFid = fid;
	mType = TYPE::C;
	mChar = value;
}
auto FidSegment::set(int fid, bool value) -> void
{
	deleteDooridata();
	mFid = fid;
	mType = TYPE::B;
	mBool = value;
}
auto FidSegment::set(int fid, string& value) -> void
{
	deleteDooridata();
	mFid = fid;
	mType = TYPE::S;
	if ( value.empty() )
		mStr = string("NaN");
	else
		mStr = value;
}
auto FidSegment::set(int fid, const string& value) -> void
{
	deleteDooridata();
	mFid = fid;
	mType = TYPE::S;
	mStr = value;
}
auto FidSegment::set(int fid, Fid& dooridata) -> void
{
	deleteDooridata();
	mFid = fid;
	mType = TYPE::R;
	mDooridata = new Fid(dooridata );
}



auto FidSegment::copyFrom(const FidSegment& rhs) -> void
{
	mFid 		= rhs.mFid		;
	mType 		= rhs.mType		;
	mInt		= rhs.mInt		;
	mLong		= rhs.mLong		;
	mFloat		= rhs.mFloat	;
	mDouble		= rhs.mDouble	;
	mChar		= rhs.mChar		;
	mBool		= rhs.mBool		;
	mStr		= rhs.mStr		;

	if( mType == TYPE::R )
		mDooridata	= new Fid(*rhs.mDooridata );
	return;
}
auto FidSegment::copyFrom(FidSegment&& rhs) -> void
{
	mFid 		= rhs.mFid		;
	mType 		= rhs.mType		;
	mInt		= rhs.mInt		;
	mLong		= rhs.mLong		;
	mFloat		= rhs.mFloat	;
	mDouble		= rhs.mDouble	;
	mChar		= rhs.mChar		;
	mBool		= rhs.mBool		;
	mStr		= rhs.mStr		;

	if( mType == TYPE::R )
		mDooridata	= rhs.mDooridata;

	return;
}


/****************************
@comment  : 이동대입연산자, 대입대상의 메모리 정리후, 복제생성자(용) 호출
@return   : 
@argument :
****************************/ 
auto FidSegment::operator=(FidSegment&& rhs) -> FidSegment&
{
	if( this ==&rhs  )
		return *this;

	if( mType == TYPE::R )
		delete mDooridata;

	copyFrom(move(rhs));

	// rhs 삭제처리
	//-------------------------------------------------- 
	if( rhs.mType == TYPE::R )
	{
		rhs.mType = TYPE::N;
		rhs.mDooridata = nullptr;
	}

	return *this;
}

/****************************
@comment  : 대입연산자, 대입대상의 메모리 정리후, 복제생성자(용) 호출
@return   : 
@argument :
****************************/ 
auto FidSegment::operator=(const FidSegment& rhs) -> FidSegment&
{
	if( this ==&rhs  )
		return *this;

	if( mType == TYPE::R )
	{
		delete mDooridata;
		mDooridata = nullptr;
	}

	copyFrom(rhs);

	return *this;
}

// doori_data_segments의 sort를 할때, 이 함수를 호출이 된다.
// operator< 함수는 'const'라고 명명선언되어 있어야 한다.
auto FidSegment::operator<(const FidSegment& rhs) const -> bool
{
	return (mFid < rhs.mFid);
}
// "1,S=doori" or "10,R=$| ... |$" Fid, stream
auto FidSegment::fromString(string& str) -> int
{
	//*this delete
	deleteDooridata();

	string value;
	string fid;
	string type;
	unsigned int commaPos=0;
	unsigned int equalPos=0;
	bool isValueStart=false;
	for(unsigned int i=0; i<str.length();++i)
	{
		if (!isValueStart)
		{
			if (str[i]==',')
				commaPos = i;
			if (str[i]=='=')
				equalPos = i;
			if (commaPos && equalPos)
				isValueStart = true;
		}
	}
	fid  = str.substr(0,commaPos++);
	type = str.substr(commaPos, sizeof(TYPE));
	equalPos++; //move it to next pos, just start pos for value
	value= str.substr(equalPos, str.length()-equalPos);

//	LOG(DEBUG, "fid : [", fid, "]");
	mFid = stoi(fid);
//	LOG(DEBUG, "type: [", type, "]");
	mType = static_cast<TYPE>(type[0]);
//	LOG(DEBUG, "value: [", value, "]");

	Fid data;
	switch (mType)
	{
		case TYPE::I:
			mInt = stoi(value);
			break;
		case TYPE::L:
			mLong = stol(value);
			break;
		case TYPE::F:
			mFloat = stof(value);
			break;
		case TYPE::D:
			mDouble = stod(value);
			break;
		case TYPE::C:
			mChar = value[0];
			break;
		case TYPE::B:
			if (value == "true")
				mBool = true;
			else
				mBool = false;
			break;
		case TYPE::S:
			mStr = value;
			break;
		case TYPE::R:
			data.fromString(value);
			mDooridata = new Fid(data);
			break;
		default:
			LOG(ERROR, "unknown type");
			break;
	}
	return 0;
}

auto FidSegment::getFid() const -> int
{
	return mFid;
}
auto FidSegment::getType() const -> char
{
	return (char)mType;
}

auto FidSegment::getValueToString() const -> string
{
	switch( mType )
	{
		case TYPE::I:
			return to_string(mInt);
		case TYPE::L:
			return to_string(mLong);
		case TYPE::F:
			return to_string(mFloat);
		case TYPE::D:
			return to_string(mDouble);
		case TYPE::C:
		{
			string s(1, mChar); //to_string에 to_string(char type) 없음
			return s;
		}
		case TYPE::B:
		{
			if (true == mBool)
				return "true";
			else
				return "false";
		}
		case TYPE::S:
			return mStr;
		case TYPE::R:
			return mDooridata->toString();
		case TYPE::N:
		default:
			break;
	}
	return "not implement";
}
auto FidSegment::sortDooridata() -> void
{
	mDooridata->sort();
	return;
}



}//namespace doori
