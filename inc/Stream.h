// Copyright 2019 The doori-project Authors. All rights reserved.
// Use of this source code is governed by a GPL-3.0 license that can be found in the LICENSE file.
// See the AUTHORS file for names of contributors.
//
// Created by doori on 19. 7. 25.
//
#pragma once
#include "Log.h"
#include "Data.h"

class Data;

namespace doori{

enum class doori_stream_enc_type : unsigned int{
	UNKNOWN = 0,
	ASCII = 1 << 0, //"00001"
	UTF_8 = 1 << 1  //"00010"
};


class Stream
{
public:
	Stream();
	Stream(const Data& dooridata);
	Stream(const Data& dooridata, const doori_stream_enc_type& type);
	Stream(const Stream& rhs);
	Stream(Stream&& rhs);
	virtual ~Stream();
	auto operator=(const Stream& rhs) -> Stream&;
	auto operator=(Stream&& rhs) -> Stream&;
	auto toByteStream() const -> std::string; //송신규격 스트림
	auto fromByteStream(std::string byteStream) -> int; //송신규격 스트림 -> Stream 객체로
	auto setString(std::string& encordType, std::string& body) -> int;
	auto init(const Data& dooridata) -> void;
	auto getString() const-> std::string;
protected:
private:
	auto copyFrom(const Stream& rhs) -> void;
	auto setEncType(std::string encStr) -> int;
	auto getEncType(unsigned int code) -> doori_stream_enc_type;
	static const unsigned int kMaxSendBuffLen = 65536; // 64kbyte
	static const unsigned int kEncordingBuffLen = 8;
	doori_stream_enc_type mType = doori_stream_enc_type::ASCII;
	unsigned int mLen = 0;
	std::string	mDooridata ="";
};

}
