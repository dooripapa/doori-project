#include "Sub.h"
#include "Runner.h"

int main(int argc, char** argv)
{
    doori::Dictionary dic;

    doori::Getopt getopt(argc, argv);

    if (!dic.load(getopt.getOptValue("--file"))) {
        cout<< "Pub error" << endl;
        exit(-1);
    }

    dic.logging();

    doori::Sub sub(dic);

    doori::Runner runner( std::move(sub) );

    runner.run();

    return 0;
}
