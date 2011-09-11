#include <iostream>

#include "systemc.h"

#include "Suite.h"

#include "NoximHexagonTest.h"
#include "NoximHMRouterTest.h"
#include "NoximHMNoCTest.h"

using namespace std;
using namespace TestSuite;

 #ifndef MAIN
 #define MAIN

//int main()
int sc_main(int argc, char* argv[])
{
    Suite suite("Noxim Honeycomb Test");

    suite.addTest(new NoximHexagonTest);
    suite.addTest(new NoximHMNoCTest);
    suite.addTest(new NoximHMRouterTest);

    suite.run();
    suite.report();
    suite.free();

    return 0;
}
#endif
