#ifndef NOXIMHMROUTERTEST_H
#define NOXIMHMROUTERTEST_H

#include "Test.h"

#include "NoximHMRouter.h"


class NoximHMRouterTest : public TestSuite::Test
{
    public:
        void run();
        NoximHMRouterTest(){};
        ~NoximHMRouterTest(){};

    private:
        void testRoutingMinusXPlusZFirst();
        void printVector(const vector<int>& v);
};

#endif // NOXIMHMROUTERTEST_H
