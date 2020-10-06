// Copyright 2019 The doori-project Authors. All rights reserved.
// Use of this source code is governed by a GPL-3.0 license that can be found in the LICENSE file.
// See the AUTHORS file for names of contributors.
//
// Created by doori on 19. 7. 25.
//
#include "Subscriber.h"

namespace doori{

auto Subscriber::startSubscriber(doori::Connection forSubscriber) noexcept -> void {
    processingMultisessions(forSubscriber);
}

auto Subscriber::onSubscribing() noexcept -> void{
    while(true)
        std::this_thread::sleep_for(std::chrono::seconds(5));
}

Subscriber::Subscriber(std::function<int(int, Stream &)> delegation) : ICommunication(delegation) {

}


}
