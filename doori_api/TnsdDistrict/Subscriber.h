// Copyright 2019 The doori-project Authors. All rights reserved.
// Use of this source code is governed by a GPL-3.0 license that can be found in the LICENSE file.
// See the AUTHORS file for names of contributors.
//
// Created by doori on 19. 7. 25.
//
#pragma once
#include <thread>
#include "ICommunication.h"
#include "Common/Log.h"

namespace doori{

    class Subscriber : public ICommunication
    {
    public:
        /**
         * subscription를 위한 처리 함수를 등록함
         * @param 데이터 수신시 처리할 delegation callback 함수등록
         */
        explicit Subscriber(std::function<int(int, Stream&)> delegation);
        /**
         * 실질적인 통신초기화, connect 요청 및 데이터 수신 이벤트 등록처리기
         * @brief 복수개의 publisher로부터 데이터를 수신되었을때, 데이터 처리방식을 등록함
         * @param forSubscriber 초기화된 doori::Connection 객체
         */
        auto startSubscriber(doori::Connection forSubscriber) noexcept -> void;
        /**
         * blocking 상태로 진입하면서 등록된 함수를 실행하게 함.
         * @todo while(1) 방식으로 blocking 처리를 더 우아한 방식으로 개선이 필요함.
         */
        auto onSubscribing() noexcept -> void;
    private:
    };

}
