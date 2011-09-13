#include "NoximHMRouterTest.h"
#include "NoximMain.h"
#include <systemc.h>
#include <vector>

void NoximHMRouterTest::run() {
	//testTxProcess();
	testRoutingMinusXPlusZFirst();
}

void NoximHMRouterTest::testTxProcess() {
	std::cout << "NoximHMRouterTest::testTxProcess()" << std::endl;

	sc_clock clock;
	sc_signal<bool> reset;

	sc_signal<NoximFlit> flit_rx[DIRECTIONS_HM]; // The input channels 2-7
	sc_signal<bool> req_rx[DIRECTIONS_HM]; // The requests associated with the input channels 3-13
	sc_signal<bool> ack_rx[DIRECTIONS_HM]; // The outgoing ack signals associated with the input channels 14-19

	sc_signal<NoximFlit> flit_tx[DIRECTIONS_HM]; // The output channels 20-25
	sc_signal<bool> req_tx[DIRECTIONS_HM]; // The requests associated with the output channels 26-31
	sc_signal<bool> ack_tx[DIRECTIONS_HM]; // The outgoing ack signals associated with the output channels 32-37

	sc_signal<int> free_slots[DIRECTIONS_HM]; //38-43
	sc_signal<int> free_slots_neighbor[DIRECTIONS_HM]; //44-49

	// Signals
	sc_signal<NoximFlit> flit_rx_local; // The input channels
	sc_signal<bool> req_rx_local; // The requests associated with the input channels
	sc_signal<bool> ack_rx_local; // The outgoing ack signals associated with the input channels

	sc_signal<NoximFlit> flit_tx_local; // The output channels
	sc_signal<bool> req_tx_local; // The requests associated with the output channels
	sc_signal<bool> ack_tx_local; // The outgoing ack signals associated with the output channels

	sc_signal<int> free_slots_local;
	sc_signal<int> free_slots_neighbor_local;

	// Router pin assignments
	NoximHMRouter* r = new NoximHMRouter("Router");
	r->clock(clock);
	r->reset(reset);
	for (int i = 0; i < DIRECTIONS_HM; i++) {
		r->flit_rx[i](flit_rx[i]);
		r->req_rx[i](req_rx[i]);
		r->ack_rx[i](ack_rx[i]);

		r->flit_tx[i](flit_tx[i]);
		r->req_tx[i](req_tx[i]);
		r->ack_tx[i](ack_tx[i]);

		r->free_slots[i](free_slots[i]);
		r->free_slots_neighbor[i](free_slots_neighbor[i]);
	}

	r->flit_rx[DIRECTION_HM_LOCAL](flit_tx_local);
	r->req_rx[DIRECTION_HM_LOCAL](req_tx_local);
	r->ack_rx[DIRECTION_HM_LOCAL](ack_tx_local);

	r->flit_tx[DIRECTION_HM_LOCAL](flit_rx_local);
	r->req_tx[DIRECTION_HM_LOCAL](req_rx_local);
	r->ack_tx[DIRECTION_HM_LOCAL](ack_rx_local);

	r->free_slots[DIRECTION_HM_LOCAL](free_slots_local);
	r->free_slots_neighbor[DIRECTION_HM_LOCAL](free_slots_neighbor_local);

	cout << "reset " << reset << endl;
	reset.write(1);
	cout << "reset " << reset << endl;

	//sc_start(2, SC_NS);
}

