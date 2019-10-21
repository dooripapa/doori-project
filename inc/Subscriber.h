// Copyright 2019 The doori-project Authors. All rights reserved.
// Use of this source code is governed by a GPL-3.0 license that can be found in the LICENSE file.
// See the AUTHORS file for names of contributors.
//
// Created by doori on 19. 7. 25.
//
#pragma once
#include <thread>
#include "ICommunication.h"
#include "Log.h"

namespace doori{

class Subscriber : public ICommunication
{
public:
    explicit Subscriber(std::function<int(int, Stream&)> delegation);
    auto startSubscriber(doori::Connection forSubscriber) noexcept -> void;
    auto onSubscribing() noexcept -> void;
private:
};

}
