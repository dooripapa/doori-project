// Copyright 2019 The doori-project Authors. All rights reserved.
// Use of this source code is governed by a GPL-3.0 license that can be found in the LICENSE file.
// See the AUTHORS file for names of contributors.
//
// Created by doori on 19. 7. 25.
//

#pragma once

#include <memory>
#include <string>
#include "Getopt.h"
#include "DataStream/Dictionary.h"

namespace doori::Process{

/**
 * doori api를 통해서, 어플리케이션을 만들때는 이 클래스를 상속받아서 엄격하게 인터페이스를 구현해야 한다.
 * 이 객체 함수를 상속받아 인터페이스를 구현되어져야 하며,
 * Runner 클래스가 인터페이스를 호출한다.
 */
class Application {
public:
    Application();
    virtual ~Application();
    /**
     * 상속받은 객체는 operator() 연산자를 재정의 해야 한다.
     * 상속받은 객체가 하고자하는 함수 routine 정의
     * @return
     */
    virtual auto operator()() noexcept -> int = 0;
    /**
     * 상속받은 객체가 이동시, 이 함수가 호출된다.
     * 상속받은 객체는 복사할 수 없도록한다.
     * @return std::unique_ptr<Application>
     */
    virtual auto clone() const noexcept -> std::unique_ptr<Application> = 0;
    /**
     * 상속받은 객체가 이 함수를 재정의 하지 않으면,
     * 이미 정의된 a.out 이름으로 리턴됨
     * @return std::string
     */
    virtual auto ProcessName() noexcept -> std::string = 0;
    /**
     * 상속받은 객체가 이 함수를 재정의 하지 않으면,
     * 이미 정의된 false가 리턴함. false : Daemon 프로세스가 아닌 형태로 기동된다는 뜻
     * @return bool
     */
    virtual auto Daemonize() noexcept -> bool = 0;
    /**
     * 상속받은 객체는 자신의 로그명(full path) 재정의 해야 한다.
     * 정의하지 않으면, /tmp/a.out.log 리턴함
     * @return std::string
     */
    virtual auto LogFile() noexcept -> std::string = 0;
    /**
     * 상속받은 객체는 자신의 로그레벨을 재정의 해야 한다.
     * 정의하지 않으면, LOG::D(ebug) 리턴함
     * @return LOG::LEVEL
     */
    virtual auto LogLevel() noexcept -> Common::Log::LEVEL = 0;
    /**
     * 상속받은 객체는 자신이 종료할때, 해당 행위를 재정의 해야 한다.
     * ex) 시스템 resource 해제 등.
     * 정의하지 않는 다면, 단순히 0을 리턴함
     * @return int
     */
    virtual auto Terminate() noexcept -> int = 0;
private:
    std::string mDefaultLogPath{"/tmp/"};
    std::string mDefaultLogName{"a.out.log"};
};

}//namespace doori
