/*
 * Noxim - the NoC Simulator
 *
 * (C) 2005-2010 by the University of Catania
 * For the complete list of authors refer to file ../doc/AUTHORS.txt
 * For the license applied to these sources refer to file ../doc/LICENSE.txt
 *
 * This file represents the top-level testbench
 */

#ifndef __NOXIMHMNOC_H__
#define __NOXIMHMNOC_H__

#include <systemc.h>
#include "NoximHMTile.h"
#include "NoximHexagon.h"
#include "NoximGlobalRoutingTable.h"
#include "NoximGlobalTrafficTable.h"

using namespace std;

SC_MODULE(NoximHMNoC)
{

    // I/O Ports
    sc_in_clk clock;		// The input clock for the NoC
    sc_in < bool > reset;	// The reset signal for the NoC

    // Signals
//    sc_signal <bool> req_to_east[MAX_STATIC_DIM + 1][MAX_STATIC_DIM + 1];
//    sc_signal <bool> req_to_west[MAX_STATIC_DIM + 1][MAX_STATIC_DIM + 1];
//    sc_signal <bool> req_to_south[MAX_STATIC_DIM + 1][MAX_STATIC_DIM + 1];
//    sc_signal <bool> req_to_north[MAX_STATIC_DIM + 1][MAX_STATIC_DIM + 1];
    sc_signal <bool> req_to_px[MAX_STATIC_DIM+1][MAX_STATIC_DIM+1][MAX_STATIC_DIM+1];
    sc_signal <bool> req_to_mx[MAX_STATIC_DIM+1][MAX_STATIC_DIM+1][MAX_STATIC_DIM+1];
    sc_signal <bool> req_to_py[MAX_STATIC_DIM+1][MAX_STATIC_DIM+1][MAX_STATIC_DIM+1];
    sc_signal <bool> req_to_my[MAX_STATIC_DIM+1][MAX_STATIC_DIM+1][MAX_STATIC_DIM+1];
    sc_signal <bool> req_to_pz[MAX_STATIC_DIM+1][MAX_STATIC_DIM+1][MAX_STATIC_DIM+1];
    sc_signal <bool> req_to_mz[MAX_STATIC_DIM+1][MAX_STATIC_DIM+1][MAX_STATIC_DIM+1];

//    sc_signal <bool> ack_to_east[MAX_STATIC_DIM + 1][MAX_STATIC_DIM + 1];
//    sc_signal <bool> ack_to_west[MAX_STATIC_DIM + 1][MAX_STATIC_DIM + 1];
//    sc_signal <bool> ack_to_south[MAX_STATIC_DIM + 1][MAX_STATIC_DIM + 1];
//    sc_signal <bool> ack_to_north[MAX_STATIC_DIM + 1][MAX_STATIC_DIM + 1];
    sc_signal <bool> ack_to_px[MAX_STATIC_DIM+1][MAX_STATIC_DIM+1][MAX_STATIC_DIM+1];
    sc_signal <bool> ack_to_mx[MAX_STATIC_DIM+1][MAX_STATIC_DIM+1][MAX_STATIC_DIM+1];
    sc_signal <bool> ack_to_py[MAX_STATIC_DIM+1][MAX_STATIC_DIM+1][MAX_STATIC_DIM+1];
    sc_signal <bool> ack_to_my[MAX_STATIC_DIM+1][MAX_STATIC_DIM+1][MAX_STATIC_DIM+1];
    sc_signal <bool> ack_to_pz[MAX_STATIC_DIM+1][MAX_STATIC_DIM+1][MAX_STATIC_DIM+1];
    sc_signal <bool> ack_to_mz[MAX_STATIC_DIM+1][MAX_STATIC_DIM+1][MAX_STATIC_DIM+1];

//    sc_signal <NoximFlit> flit_to_east[MAX_STATIC_DIM + 1][MAX_STATIC_DIM + 1];
//    sc_signal <NoximFlit> flit_to_west[MAX_STATIC_DIM + 1][MAX_STATIC_DIM + 1];
//    sc_signal <NoximFlit> flit_to_south[MAX_STATIC_DIM + 1][MAX_STATIC_DIM + 1];
//    sc_signal <NoximFlit> flit_to_north[MAX_STATIC_DIM + 1][MAX_STATIC_DIM + 1];
    sc_signal <NoximFlit> flit_to_px[MAX_STATIC_DIM+1][MAX_STATIC_DIM+1][MAX_STATIC_DIM+1];
    sc_signal <NoximFlit> flit_to_mx[MAX_STATIC_DIM+1][MAX_STATIC_DIM+1][MAX_STATIC_DIM+1];
    sc_signal <NoximFlit> flit_to_py[MAX_STATIC_DIM+1][MAX_STATIC_DIM+1][MAX_STATIC_DIM+1];
    sc_signal <NoximFlit> flit_to_my[MAX_STATIC_DIM+1][MAX_STATIC_DIM+1][MAX_STATIC_DIM+1];
    sc_signal <NoximFlit> flit_to_pz[MAX_STATIC_DIM+1][MAX_STATIC_DIM+1][MAX_STATIC_DIM+1];
    sc_signal <NoximFlit> flit_to_mz[MAX_STATIC_DIM+1][MAX_STATIC_DIM+1][MAX_STATIC_DIM+1];

//    sc_signal <int> free_slots_to_east[MAX_STATIC_DIM + 1][MAX_STATIC_DIM + 1];
//    sc_signal <int> free_slots_to_west[MAX_STATIC_DIM + 1][MAX_STATIC_DIM + 1];
//    sc_signal <int> free_slots_to_south[MAX_STATIC_DIM + 1][MAX_STATIC_DIM + 1];
//    sc_signal <int> free_slots_to_north[MAX_STATIC_DIM + 1][MAX_STATIC_DIM + 1];
    sc_signal <int> free_slots_to_px[MAX_STATIC_DIM+1][MAX_STATIC_DIM+1][MAX_STATIC_DIM+1];
    sc_signal <int> free_slots_to_mx[MAX_STATIC_DIM+1][MAX_STATIC_DIM+1][MAX_STATIC_DIM+1];
    sc_signal <int> free_slots_to_py[MAX_STATIC_DIM+1][MAX_STATIC_DIM+1][MAX_STATIC_DIM+1];
    sc_signal <int> free_slots_to_my[MAX_STATIC_DIM+1][MAX_STATIC_DIM+1][MAX_STATIC_DIM+1];
    sc_signal <int> free_slots_to_pz[MAX_STATIC_DIM+1][MAX_STATIC_DIM+1][MAX_STATIC_DIM+1];
    sc_signal <int> free_slots_to_mz[MAX_STATIC_DIM+1][MAX_STATIC_DIM+1][MAX_STATIC_DIM+1];

    sc_signal <int> invalid_free_slots[MAX_STATIC_DIM+1][MAX_STATIC_DIM+1][MAX_STATIC_DIM+1][DIRECTIONS_HM];


    // NoP
//    sc_signal <NoximNoP_data> NoP_data_to_east[MAX_STATIC_DIM][MAX_STATIC_DIM];
//    sc_signal <NoximNoP_data> NoP_data_to_west[MAX_STATIC_DIM][MAX_STATIC_DIM];
//    sc_signal <NoximNoP_data> NoP_data_to_south[MAX_STATIC_DIM][MAX_STATIC_DIM];
//    sc_signal <NoximNoP_data> NoP_data_to_north[MAX_STATIC_DIM][MAX_STATIC_DIM];

//    sc_signal <NoximNoP_data> NoP_data_to_px[MAX_STATIC_DIM+1][MAX_STATIC_DIM+1][MAX_STATIC_DIM+1];
//    sc_signal <NoximNoP_data> NoP_data_to_mx[MAX_STATIC_DIM+1][MAX_STATIC_DIM+1][MAX_STATIC_DIM+1];
//    sc_signal <NoximNoP_data> NoP_data_to_py[MAX_STATIC_DIM+1][MAX_STATIC_DIM+1][MAX_STATIC_DIM+1];
//    sc_signal <NoximNoP_data> NoP_data_to_my[MAX_STATIC_DIM+1][MAX_STATIC_DIM+1][MAX_STATIC_DIM+1];
//    sc_signal <NoximNoP_data> NoP_data_to_pz[MAX_STATIC_DIM+1][MAX_STATIC_DIM+1][MAX_STATIC_DIM+1];
//    sc_signal <NoximNoP_data> NoP_data_to_mz[MAX_STATIC_DIM+1][MAX_STATIC_DIM+1][MAX_STATIC_DIM+1];

    // Global tables
    NoximGlobalRoutingTable grtable;
    NoximGlobalTrafficTable gttable;

    //---------- Mau experiment <start>
//    void flitsMonitor() {
//
//	if (!reset.read()) {
//	    //      if ((int)sc_simulation_time() % 5)
//	    //        return;
//
//	    unsigned int count = 0;
//	    for (int i = 0; i < NoximGlobalParams::mesh_dim_x; i++)
//		for (int j = 0; j < NoximGlobalParams::mesh_dim_y; j++)
//		    count += t[i][j]->r->getFlitsCount();
//	    cout << count << endl;
//	}
//    }
    //---------- Mau experiment <stop>

    // Constructor

    SC_CTOR(NoximHMNoC) {

	//---------- Mau experiment <start>
	/*
	   SC_METHOD(flitsMonitor);
	   sensitive(reset);
	   sensitive_pos(clock);
	 */
	//---------- Mau experiment <stop>

        // Build the Mesh
        buildHoneycombMesh();
    }

    // Support methods
    NoximHMTile *searchNode(const int id) const;

  private:
      void buildHoneycombMesh();


};

#endif
