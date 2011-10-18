#include "NoximReservationTableTest.h"

void NoximReservationTableTest::run() {
	testAll();
}

void NoximReservationTableTest::testAll() {
	// every OUT port should be available at first
	NoximReservationTable rt;
	int i;
	for (i = 0; i < DIRS + 1; i++)
		test_(rt.isAvailable(i));
	// OUT port of every IN port should be NOT_RESERVED
	for (i = 0; i < DIRS + 1; i++)
		test_(rt.getOutputPort(i)==NOT_RESERVED);
	// reserve 1 port
	rt.reserve(DIR_X, DIR_Y);
	test_(rt.getOutputPort(DIR_X)==DIR_Y);
	test_(!rt.isAvailable(DIR_Y));
	// release it
	rt.release(DIR_Y);
	for (i = 0; i < DIRS + 1; i++)
		test_(rt.isAvailable(i));
	// invalidate z
	rt.invalidate(DIR_Z);
	test_(!rt.isAvailable(DIR_Z));
	// try to reserve invalid port, assertion failed
//	rt.reserve(DIR_Y, DIR_Z);
}
