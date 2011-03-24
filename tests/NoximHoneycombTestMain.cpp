#include <iostream>

#include "Suite.h"

#include "NoximHexagonTest.h"

using namespace std;
using namespace TestSuite;

int main()
{
    Suite suite("Noxim Honeycomb Test");

    suite.addTest(new NoximHexagonTest);

    suite.run();
    suite.report();
    suite.free();

    return 0;
}
