// Copyright 2019 The doori-project Authors. All rights reserved.
// Use of this source code is governed by a GPL-3.0 license that can be found in the LICENSE file.
// See the AUTHORS file for names of contributors.
//
// Created by doori on 19. 7. 25.
//
#pragma    once

#include "Addr.h"
#include "DataStream/Json.h"

namespace doori::CommunicationMember{

    class Endpoint {
    public:
        enum class TYPE : unsigned int {
            TCP, UDP, SHM, MSQ, NON
        };
        enum READ_STATUS : bool {
            TRUE = true, FALSE = false
        };

        /**
         * 기본생성자
         */
        Endpoint();

        /**
         * doori::DataStream::Json 입력받아 Endpoint 초기화를 한다.
         * @param json
         */
        explicit Endpoint(TYPE transportType, DataStream::Json json);

        /**
         * Endpoint객체의 통신유형.(IPC type)
         * @return TYPE : Endpoint::TYPE
         */
        auto Type() noexcept -> TYPE;

        /**
         * Endpoint 초기화되었는지 읽을 수 있는 상태 여부
         * @return Endpoint::READ_STATUS
         */
        auto CanRead() noexcept -> READ_STATUS;

        /**
         * 토폴리지 정보를 json 형식으로 리턴
         * @return doori::json
         */
        [[nodiscard]] const DataStream::Json& getTopoloiesInfo() const;

    private:
        READ_STATUS mSet;
        TYPE mType;
        DataStream::Json mTopoloiesInfo;
    };

}//namespace doori
