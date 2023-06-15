// Copyright 2019 The doori-project Authors. All rights reserved.
// Use of this source code is governed by a GPL-3.0 license that can be found in the LICENSE file.
// See the AUTHORS file for names of contributors.
//
// Created by doori on 19. 7. 25.
//
#include "Fid.h"

using namespace std;

namespace doori::api::Data{

/****************************
@comment  : 생성자, 복제생성자, 소멸자
@return   : 
@argument :
****************************/ 
Fid::Fid()
{
}

Fid::Fid(const Fid& rhs)
{
    copyFrom( rhs );
}

Fid::Fid(Fid&& rhs)
{
    copyFrom( rhs );
}

Fid::Fid(const FidSegment& dataSegment)
{
	segments.push_back( dataSegment );
}
Fid::Fid(FidSegment&& dataSegment)
{
	segments.push_back( dataSegment );
}


Fid::~Fid()
{
}

/****************************
@comment  : 
@return   : 
@argument :
****************************/ 
auto Fid::append(const FidSegment& dataSegment) -> void
{
	segments.push_back( dataSegment );
	return;
}
/****************************
@comment  : 
@return   : 
@argument :
****************************/ 
auto Fid::append(FidSegment&& dataSegment) -> void
{
	segments.push_back( dataSegment );
	return;
}

/****************************
@comment  : 
@return   : 
@argument :
****************************/ 
auto Fid::sort() -> void
{
	vector<FidSegment>::iterator iter;
	for( iter=segments.begin(); iter<segments.end(); iter++ )
	{
		if( iter->getType() == static_cast<char>(FidSegment::TYPE::R) )
			iter->sortDooridata();
	}
	std::sort( segments.begin(), segments.end() );
	return;
}

/****************************
@comment  : 
@return   : 
@argument :
****************************/ 
auto Fid::walk(ostream& outPut ) -> void
{
	vector<FidSegment>::iterator iter;
	for( iter=segments.begin(); iter<segments.end(); iter++ )
	{
		outPut << (*iter).getFid() << "," << (*iter).getType() << "=" << (*iter).getValueToString() << endl;
	}
	return;
}

//auto Fid::walk( ) const -> void
// const로 선언되어 있다면, const_iterator으로 변경해야 함
//-------------------------------------------------- 
auto Fid::walk( ) const -> void
{
	vector<FidSegment>::const_iterator iter;
	cout << "$|";
	for( iter=segments.begin(); iter<segments.end(); iter++ )
	{
		cout << (*iter).getFid() << "," << (*iter).getType() << "=" << (*iter).getValueToString() << "|";
	}
	cout << "$";
	cout << endl;
	return;
}

auto Fid::begin() noexcept -> vector<FidSegment>::iterator
{
	return segments.begin();
}

auto Fid::end() noexcept -> vector<FidSegment>::iterator
{
	return segments.end();
}

auto Fid::cbegin() noexcept -> const vector<FidSegment>::iterator
{
	return segments.begin();
}

auto Fid::cend() noexcept -> const vector<FidSegment>::iterator
{
	return segments.end();
}


auto Fid::find_if_Fid(int fid) -> vector<FidSegment>::iterator
{
	return 	find_if(
				segments.begin(), 
				segments.end(), 
				[=](FidSegment segment)
				{
					return (segment.getFid()==fid);
				}
			);
}
auto Fid::find_if_Fid(vector<FidSegment>::iterator startIterator, int fid) -> vector<FidSegment>::iterator
{
	return 	find_if(
				startIterator, 
				segments.end(), 
				[=](FidSegment segment)
				{
					return (segment.getFid()==fid);
				}
			);
}

///@brief doori_data -> $| FID,mStreamProtocol=VALUE |$  형식의 문자열로 변환
auto Fid::toString() const -> string
{
	string sTemp;
	vector<FidSegment>::const_iterator iter;
	sTemp += "$|";
	if (segments.size()==0)
	{
		sTemp+="|$";
		return sTemp;
	}
	for( iter=segments.cbegin(); iter!=segments.cend(); iter++ )
	{
		sTemp += to_string( (*iter).getFid() );
		sTemp += ",";
	    sTemp += (*iter).getType();
	    sTemp += "=";
		sTemp += (*iter).getValueToString();
		sTemp += "|";
	}
	sTemp += "$";
	return sTemp;
}

///@brief doori_data_string -> doori_data 객체로 변경
auto Fid::fromString(string formattedStr) -> int
{
	string doori_data_string;
	string doori_data_segment_string;
	FidSegment part;
	auto sDataPos = 0;
	auto eDataPos = formattedStr.size();
	auto lenFromTo = [](decltype(sDataPos) sPos, decltype(eDataPos) ePos)
					 {
						 return(ePos - sPos);
					 };

	auto dooridataDepth = 0;

	//Fid string format check : $| ... |$
	if (formattedStr[0]=='$' 
	&&  formattedStr[1]=='|' 
	&&  formattedStr[formattedStr.size()-1] == '$'
	&&  formattedStr[formattedStr.size()-2] == '|')
	{
		// $| , |$, total size 4
		sDataPos += 2;
		eDataPos += -2;
		doori_data_string = formattedStr.substr(sDataPos, lenFromTo(sDataPos,eDataPos));
//		LOG(DEBUG, "##################################");
//		LOG(DEBUG, "### [", doori_data_string, "] ####");
	}
	else
	{
		LOG(ERROR, "Format error", "[", formattedStr,"]");
		return -1;
	}

	auto DataTotalLen = doori_data_string.size();
	// separate FidSegment from Fid
	// FidSegment 의 변수는, for문에서 반복적으로 값이 할당됨
	// unserialize() 함수 호출시, 기존에 할당된 개체를 해제되어야 한다.
	// 그부분을 unserialize() 내부 함수 안에서 해체를 함

	sDataPos=eDataPos=0;
	for(unsigned int i = 0; i<DataTotalLen; ++i)
	{
		eDataPos = i;
		if (doori_data_string[i]=='\\') 
		{
			++i; //next character ignore!
			continue;
		}
		///////////////// doori_data 시작여부  "$| ... |$"
		if (doori_data_string[i++]=='R')
		{
			if (doori_data_string[i++]=='=')
			{
				if (doori_data_string[i++]=='$')
				{
					if (doori_data_string[i]=='|')
					{
						++dooridataDepth;
//						LOG(DEBUG, "doori data depth increment:", dooridataDepth);
						continue;
					}
				}
				--i;
			}
			--i;
		}
		--i; //위치복구
		
		if (dooridataDepth>0)
		{
			if (doori_data_string[i++]=='|')
			{
				if (i<DataTotalLen) //문자열 끝 여부
				{
					if (doori_data_string[i]=='$')
					{
						if ((i+1)==DataTotalLen) ////// doori_data 마무리.
						{
							doori_data_segment_string = doori_data_string.substr(sDataPos, lenFromTo(sDataPos, DataTotalLen));
//							LOG(DEBUG, "Datat segment[", doori_data_segment_string, "]");
							part.fromString(doori_data_segment_string);
							segments.push_back(part);
							return 0;
						}
						else
						{
							--dooridataDepth;
//							LOG(DEBUG, "doori data depth decrease:", dooridataDepth);
							continue;
						}
					}
				}
				else
				{
					LOG(DEBUG, "Fid string is wrong!");
					return -1;
				}
			}
			--i; //위치복구
		}

		///////////////// doori_data 끝이어야 '|' 구분자가 의미 있음
		if (dooridataDepth==0)  
		{
			if (doori_data_string[i]=='|' )
			{
				doori_data_segment_string = doori_data_string.substr(sDataPos, lenFromTo(sDataPos, eDataPos));
//				LOG(DEBUG, "Datat segment[", doori_data_segment_string, "]");
				part.fromString(doori_data_segment_string);
				segments.push_back(part);
				sDataPos = eDataPos+1; // '|' 's len +1
			}
			if ((i+1) == DataTotalLen ) ////// doori_data 마무리
			{
				doori_data_segment_string = doori_data_string.substr(sDataPos, lenFromTo(sDataPos, DataTotalLen));
//				LOG(DEBUG, "Datat segment[", doori_data_segment_string, "]");
				part.fromString(doori_data_segment_string);
				segments.push_back(part);
				return 0;
			}
		}
	}
	return 0;
}

auto Fid::operator=(const Fid& rhs)-> Fid&
{
	if( this ==&rhs  ) 
		return *this;  

	copyFrom( rhs );
	return *this;
}

auto Fid::clear() noexcept -> void
{
	segments.clear();
}

///@note 이동연산 의미가 없으나, 향후 대비차원으로 구현부 추가됨
auto Fid::operator=(Fid&& rhs)-> Fid&
{
	if( this ==&rhs  ) 
		return *this;  

	copyFrom( rhs );
	return *this;
}

auto Fid::copyFrom(const Fid& rhs) noexcept ->void
{
	segments = rhs.segments;
	return;
}

///@note 이동연산 의미가 없으나, 향후 대비차원으로 구현부 추가됨
auto Fid::copyFrom(Fid&& rhs) noexcept ->void
{
	segments = rhs.segments;
	return;
}

}//namespace doori
