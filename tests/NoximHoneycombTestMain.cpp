#include <iostream>

#include "Suite.h"

#include "NoximHexagonTest.h"
#include "NoximHMRouterTest.h"
#include "NoximHMNoCTest.h"

using namespace std;
using namespace TestSuite;

int main()
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
