/*
 * NoximHMRouterTB.cpp
 *
 *  Created on: Sep 11, 2011
 *      Author: nan
 */
#include <systemc.h>
#include "NoximHMRouter.h"
#include "NoximMain.h"
#include <assert.h>

int sc_main(int argc, char* argv[]) {
	std::cout << "NoximHMRouterTB" << std::endl;

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

	reset.write(1);

	sc_start(1, SC_NS);
	assert(reset == 1);
	assert(r->routed_flits == 0);
	assert(r->local_drained == 0);
	sc_stop();
	cout << "exiting..." << endl;
}
