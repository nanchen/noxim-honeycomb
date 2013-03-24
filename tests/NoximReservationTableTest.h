#ifndef NOXIMRESERVATIONTABLETEST_H
#define NOXIMRESERVATIONTABLETEST_H

#include "Test.h"
#include "NoximReservationTable.h"

class NoximReservationTableTest : public TestSuite::Test
{
    public:
        void run();
        NoximReservationTableTest(){};
        ~NoximReservationTableTest(){};
    private:
        void testAll();
};

#endif
