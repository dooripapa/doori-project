#include <memory>
#include "tnsd.h"
#include "ProcessSupport/Runner.h"

using namespace std;

int main(int argc, char **argv)
{
    doori::Dictionary    dictionary;

    doori::Getopt        getopt(argc, argv);

    if(!dictionary.load(getopt.getOptValue("--file")))
    {
        cout<< "MiddleSide error" << endl;
        exit(-1);
    }

    doori::Tnsd tnsd(dictionary);

    doori::Runner runner( std::move(tnsd) );

    runner.run();

    return 0;
}