void NoximHMRouterTest::testRoutingMinusXPlusZFirst() {
	NoximHMRouter* router = new NoximHMRouter("router");

	//----------start from +x--------------
	//+x -> -z
	vector<int> ret = router->routingMinusXPlusZFirst(NoximHMCoord(1, 0, 0),
			NoximHMCoord(1, 1, 0));
	printVector(ret);
	test_(ret.size()==1);
	test_(ret[0] == DIRECTION_PY);

	//+x -> py
	ret = router->routingMinusXPlusZFirst(NoximHMCoord(1, 0, 0),
			NoximHMCoord(0, 1, 0));
	printVector(ret);
	test_(ret.size()==4);
	test_(ret[0] == DIRECTION_PZ);
	test_(ret[1] == DIRECTION_MX);
	test_(ret[2] == DIRECTION_PY);
	test_(ret[3] == DIRECTION_MZ);

	//+x -> -x
	ret = router->routingMinusXPlusZFirst(NoximHMCoord(1, 0, 0),
			NoximHMCoord(0, 1, 1));
	printVector(ret);
	test_(ret.size()==3);
	test_(ret[0] == DIRECTION_PZ);
	test_(ret[1] == DIRECTION_MX);
	test_(ret[2] == DIRECTION_PY);

	//+x -> +z
	ret = router->routingMinusXPlusZFirst(NoximHMCoord(1, 0, 0),
			NoximHMCoord(0, 0, 1));
	printVector(ret);
	test_(ret.size()==2);
	test_(ret[0] == DIRECTION_PZ);
	test_(ret[1] == DIRECTION_MX);

	//+x -> -y
	ret = router->routingMinusXPlusZFirst(NoximHMCoord(1, 0, 0),
			NoximHMCoord(1, 0, 1));
	printVector(ret);
	test_(ret.size()==1);
	test_(ret[0] == DIRECTION_PZ);

	//-------------start from -z------------
	//-z -> +y
	ret = router->routingMinusXPlusZFirst(NoximHMCoord(1, 1, 0),
			NoximHMCoord(0, 1, 0));
	printVector(ret);
	test_(ret.size()==1);
	test_(ret[0] == DIRECTION_MX);

	//-z -> -x
	ret = router->routingMinusXPlusZFirst(NoximHMCoord(1, 1, 0),
			NoximHMCoord(0, 1, 1));
	printVector(ret);
	test_(ret.size()==2);
	test_(ret[0] == DIRECTION_MX);
	test_(ret[1] == DIRECTION_PZ);

	//-z -> +z
	ret = router->routingMinusXPlusZFirst(NoximHMCoord(1, 1, 0),
			NoximHMCoord(0, 0, 1));
	printVector(ret);
	test_(ret.size()==3);
	test_(ret[0] == DIRECTION_MX);
	test_(ret[1] == DIRECTION_PZ);
	test_(ret[2] == DIRECTION_MY);

	//-z -> -y
	ret = router->routingMinusXPlusZFirst(NoximHMCoord(1, 1, 0),
			NoximHMCoord(1, 0, 1));
	printVector(ret);
	test_(ret.size()==4);
	test_(ret[0] == DIRECTION_MX);
	test_(ret[1] == DIRECTION_PZ);
	test_(ret[2] == DIRECTION_MY);
	test_(ret[3] == DIRECTION_PX);

	//-z -> +x
	ret = router->routingMinusXPlusZFirst(NoximHMCoord(1, 1, 0),
			NoximHMCoord(1, 0, 0));
	printVector(ret);
	test_(ret.size()==1);
	test_(ret[0] == DIRECTION_MY);

	//-------start from +y-------------
	// -> -x
	ret = router->routingMinusXPlusZFirst(NoximHMCoord(0, 1, 0),
			NoximHMCoord(0, 1, 1));
	printVector(ret);
	test_(ret.size()==1);
	test_(ret[0] == DIRECTION_PZ);

	// -> +z
	ret = router->routingMinusXPlusZFirst(NoximHMCoord(0, 1, 0),
			NoximHMCoord(0, 0, 1));
	printVector(ret);
	test_(ret.size()==2);
	test_(ret[0] == DIRECTION_PZ);
	test_(ret[1] == DIRECTION_MY);

	// -> -y
	ret = router->routingMinusXPlusZFirst(NoximHMCoord(0, 1, 0),
			NoximHMCoord(1, 0, 1));
	printVector(ret);
	test_(ret.size()==3);
	test_(ret[0] == DIRECTION_PZ);
	test_(ret[1] == DIRECTION_MY);
	test_(ret[2] == DIRECTION_PX);

	// -> +x
	ret = router->routingMinusXPlusZFirst(NoximHMCoord(0, 1, 0),
			NoximHMCoord(1, 0, 0));
	printVector(ret);
	test_(ret.size()==2);
	test_(ret[0] == DIRECTION_PX);
	test_(ret[1] == DIRECTION_MY);

	// -> -z
	ret = router->routingMinusXPlusZFirst(NoximHMCoord(0, 1, 0),
			NoximHMCoord(1, 1, 0));
	printVector(ret);
	test_(ret.size()==1);
	test_(ret[0] == DIRECTION_PX);

	//-------------start from -x------------
	// >> +z
	ret = router->routingMinusXPlusZFirst(NoximHMCoord(0, 1, 1),
			NoximHMCoord(0, 0, 1));
	printVector(ret);
	test_(ret.size()==1);
	test_(ret[0] == DIRECTION_MY);

	// >> -y
	ret = router->routingMinusXPlusZFirst(NoximHMCoord(0, 1, 1),
			NoximHMCoord(1, 0, 1));
	printVector(ret);
	test_(ret.size()==2);
	test_(ret[0] == DIRECTION_MY);
	test_(ret[1] == DIRECTION_PX);

	// >> +x
	ret = router->routingMinusXPlusZFirst(NoximHMCoord(0, 1, 1),
			NoximHMCoord(1, 0, 0));
	printVector(ret);
	test_(ret.size()==3);
	test_(ret[0] == DIRECTION_MY);
	test_(ret[1] == DIRECTION_PX);
	test_(ret[2] == DIRECTION_MZ);

	// >> -z
	ret = router->routingMinusXPlusZFirst(NoximHMCoord(0, 1, 1),
			NoximHMCoord(1, 1, 0));
	printVector(ret);
	test_(ret.size()==2);
	test_(ret[0] == DIRECTION_MZ);
	test_(ret[1] == DIRECTION_PX);

	// >> -y
	ret = router->routingMinusXPlusZFirst(NoximHMCoord(0, 1, 1),
			NoximHMCoord(0, 1, 0));
	printVector(ret);
	test_(ret.size()==1);
	test_(ret[0] == DIRECTION_MZ);

	// -------start from +z--------------
	// >> -x
	ret = router->routingMinusXPlusZFirst(NoximHMCoord(0, 0, 1),
			NoximHMCoord(1, 0, 1));
	printVector(ret);
	test_(ret.size()==1);
	test_(ret[0] == DIRECTION_PX);

	// >> +x
	ret = router->routingMinusXPlusZFirst(NoximHMCoord(0, 0, 1),
			NoximHMCoord(1, 0, 0));
	printVector(ret);
	test_(ret.size()==2);
	test_(ret[0] == DIRECTION_PX);
	test_(ret[1] == DIRECTION_MZ);

	// >> -z
	ret = router->routingMinusXPlusZFirst(NoximHMCoord(0, 0, 1),
			NoximHMCoord(1, 1, 0));
	printVector(ret);
	test_(ret.size()==3);
	test_(ret[0] == DIRECTION_PY);
	test_(ret[1] == DIRECTION_MZ);
	test_(ret[2] == DIRECTION_PX);

	// >> +y
	ret = router->routingMinusXPlusZFirst(NoximHMCoord(0, 0, 1),
			NoximHMCoord(0, 1, 0));
	printVector(ret);
	test_(ret.size()==2);
	test_(ret[0] == DIRECTION_PY);
	test_(ret[1] == DIRECTION_MZ);

	// >> -x
	ret = router->routingMinusXPlusZFirst(NoximHMCoord(0, 0, 1),
			NoximHMCoord(0, 1, 1));
	printVector(ret);
	test_(ret.size()==1);
	test_(ret[0] == DIRECTION_PY);

	//-------start from -y------------
	// >> +x
	ret = router->routingMinusXPlusZFirst(NoximHMCoord(1, 0, 1),
			NoximHMCoord(1, 0, 0));
	printVector(ret);
	test_(ret.size()==1);
	test_(ret[0] == DIRECTION_MZ);

	// >> -z
	ret = router->routingMinusXPlusZFirst(NoximHMCoord(1, 0, 1),
			NoximHMCoord(1, 1, 0));
	printVector(ret);
	test_(ret.size()==2);
	test_(ret[0] == DIRECTION_MZ);
	test_(ret[1] == DIRECTION_PY);

	// >> +y
	ret = router->routingMinusXPlusZFirst(NoximHMCoord(1, 0, 1),
			NoximHMCoord(0, 1, 0));
	printVector(ret);
	test_(ret.size()==3);
	test_(ret[0] == DIRECTION_MX);
	test_(ret[1] == DIRECTION_PY);
	test_(ret[2] == DIRECTION_MZ);

	// >> -x
	ret = router->routingMinusXPlusZFirst(NoximHMCoord(1, 0, 1),
			NoximHMCoord(0, 1, 1));
	printVector(ret);
	test_(ret.size()==2);
	test_(ret[0] == DIRECTION_MX);
	test_(ret[1] == DIRECTION_PY);

	// >> +z
	ret = router->routingMinusXPlusZFirst(NoximHMCoord(1, 0, 1),
			NoximHMCoord(0, 0, 1));
	printVector(ret);
	test_(ret.size()==1);
	test_(ret[0] == DIRECTION_MX);

	//arbitary   commented out, currently by default hmsize = 1
	/*
	 ret = router->routingMinusXPlusZFirst(NoximHMCoord(3,0,-2),NoximHMCoord(-3,1,4));
	 printVector(ret);
	 test_(ret.size()==13);
	 test_(ret[0] == DIRECTION_PZ);
	 test_(ret[1] == DIRECTION_MX);
	 test_(ret[2] == DIRECTION_PZ);
	 test_(ret[3] == DIRECTION_MX);
	 test_(ret[4] == DIRECTION_PZ);
	 test_(ret[5] == DIRECTION_MX);
	 test_(ret[6] == DIRECTION_PZ);
	 test_(ret[7] == DIRECTION_MX);
	 test_(ret[8] == DIRECTION_PZ);
	 test_(ret[9] == DIRECTION_MX);
	 test_(ret[10] == DIRECTION_PZ);
	 test_(ret[11] == DIRECTION_MX);
	 test_(ret[12] == DIRECTION_PY);
	 */
}

void NoximHMRouterTest::printVector(const vector<int>& v) {
	cout << "vector: ";
	for (int i = 0; i < v.size(); i++)
		cout << v[i] << ",";
	cout << endl;
}

