#include "Pub.h"

using namespace std;
using namespace doori;

int main(int argc, char** argv) {

    api::Data::Dictionary dic;

    api::Process::Getopt getopt(argc, argv);

    if (!dic.load(getopt.getOptValue("--file"))) {
        cout<< "Pub error" << endl;
        exit(-1);
    }

    dic.logging("Tnsd IP", service::Publisher::Pub::TNSD_IP);
    dic.logging("Tnsd Port", service::Publisher::Pub::TNSD_PORT);
    dic.logging("Log Name", service::Publisher::Pub::LOG_NAME);
    dic.logging("Log Path", service::Publisher::Pub::LOG_PATH);
    dic.logging("Log Level", service::Publisher::Pub::LOG_LEVEL);
    dic.logging("Binding IP", service::Publisher::Pub::PUB_IP);
    dic.logging("Binding Port", service::Publisher::Pub::PUB_PORT);

    service::Publisher::Pub pub(dic);

    api::Process::Runner runner( std::move(pub) );

    runner.run();

    return 0;
}
