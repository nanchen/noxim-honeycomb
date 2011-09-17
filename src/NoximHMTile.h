/*
 * Noxim - the NoC Simulator
 *
 * (C) 2005-2010 by the University of Catania
 * For the complete list of authors refer to file ../doc/AUTHORS.txt
 * For the license applied to these sources refer to file ../doc/LICENSE.txt
 *
 * This file contains the declaration of the tile
 */

#ifndef __NOXIMHMTILE_H__
#define __NOXIMHMTILE_H__

#include <systemc.h>
#include "NoximHMRouter.h"
#include "NoximProcessingElement.h"
#include <stdio.h>

using namespace std;

SC_MODULE(NoximHMTile) {

	// I/O Ports
	sc_in_clk clock; // The input clock for the tile  0
	sc_in<bool> reset; // The reset signal for the tile 1

	sc_in<NoximFlit> flit_rx[DIRS]; // The input channels 2-4
	sc_in<bool> req_rx[DIRS]; // The requests associated with the input channels 5-7
	sc_out<bool> ack_rx[DIRS]; // The outgoing ack signals associated with the input channels 8-10

	sc_out<NoximFlit> flit_tx[DIRS]; // The output channels 11-13
	sc_out<bool> req_tx[DIRS]; // The requests associated with the output channels 14-16
	sc_in<bool> ack_tx[DIRS]; // The outgoing ack signals associated with the output channels 17-19

	sc_out<int> free_slots[DIRS]; //20-22
	sc_in<int> free_slots_neighbor[DIRS]; //23-25

	// NoP related I/O
	//	sc_out<NoximNoP_data> NoP_data_out[DIRS]; //50-55
	//	sc_in<NoximNoP_data> NoP_data_in[DIRS]; //56-61

	// Signals
	sc_signal<NoximFlit> flit_rx_local; // The input channels
	sc_signal<bool> req_rx_local; // The requests associated with the input channels
	sc_signal<bool> ack_rx_local; // The outgoing ack signals associated with the input channels

	sc_signal<NoximFlit> flit_tx_local; // The output channels
	sc_signal<bool> req_tx_local; // The requests associated with the output channels
	sc_signal<bool> ack_tx_local; // The outgoing ack signals associated with the output channels

	sc_signal<int> free_slots_local;
	sc_signal<int> free_slots_neighbor_local;

	// Instances
	NoximHMRouter *r; // Router instance
	NoximProcessingElement *pe; // Processing Element instance

	// neighbor tile
	NoximHMTile* nTile[DIRECTIONS_HM];

	// coord
	NoximHMCoord getCoord() const{
		return coord;
	}
	void setCoord(int x, int y, int z);

	// type
	enum TileType {
		POSITIVE, NEGATIVE, UNDEFINED
	};
	TileType type;
	TileType getType() const;

	// id
	int id;
	void setId(int aId);
	int getId() const;

	char* toString() const;

	// Constructor

	SC_CTOR(NoximHMTile) {

		// init type
		type = UNDEFINED;

		// init neighbor tiles
		for (int i = 0; i < DIRECTIONS_HM; i++)
			nTile[i] = NULL;

//		// init coord
//		setCoord(0,0,0);

		// Router pin assignments
		r = new NoximHMRouter("Router");
		r->setCoord(getCoord());
		r->clock(clock);
		r->reset(reset);
		for (int i = 0; i < DIRS; i++) {
			r->flit_rx[i](flit_rx[i]);
			r->req_rx[i](req_rx[i]);
			r->ack_rx[i](ack_rx[i]);

			r->flit_tx[i](flit_tx[i]);
			r->req_tx[i](req_tx[i]);
			r->ack_tx[i](ack_tx[i]);

			r->free_slots[i](free_slots[i]);
			r->free_slots_neighbor[i](free_slots_neighbor[i]);

			// NoP
			//			r->NoP_data_out[i](NoP_data_out[i]);
			//			r->NoP_data_in[i](NoP_data_in[i]);
		}

		r->flit_rx[DIR_LOCAL](flit_tx_local);
		r->req_rx[DIR_LOCAL](req_tx_local);
		r->ack_rx[DIR_LOCAL](ack_tx_local);

		r->flit_tx[DIR_LOCAL](flit_rx_local);
		r->req_tx[DIR_LOCAL](req_rx_local);
		r->ack_tx[DIR_LOCAL](ack_rx_local);

		r->free_slots[DIR_LOCAL](free_slots_local);
		r->free_slots_neighbor[DIR_LOCAL](free_slots_neighbor_local);

		// Processing Element pin assignments
		pe = new NoximProcessingElement("ProcessingElement");
		pe->clock(clock);
		pe->reset(reset);

		pe->flit_rx(flit_rx_local);
		pe->req_rx(req_rx_local);
		pe->ack_rx(ack_rx_local);

		pe->flit_tx(flit_tx_local);
		pe->req_tx(req_tx_local);
		pe->ack_tx(ack_tx_local);

		pe->free_slots_neighbor(free_slots_neighbor_local);

	}

private:
	NoximHMCoord coord;

};

#endif
