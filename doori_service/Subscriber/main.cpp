#include "Sub.h"

int main(int argc, char** argv)
{
    api::Data::Dictionary dic;

    api::Process::Getopt getopt(argc, argv);

    if (!dic.load(getopt.getOptValue("--file"))) {
        cout<< "Sub error" << endl;
        exit(-1);
    }

    dic.logging("Tnsd IP", service::Subscriber::Sub::TNSD_IP);
    dic.logging("Tnsd Port", service::Subscriber::Sub::TNSD_PORT);
    dic.logging("Log Name", service::Subscriber::Sub::LOG_NAME);
    dic.logging("Log Path", service::Subscriber::Sub::LOG_PATH);
    dic.logging("Log Level", service::Subscriber::Sub::LOG_LEVEL);
    dic.logging("Binding IP", service::Subscriber::Sub::SUB_IP);
    dic.logging("Binding Port", service::Subscriber::Sub::SUB_PORT);

    service::Subscriber::Sub sub(dic);

    api::Process::Runner runner( std::move(sub) );

    runner.run();

    return 0;
}
