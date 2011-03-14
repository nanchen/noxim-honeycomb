/*
 * Noxim - the NoC Simulator
 *
 * (C) 2005-2010 by the University of Catania
 * For the complete list of authors refer to file ../doc/AUTHORS.txt
 * For the license applied to these sources refer to file ../doc/LICENSE.txt
 *
 * This file contains the implementation of the Network-on-Chip
 */
#include <math.h>

#include "NoximHMNoC.h"

extern "C"{
#include "queue.h"
}

using namespace std;

void NoximHMNoC::buildHoneycombMesh()
{
    cout << "buildHoneycombMesh()" <<endl;
    // Check for routing table availability
    if (NoximGlobalParams::routing_algorithm == ROUTING_TABLE_BASED)
        assert(grtable.load(NoximGlobalParams::routing_table_filename));

    // Check for traffic table availability
    if (NoximGlobalParams::traffic_distribution == TRAFFIC_TABLE_BASED)
        assert(gttable.load(NoximGlobalParams::traffic_table_filename));

    // Create honeycomb mesh
    NoximHexagon* root = NoximHexagon::buildHexagonTree(NoximGlobalParams::honeycomb_mesh_size);

    // Travese (level-order) the tree
//    Queue q = Queue_CreateQueue(100);
//    Queue_Enqueue(root,q);
//    while(!Queue_IsEmpty(q))
//    {
//        NoximHexagon* h = (NoximHexagon*) Queue_FrontAndDequeue(q);
//
//    }

    // Iterate over the tile array
    for(int x=-MAX_ABSOLUTE_COORD_VALUE; x<MAX_ABSOLUTE_COORD_VALUE+1; x++){
        for(int y=-MAX_ABSOLUTE_COORD_VALUE; y<MAX_ABSOLUTE_COORD_VALUE+1; y++){
            for(int z=-MAX_ABSOLUTE_COORD_VALUE; z<MAX_ABSOLUTE_COORD_VALUE+1; z++){
                NoximHMTile* tile = NoximHexagon::getTile(x,y,z);

                const int MESH_SIZE = NoximGlobalParams::honeycomb_mesh_size;
                const int id = x * MESH_SIZE * MESH_SIZE + y * MESH_SIZE + z;

                // Tell to the router its coordinates
                tile->r->configure(id,
                          NoximGlobalParams::stats_warm_up_time,
                          NoximGlobalParams::buffer_depth,
                          grtable);

                // Tell to the PE its coordinates
                tile->pe->local_id = id;
                tile->pe->traffic_table = &gttable;	// Needed to choose destination
                tile->pe->never_transmit = (gttable.occurrencesAsSource(tile->pe->local_id) == 0);

                // Map clock and reset
                tile->clock(clock);
                tile->reset(reset);

                // Map Rx signals
                /*
                tile->req_rx[DIRECTION_NORTH] (req_to_south[i][j]);
                tile->flit_rx[DIRECTION_NORTH] (flit_to_south[i][j]);
                tile->ack_rx[DIRECTION_NORTH] (ack_to_north[i][j]);

                tile->req_rx[DIRECTION_EAST] (req_to_west[i + 1][j]);
                tile->flit_rx[DIRECTION_EAST] (flit_to_west[i + 1][j]);
                tile->ack_rx[DIRECTION_EAST] (ack_to_east[i + 1][j]);

                tile->req_rx[DIRECTION_SOUTH] (req_to_north[i][j + 1]);
                tile->flit_rx[DIRECTION_SOUTH] (flit_to_north[i][j + 1]);
                tile->ack_rx[DIRECTION_SOUTH] (ack_to_south[i][j + 1]);

                tile->req_rx[DIRECTION_WEST] (req_to_east[i][j]);
                tile->flit_rx[DIRECTION_WEST] (flit_to_east[i][j]);
                tile->ack_rx[DIRECTION_WEST] (ack_to_west[i][j]);
                */

                // Map Tx signals
//                tile->req_tx[DIRECTION_NORTH] (req_to_north[i][j]);
//                tile->flit_tx[DIRECTION_NORTH] (flit_to_north[i][j]);
//                tile->ack_tx[DIRECTION_NORTH] (ack_to_south[i][j]);
                if(NoximHexagon::getNeighborTile(x, y, z, DIRECTION_PX))
                {
                    tile->req_tx[DIRECTION_PX] (req_to_px[x][y][z]);
                    tile->flit_tx[DIRECTION_PX] (flit_to_px[x][y][z]);
                    tile->ack_tx[DIRECTION_PX] (ack_to_mx[x][y][z]);
                }

//                tile->req_tx[DIRECTION_EAST] (req_to_east[i + 1][j]);
//                tile->flit_tx[DIRECTION_EAST] (flit_to_east[i + 1][j]);
//                tile->ack_tx[DIRECTION_EAST] (ack_to_west[i + 1][j]);
//
//                tile->req_tx[DIRECTION_SOUTH] (req_to_south[i][j + 1]);
//                tile->flit_tx[DIRECTION_SOUTH] (flit_to_south[i][j + 1]);
//                tile->ack_tx[DIRECTION_SOUTH] (ack_to_north[i][j + 1]);
//
//                tile->req_tx[DIRECTION_WEST] (req_to_west[i][j]);
//                tile->flit_tx[DIRECTION_WEST] (flit_to_west[i][j]);
//                tile->ack_tx[DIRECTION_WEST] (ack_to_east[i][j]);

                // Map buffer level signals (analogy with req_tx/rx port mapping)
//                tile->free_slots[DIRECTION_NORTH] (free_slots_to_north[i][j]);
//                tile->free_slots[DIRECTION_EAST] (free_slots_to_east[i + 1][j]);
//                tile->free_slots[DIRECTION_SOUTH] (free_slots_to_south[i][j + 1]);
//                tile->free_slots[DIRECTION_WEST] (free_slots_to_west[i][j]);
//
//                tile->free_slots_neighbor[DIRECTION_NORTH] (free_slots_to_south[i][j]);
//                tile->free_slots_neighbor[DIRECTION_EAST] (free_slots_to_west[i + 1][j]);
//                tile->free_slots_neighbor[DIRECTION_SOUTH] (free_slots_to_north[i][j + 1]);
//                tile->free_slots_neighbor[DIRECTION_WEST] (free_slots_to_east[i][j]);

                // NoP
//                tile->NoP_data_out[DIRECTION_NORTH] (NoP_data_to_north[i][j]);
//                tile->NoP_data_out[DIRECTION_EAST] (NoP_data_to_east[i + 1][j]);
//                tile->NoP_data_out[DIRECTION_SOUTH] (NoP_data_to_south[i][j + 1]);
//                tile->NoP_data_out[DIRECTION_WEST] (NoP_data_to_west[i][j]);
//
//                tile->NoP_data_in[DIRECTION_NORTH] (NoP_data_to_south[i][j]);
//                tile->NoP_data_in[DIRECTION_EAST] (NoP_data_to_west[i + 1][j]);
//                tile->NoP_data_in[DIRECTION_SOUTH] (NoP_data_to_north[i][j + 1]);
//                tile->NoP_data_in[DIRECTION_WEST] (NoP_data_to_east[i][j]);
            }
        }
    }


    // dummy NoximNoP_data structure
    NoximNoP_data tmp_NoP;

    tmp_NoP.sender_id = NOT_VALID;

    for (int i = 0; i < DIRECTIONS_HM; i++) {
        tmp_NoP.channel_status_neighbor[i].free_slots = NOT_VALID;
        tmp_NoP.channel_status_neighbor[i].available = false;
    }

    /*
    // Clear signals for borderline nodes
    for (int i = 0; i <= NoximGlobalParams::mesh_dim_x; i++) {
	req_to_south[i][0] = 0;
	ack_to_north[i][0] = 0;
	req_to_north[i][NoximGlobalParams::mesh_dim_y] = 0;
	ack_to_south[i][NoximGlobalParams::mesh_dim_y] = 0;

	free_slots_to_south[i][0].write(NOT_VALID);
	free_slots_to_north[i][NoximGlobalParams::mesh_dim_y].write(NOT_VALID);

	NoP_data_to_south[i][0].write(tmp_NoP);
	NoP_data_to_north[i][NoximGlobalParams::mesh_dim_y].write(tmp_NoP);

    }

    for (int j = 0; j <= NoximGlobalParams::mesh_dim_y; j++) {
	req_to_east[0][j] = 0;
	ack_to_west[0][j] = 0;
	req_to_west[NoximGlobalParams::mesh_dim_x][j] = 0;
	ack_to_east[NoximGlobalParams::mesh_dim_x][j] = 0;

	free_slots_to_east[0][j].write(NOT_VALID);
	free_slots_to_west[NoximGlobalParams::mesh_dim_x][j].write(NOT_VALID);

	NoP_data_to_east[0][j].write(tmp_NoP);
	NoP_data_to_west[NoximGlobalParams::mesh_dim_x][j].write(tmp_NoP);

    }

    // invalidate reservation table entries for non-exhistent channels
    for (int i = 0; i < NoximGlobalParams::mesh_dim_x; i++) {
	t[i][0]->r->reservation_table.invalidate(DIRECTION_NORTH);
	t[i][NoximGlobalParams::mesh_dim_y - 1]->r->reservation_table.invalidate(DIRECTION_SOUTH);
    }
    for (int j = 0; j < NoximGlobalParams::mesh_dim_y; j++) {
	t[0][j]->r->reservation_table.invalidate(DIRECTION_WEST);
	t[NoximGlobalParams::mesh_dim_x - 1][j]->r->reservation_table.invalidate(DIRECTION_EAST);
    }
    */
}



//NoximHMTile *NoximHMNoC::searchNode(const int id) const
//{
//    for (int i = 0; i < NoximGlobalParams::mesh_dim_x; i++)
//	for (int j = 0; j < NoximGlobalParams::mesh_dim_y; j++)
//	    if (t[i][j]->r->local_id == id)
//		return t[i][j];
//
//    return false;
//}
