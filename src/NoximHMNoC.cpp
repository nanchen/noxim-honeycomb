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

extern "C" {
#include "queue.h"
}

static const int OFFSET = NoximHexagon::OFFSET;

void printNeighborSignals(const int dir, const NoximHMTile* tile) {
	NoximHMTile* nTile = NoximHexagon::getNeighborTile(tile->getCoord(), dir);
	if (nTile)
		printf("\t<==>[@%s] %s\n", NoximHexagon::getDirectionStr(dir),
				nTile->toString());
	else
		printf("\t<==>[@%s] NULL\n", NoximHexagon::getDirectionStr(dir));
}

void NoximHMNoC::buildHoneycombMesh() {
	// Check for routing table availability
	if (NoximGlobalParams::routing_algorithm == ROUTING_TABLE_BASED)
		assert(grtable.load(NoximGlobalParams::routing_table_filename));

	// Check for traffic table availability
	if (NoximGlobalParams::traffic_distribution == TRAFFIC_TABLE_BASED)
		assert(gttable.load(NoximGlobalParams::traffic_table_filename));

	// Create honeycomb mesh
	NoximHexagon* root = NoximHexagon::buildHexagonTree(
			NoximGlobalParams::honeycomb_mesh_size);

	// Travese (level-order) the tree
	//    Queue q = Queue_CreateQueue(100);
	//    Queue_Enqueue(root,q);
	//    while(!Queue_IsEmpty(q))
	//    {
	//        NoximHexagon* h = (NoximHexagon*) Queue_FrontAndDequeue(q);
	//
	//    }

	//    // dummy NoximNoP_data structure
	//    NoximNoP_data tmp_NoP;
	//
	//    tmp_NoP.sender_id = NOT_VALID;
	//
	//    for (int i = 0; i < DIRECTIONS_HM; i++) {
	//        tmp_NoP.channel_status_neighbor[i].free_slots = NOT_VALID;
	//        tmp_NoP.channel_status_neighbor[i].available = false;
	//    }


	// Iterate over all tiles
	for (int i = 0; i <= NoximHexagon::getLatestId(); i++) {
		NoximHMTile* tile = NoximHexagon::getTile(i);
		cout << "NoximHMNoC::Building signals for " << tile->toString() << endl;

		//coord
		NoximHMCoord coord = tile->getCoord();
		int x = coord.x + OFFSET;
		int y = coord.y + OFFSET;
		int z = coord.z + OFFSET;

		// Map clock and reset
		tile->clock(clock);
		tile->reset(reset);

		// Tell to the router its id
		tile->r->configure(tile->getId(),
				NoximGlobalParams::stats_warm_up_time,
				NoximGlobalParams::buffer_depth, grtable);

		// Tell to the PE its coordinates
		tile->pe->traffic_table = &gttable; // Needed to choose destination
		tile->pe->never_transmit = (gttable.occurrencesAsSource(
				tile->pe->local_id) == 0);

		// Signal binding
		if (tile->getType() == NoximHMTile::POSITIVE) {
			printNeighborSignals(DIRECTION_PX, tile);
			// in direction: rx, free_slots_neighbor, NoP_data_in
			tile->req_rx[DIR_X](req_to_mx[x][y][z]);
			tile->flit_rx[DIR_X](flit_to_mx[x][y][z]);
			tile->ack_rx[DIR_X](ack_to_px[x][y][z]);
			tile->free_slots_neighbor[DIR_X](free_slots_to_mx[x][y][z]);
			//    				tile->NoP_data_in[DIRECTION_PX] (NoP_data_to_mx[x][y][z]);
			// tx
			tile->req_tx[DIR_X](req_to_px[x + 1][y][z]);
			tile->flit_tx[DIR_X](flit_to_px[x + 1][y][z]);
			tile->ack_tx[DIR_X](ack_to_mx[x + 1][y][z]);
			tile->free_slots[DIR_X](free_slots_to_px[x + 1][y][z]);
			//                    tile->NoP_data_out[DIRECTION_PX](NoP_data_to_px[x+1][y][z]);

			printNeighborSignals(DIRECTION_PY, tile);
			// rx
			tile->req_rx[DIR_Y](req_to_my[x][y][z]);
			tile->flit_rx[DIR_Y](flit_to_my[x][y][z]);
			tile->ack_rx[DIR_Y](ack_to_py[x][y][z]);
			tile->free_slots_neighbor[DIR_Y](free_slots_to_my[x][y][z]);
			//       				tile->NoP_data_in[DIRECTION_PY] (NoP_data_to_my[x][y][z]);
			// tx
			tile->req_tx[DIR_Y](req_to_py[x][y + 1][z]);
			tile->flit_tx[DIR_Y](flit_to_py[x][y + 1][z]);
			tile->ack_tx[DIR_Y](ack_to_my[x][y + 1][z]);
			tile->free_slots[DIR_Y](free_slots_to_py[x][y + 1][z]);
			//                    tile->NoP_data_out[DIRECTION_PY](NoP_data_to_py[x][y+1][z]);

			printNeighborSignals(DIRECTION_PZ, tile);
			// rx
			tile->req_rx[DIR_Z](req_to_mz[x][y][z]);
			tile->flit_rx[DIR_Z](flit_to_mz[x][y][z]);
			tile->ack_rx[DIR_Z](ack_to_pz[x][y][z]);
			tile->free_slots_neighbor[DIR_Z](free_slots_to_mz[x][y][z]);
			//       				tile->NoP_data_in[DIRECTION_PZ] (NoP_data_to_mz[x][y][z]);
			// tx
			tile->req_tx[DIR_Z](req_to_pz[x][y][z + 1]);
			tile->flit_tx[DIR_Z](flit_to_pz[x][y][z + 1]);
			tile->ack_tx[DIR_Z](ack_to_mz[x][y][z + 1]);
			tile->free_slots[DIR_Z](free_slots_to_pz[x][y][z + 1]);
			//                    tile->NoP_data_out[DIRECTION_PZ](NoP_data_to_pz[x][y][z+1]);
		}
		else if (tile->getType() == NoximHMTile::NEGATIVE) {
			printNeighborSignals(DIRECTION_MX, tile);
			//rx
			tile->req_rx[DIR_X](req_to_px[x][y][z]);
			tile->flit_rx[DIR_X](flit_to_px[x][y][z]);
			tile->ack_rx[DIR_X](ack_to_mx[x][y][z]);
			tile->free_slots_neighbor[DIR_X](free_slots_to_px[x][y][z]);
			//    				tile->NoP_data_in[DIRECTION_MX] (NoP_data_to_px[x+1][y][z]);
			//tx
			tile->req_tx[DIR_X](req_to_mx[x - 1][y][z]);
			tile->flit_tx[DIR_X](flit_to_mx[x - 1][y][z]);
			tile->ack_tx[DIR_X](ack_to_px[x - 1][y][z]);
			tile->free_slots[DIR_X](free_slots_to_mx[x - 1][y][z]);
			//                    tile->NoP_data_out[DIRECTION_MX](NoP_data_to_mx[x][y][z]);

			printNeighborSignals(DIRECTION_MY, tile);
			//rx
			tile->req_rx[DIR_Y](req_to_py[x][y][z]);
			tile->flit_rx[DIR_Y](flit_to_py[x][y][z]);
			tile->ack_rx[DIR_Y](ack_to_my[x][y][z]);
			tile->free_slots_neighbor[DIR_Y](free_slots_to_py[x][y][z]);
			//    				tile->NoP_data_in[DIRECTION_MY] (NoP_data_to_py[x][y+1][z]);
			//tx
			tile->req_tx[DIR_Y](req_to_my[x][y - 1][z]);
			tile->flit_tx[DIR_Y](flit_to_my[x][y - 1][z]);
			tile->ack_tx[DIR_Y](ack_to_py[x][y - 1][z]);
			tile->free_slots[DIR_Y](free_slots_to_my[x][y - 1][z]);
			//                    tile->NoP_data_out[DIRECTION_MY](NoP_data_to_my[x][y][z]);

			printNeighborSignals(DIRECTION_MZ, tile);
			//rx
			tile->req_rx[DIR_Z](req_to_pz[x][y][z]);
			tile->flit_rx[DIR_Z](flit_to_pz[x][y][z]);
			tile->ack_rx[DIR_Z](ack_to_mz[x][y][z]);
			tile->free_slots_neighbor[DIR_Z](free_slots_to_pz[x][y][z]);
			//    				tile->NoP_data_in[DIRECTION_MZ] (NoP_data_to_pz[x][y][z+1]);
			//tx
			tile->req_tx[DIR_Z](req_to_mz[x][y][z - 1]);
			tile->flit_tx[DIR_Z](flit_to_mz[x][y][z - 1]);
			tile->ack_tx[DIR_Z](ack_to_pz[x][y][z - 1]);
			tile->free_slots[DIR_Z](free_slots_to_mz[x][y][z - 1]);
			//                    tile->NoP_data_out[DIRECTION_MZ](NoP_data_to_mz[x][y][z]);
		}
		// neighbor tiles
		// +x
		NoximHMTile* n = NoximHexagon::getNeighborTile(coord, DIRECTION_PX);
		if (n)
			tile->nTile[DIRECTION_PX] = n;
		// -x
		n = NoximHexagon::getNeighborTile(coord, DIRECTION_MX);
		if (n)
			tile->nTile[DIRECTION_MX] = n;
		n = NoximHexagon::getNeighborTile(coord, DIRECTION_PY);
		if (n)
			tile->nTile[DIRECTION_PY] = n;
		n = NoximHexagon::getNeighborTile(coord, DIRECTION_MY);
		if (n)
			tile->nTile[DIRECTION_MY] = n;
		n = NoximHexagon::getNeighborTile(coord, DIRECTION_PZ);
		if (n)
			tile->nTile[DIRECTION_PZ] = n;
		n = NoximHexagon::getNeighborTile(coord, DIRECTION_MZ);
		if (n)
			tile->nTile[DIRECTION_MZ] = n;
	}

	// Iterate over the tile array
	//    for(int x=-MAX_ABSOLUTE_COORD_VALUE; x<MAX_ABSOLUTE_COORD_VALUE+1; x++){
	//        for(int y=-MAX_ABSOLUTE_COORD_VALUE; y<MAX_ABSOLUTE_COORD_VALUE+1; y++){
	//            for(int z=-MAX_ABSOLUTE_COORD_VALUE; z<MAX_ABSOLUTE_COORD_VALUE+1; z++){
	//                NoximHMTile* tile = NoximHexagon::getTile(x,y,z);
	//
	//                // in case there is no tile at x, y, z
	//                if(tile == NULL)
	//                    continue;
	//
	//                cout << "checking signals for tile at (" << x << ", " << y << ", " << z << "):" << endl;
	//                for(int x0=-MAX_ABSOLUTE_COORD_VALUE; x0<MAX_ABSOLUTE_COORD_VALUE+1; x0++){
	//                      for(int y0=-MAX_ABSOLUTE_COORD_VALUE; y0<MAX_ABSOLUTE_COORD_VALUE+1; y0++){
	//                          for(int z0=-MAX_ABSOLUTE_COORD_VALUE; z0<MAX_ABSOLUTE_COORD_VALUE+1; z0++){
	//                        	  NoximHMTile* tile0 = NoximHexagon::getTile(x0,y0,z0);
	//
	//                        	  if(tile0 == NULL)
	//                        		  continue;
	//                        	  if(tile->req_tx[DIRECTION_PX]==tile0->req_rx[DIRECTION_MX])
	//                        		  cout << "px ==  mx" << endl;
	//                          }
	//                     }
	//                }
	//            }
	//        }
	//    }

	// dummy NoximNoP_data structure
	/*
	 NoximNoP_data tmp_NoP;

	 tmp_NoP.sender_id = NOT_VALID;

	 for (int i = 0; i < DIRECTIONS_HM; i++) {
	 tmp_NoP.channel_status_neighbor[i].free_slots = NOT_VALID;
	 tmp_NoP.channel_status_neighbor[i].available = false;
	 }
	 */

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
