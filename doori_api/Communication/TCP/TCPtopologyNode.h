//
// Created by jslee on 24. 1. 6.
//

#ifndef DOORI_PROJECT_TCPTOPOLOGYNODE_H
#define DOORI_PROJECT_TCPTOPOLOGYNODE_H

namespace doori::api::Communication;

namespace doori::api::Communication::TCP {
    class TCPtopologyNode : public IIPCTopologyNode {

    public:
        long Send(const string &data) const override;

        long Recv(string &data, std::uint32_t utilSize) const override;

        int Close() override;


    };
} // doori

#endif //DOORI_PROJECT_TCPTOPOLOGYNODE_H
