#include <iostream>
#include <ostream>
#include <string>
#include "test.hpp"

#define DEBUG logger[mylog::LEVEL::D]
#define INFO logger[mylog::LEVEL::I]

using namespace std;

int main() {
		mylog logger;

		logger.setLevel(mylog::LEVEL::I);

		DEBUG<<"The is leejaeseong"<<100.0f << 29238455L << '\n';
		DEBUG<<"The is leejaeseong"<<100.0f << 29238455L << std::endl;
		DEBUG<<"debug"<<std::endl;
		INFO<<"info"<<std::endl;

    return 0;
}
