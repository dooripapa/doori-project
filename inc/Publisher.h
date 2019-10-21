// Copyright 2019 The doori-project Authors. All rights reserved.
// Use of this source code is governed by a GPL-3.0 license that can be found in the LICENSE file.
// See the AUTHORS file for names of contributors.
//
// Created by doori on 19. 7. 25.
//
#pragma	once

#include <thread>
#include "ICommunication.h"
#include "Log.h"

namespace doori{

class Publisher : public ICommunication
{
public:
    explicit Publisher(std::function<int(int, Stream &)> delegation);
    auto startPublisher(doori::Connection) noexcept -> void;
    auto publish(const Stream&) noexcept -> bool;
private:
};

}
