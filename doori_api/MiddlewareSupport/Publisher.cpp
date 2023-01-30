// Copyright 2019 The doori-project Authors. All rights reserved.
// Use of this source code is governed by a GPL-3.0 license that can be found in the LICENSE file.
// See the AUTHORS file for names of contributors.
//
// Created by doori on 19. 7. 25.
//
#include "Publisher.h"

namespace doori{

///@brief Ready단계, 복수개의 publisher로부터 데이터를 수신되었을때, 데이터 처리방식을 등록함,
auto Publisher::startPublisher(doori::Connection forMultiRequestSession) noexcept -> void {
    processingMultisessions(forMultiRequestSession);
}

///@todo sendTo 실패시 fd 삭제처리
auto Publisher::publish(const Stream &stream) noexcept -> bool{
    return sendStreamToMultisessions(stream);
}

Publisher::Publisher(std::function<int(int, Stream &)> delegation) : ICommunication(delegation) {

}

}
