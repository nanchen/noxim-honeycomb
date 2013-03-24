#ifndef NOXIMHMNOCTEST_H
#define NOXIMHMNOCTEST_H

#include "Test.h"
#include "NoximHMNoC.h"

class NoximHMNoCTest : public TestSuite::Test
{
    public:
        void run();
        NoximHMNoCTest(){};
        ~NoximHMNoCTest(){};
    private:
        void testBuildHoneycombMesh();
};

#endif // NOXIMHMNOCTEST_H
