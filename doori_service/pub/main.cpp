#include "Pub.h"
#include "Runner.h"

int main(int argc, char** argv) {

    doori::Dictionary dic;

    doori::Getopt getopt(argc, argv);

    if (!dic.open(getopt.getOptValue("--file"))) {
        cout<< "Pub error" << endl;
        exit(-1);
    }

    dic.pprint();

    doori::Pub pub(dic);

    doori::Runner runner( std::move(pub) );

    runner.run();

    return 0;
}