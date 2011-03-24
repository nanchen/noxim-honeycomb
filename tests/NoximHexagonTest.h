#ifndef NOXIMHEXAGONTEST_H
#define	NOXIMHEXAGONTEST_H

#include "Test.h"

class NoximHexagonTest: public TestSuite::Test {

public:
    void run();
    NoximHexagonTest(){};
    ~NoximHexagonTest(){};

private:
    void testConstructor();
    void testCreateNeighbor();
    void testBuildHexagonTree();

};

#endif	/* NOXIMHEXAGONTEST_H */

