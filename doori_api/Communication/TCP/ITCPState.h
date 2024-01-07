//
// Created by jslee on 24. 1. 6.
//

#ifndef DOORI_PROJECT_ITCPSTATE_H
#define DOORI_PROJECT_ITCPSTATE_H

class ITCPState {
public:
    virtual void open();
    virtual void establish();
    virtual void close();
    virtual void wait();
};doori::api::Communication::TCP

#endif //DOORI_PROJECT_ITCPSTATE_H
