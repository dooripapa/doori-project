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

    service::Tnsd::Tnsd tnsd(dictionary);

    Process::Runner runner( std::move(tnsd) );

    runner.run();

    return 0;
}

