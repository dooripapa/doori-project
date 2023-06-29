#include <memory>
#include "tnsd.h"
#include "Process/Runner.h"

using namespace std;
using namespace doori;

int main(int argc, char **argv)
{
    Data::Dictionary    dictionary;

    Process::Getopt        getopt(argc, argv);

    if(!dictionary.load(getopt.getOptValue("--file")))
    {
        cout<< "MiddleSide error" << endl;
        exit(-1);
    }

    dictionary.logging("Tnsd IP", service::TnsDaemon::Tnsd<Communication::Socket>::TNSD_IP);
    dictionary.logging("Tnsd Port", service::TnsDaemon::Tnsd<Communication::Socket>::TNSD_PORT);
    dictionary.logging("Log Name", service::TnsDaemon::Tnsd<Communication::Socket>::LOG_NAME);
    dictionary.logging("Log Path", service::TnsDaemon::Tnsd<Communication::Socket>::LOG_PATH);
    dictionary.logging("Log Level", service::TnsDaemon::Tnsd<Communication::Socket>::LOG_LEVEL);

    service::TnsDaemon::Tnsd< Communication::Socket > tnsd(dictionary);

    Process::Runner runner( std::move(tnsd) );

    runner.run();

    return 0;
}

