// Copyright 2019 The doori-project Authors. All rights reserved.
// Use of this source code is governed by a GPL-3.0 license that can be found in the LICENSE file.
// See the AUTHORS file for names of contributors.
//
// Created by doori on 19. 7. 25.
//
#include "Subscriber.h"

namespace doori{

///@brief 복수개의 publisher로부터 데이터를 수신되었을때, 데이터 처리방식을 등록함
auto Subscriber::startSubscriber(doori::Connection forSubscriber) noexcept -> void {
    processingMultisessions(forSubscriber);
}


///@brief blocking 상태로 진입하면서 등록된 함수를 실행하게 함.
///@todo while(1) 방식으로 blocking 처리를 더 우아한 방식으로 개선이 필요함.
auto Subscriber::onSubscribing() noexcept -> void{
    while(true)
        std::this_thread::sleep_for(std::chrono::seconds(5));
}

Subscriber::Subscriber(std::function<int(int, Stream &)> delegation) : ICommunication(delegation) {

}


}
