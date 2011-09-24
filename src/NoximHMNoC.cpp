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

static const int OFFSET = 10;

char* getDirectionStr(const int dir){
	char* ret = (char*)malloc(2*sizeof(char));
	switch(dir){
		case DIRECTION_PX:
			ret = "+x";
			break;
		case DIRECTION_MX:
			ret = "-x";
			break;
		case DIRECTION_PY:
			ret = "+y";
			break;
		case DIRECTION_MY:
			ret = "-y";
			break;
		case DIRECTION_PZ:
			ret = "+z";
			break;
		case DIRECTION_MZ:
			ret = "-z";
			break;
	}
	return ret;
}

void printNeighborSignals(const int dir, const NoximHMTile* n){
	printf("\t==>%s %s\n",getDirectionStr(dir),n->toString());
}

void NoximHMNoC::buildHoneycombMesh()
{
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

//    // dummy NoximNoP_data structure
//    NoximNoP_data tmp_NoP;
//
//    tmp_NoP.sender_id = NOT_VALID;
//
//    for (int i = 0; i < DIRECTIONS_HM; i++) {
//        tmp_NoP.channel_status_neighbor[i].free_slots = NOT_VALID;
//        tmp_NoP.channel_status_neighbor[i].available = false;
//    }

//    //Initilize signals on tile
//	for (int x = -MAX_ABSOLUTE_COORD_VALUE; x < MAX_ABSOLUTE_COORD_VALUE + 1; x++) {
//		for (int y = -MAX_ABSOLUTE_COORD_VALUE; y < MAX_ABSOLUTE_COORD_VALUE+ 1; y++) {
//			for (int z = -MAX_ABSOLUTE_COORD_VALUE; z< MAX_ABSOLUTE_COORD_VALUE + 1; z++) {
//				NoximHMTile* tile = NULL;
//				tile = NoximHexagon::getTile(x, y, z);
//
//				// in case there is no tile at x, y, z
//				if (tile == NULL)
//					continue;
//
//				for (int i = 0; i < DIRS; i++) {
//					sc_signal<bool> nullBool;
//					sc_signal<NoximFlit> nullFlit;
//					sc_signal<int> nullInt;
//					nullInt.write(5);
//
////					sc_signal<NoximNoP_data> nullNoP;
////
////					nullInt.write(NOT_VALID);
////					nullNoP.write(tmp_NoP);
//
////					tile->flit_rx[i](nullFlit);
////					tile->req_rx[i](nullBool);
////					tile->ack_rx[i](nullBool);
////
////					tile->flit_tx[i](nullFlit);
////					tile->req_tx[i](nullBool);
////					tile->ack_tx[i](nullBool);
//
////					tile->free_slots[i](nullInt);
////					tile->free_slots_neighbor[i](nullInt);
//
//					invalid_free_slots[x+OFFSET][y+OFFSET][z+OFFSET][i].write(NOT_VALID);
//					tile->free_slots_neighbor[i](invalid_free_slots[x+OFFSET][y+OFFSET][z+OFFSET][i]);
////					tile->free_slots_neighbor[i] (free_slots_to_mx[OFFSET][OFFSET][OFFSET]);
//
////					tile->NoP_data_in[i](nullNoP);
////					tile->NoP_data_out[i](nullNoP);
//				}
//			}
//		}
//	}


    // Iterate over the tile array
    for(int x=-MAX_ABSOLUTE_COORD_VALUE; x<MAX_ABSOLUTE_COORD_VALUE+1; x++){
        for(int y=-MAX_ABSOLUTE_COORD_VALUE; y<MAX_ABSOLUTE_COORD_VALUE+1; y++){
            for(int z=-MAX_ABSOLUTE_COORD_VALUE; z<MAX_ABSOLUTE_COORD_VALUE+1; z++){
                NoximHMTile* tile = NULL;
                tile = NoximHexagon::getTile(x,y,z);

                // in case there is no tile at x, y, z
                if(tile == NULL)
                    continue;

                cout << "building signals for " << tile->toString() << endl;

                // Tell to the router its id
				tile->r->configure(tile->getId(), NoximGlobalParams::stats_warm_up_time,
						NoximGlobalParams::buffer_depth, grtable);

				// Tell to the PE its coordinates
				tile->pe->traffic_table = &gttable; // Needed to choose destination
				tile->pe->never_transmit = (gttable.occurrencesAsSource(
						tile->pe->local_id) == 0);

                // Map clock and reset
                tile->clock(clock);
                tile->reset(reset);

                // Signal binding
                if(tile->getType() == NoximHMTile::POSITIVE){
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

//                	printNeighborSignals(DIRECTION_PX,n);
                	// in direction: rx, free_slots_neighbor, NoP_data_in
                    tile->req_rx[DIR_X] (req_to_mx[x+OFFSET][y+OFFSET][z+OFFSET]);
                    tile->flit_rx[DIR_X] (flit_to_mx[x+OFFSET][y+OFFSET][z+OFFSET]);
                    tile->ack_rx[DIR_X] (ack_to_px[x+OFFSET][y+OFFSET][z+OFFSET]);
                    tile->free_slots_neighbor[DIR_X] (free_slots_to_mx[x+OFFSET][y+OFFSET][z+OFFSET]);
//    				tile->NoP_data_in[DIRECTION_PX] (NoP_data_to_mx[x+OFFSET][y+OFFSET][z+OFFSET]);
                    // tx
                    tile->req_tx[DIR_X] (req_to_px[x+OFFSET+1][y+OFFSET][z+OFFSET]);
                    tile->flit_tx[DIR_X] (flit_to_px[x+OFFSET+1][y+OFFSET][z+OFFSET]);
                    tile->ack_tx[DIR_X] (ack_to_mx[x+OFFSET+1][y+OFFSET][z+OFFSET]);
                    tile->free_slots[DIR_X](free_slots_to_px[x+OFFSET+1][y+OFFSET][z+OFFSET]);
//                    tile->NoP_data_out[DIRECTION_PX](NoP_data_to_px[x+OFFSET+1][y+OFFSET][z+OFFSET]);

//                    printNeighborSignals(DIRECTION_PY,n);
                    // rx
                    tile->req_rx[DIR_Y] (req_to_my[x+OFFSET][y+OFFSET][z+OFFSET]);
                    tile->flit_rx[DIR_Y] (flit_to_my[x+OFFSET][y+OFFSET][z+OFFSET]);
                    tile->ack_rx[DIR_Y] (ack_to_py[x+OFFSET][y+OFFSET][z+OFFSET]);
                    tile->free_slots_neighbor[DIR_Y] (free_slots_to_my[x+OFFSET][y+OFFSET][z+OFFSET]);
//       				tile->NoP_data_in[DIRECTION_PY] (NoP_data_to_my[x+OFFSET][y+OFFSET][z+OFFSET]);
                    // tx
                    tile->req_tx[DIR_Y] (req_to_py[x+OFFSET][y+OFFSET+1][z+OFFSET]);
                    tile->flit_tx[DIR_Y] (flit_to_py[x+OFFSET][y+OFFSET+1][z+OFFSET]);
                    tile->ack_tx[DIR_Y] (ack_to_my[x+OFFSET][y+OFFSET+1][z+OFFSET]);
                    tile->free_slots[DIR_Y](free_slots_to_py[x+OFFSET][y+OFFSET+1][z+OFFSET]);
//                    tile->NoP_data_out[DIRECTION_PY](NoP_data_to_py[x+OFFSET][y+OFFSET+1][z+OFFSET]);

//                	printNeighborSignals(DIRECTION_PZ,n);
                    // rx
                    tile->req_rx[DIR_Z] (req_to_mz[x+OFFSET][y+OFFSET][z+OFFSET]);
                    tile->flit_rx[DIR_Z] (flit_to_mz[x+OFFSET][y+OFFSET][z+OFFSET]);
                    tile->ack_rx[DIR_Z] (ack_to_pz[x+OFFSET][y+OFFSET][z+OFFSET]);
                    tile->free_slots_neighbor[DIR_Z] (free_slots_to_mz[x+OFFSET][y+OFFSET][z+OFFSET]);
//       				tile->NoP_data_in[DIRECTION_PZ] (NoP_data_to_mz[x+OFFSET][y+OFFSET][z+OFFSET]);
                    // tx
                    tile->req_tx[DIR_Z] (req_to_pz[x+OFFSET][y+OFFSET][z+OFFSET+1]);
                    tile->flit_tx[DIR_Z] (flit_to_pz[x+OFFSET][y+OFFSET][z+OFFSET+1]);
                    tile->ack_tx[DIR_Z] (ack_to_mz[x+OFFSET][y+OFFSET][z+OFFSET+1]);
                    tile->free_slots[DIR_Z](free_slots_to_pz[x+OFFSET][y+OFFSET][z+OFFSET+1]);
//                    tile->NoP_data_out[DIRECTION_PZ](NoP_data_to_pz[x+OFFSET][y+OFFSET][z+OFFSET+1]);
                }

                else if(tile->getType() == NoximHMTile::NEGATIVE){

                	tile->req_rx[DIR_X] (req_to_px[x+OFFSET][y+OFFSET][z+OFFSET]);
                    tile->flit_rx[DIR_X] (flit_to_px[x+OFFSET][y+OFFSET][z+OFFSET]);
                    tile->ack_rx[DIR_X] (ack_to_mx[x+OFFSET][y+OFFSET][z+OFFSET]);
                    tile->free_slots_neighbor[DIR_X] (free_slots_to_px[x+OFFSET][y+OFFSET][z+OFFSET]);
//    				tile->NoP_data_in[DIRECTION_MX] (NoP_data_to_px[x+OFFSET+1][y+OFFSET][z+OFFSET]);


                    tile->req_tx[DIR_X] (req_to_mx[x+OFFSET-1][y+OFFSET][z+OFFSET]);
                    tile->flit_tx[DIR_X] (flit_to_mx[x+OFFSET-1][y+OFFSET][z+OFFSET]);
                    tile->ack_tx[DIR_X] (ack_to_px[x+OFFSET-1][y+OFFSET][z+OFFSET]);
                    tile->free_slots[DIR_X](free_slots_to_mx[x+OFFSET-1][y+OFFSET][z+OFFSET]);
//                    tile->NoP_data_out[DIRECTION_MX](NoP_data_to_mx[x+OFFSET][y+OFFSET][z+OFFSET]);


                    tile->req_rx[DIR_Y] (req_to_py[x+OFFSET][y+OFFSET][z+OFFSET]);
                    tile->flit_rx[DIR_Y] (flit_to_py[x+OFFSET][y+OFFSET][z+OFFSET]);
                    tile->ack_rx[DIR_Y] (ack_to_my[x+OFFSET][y+OFFSET][z+OFFSET]);
                    tile->free_slots_neighbor[DIR_Y] (free_slots_to_py[x+OFFSET][y+OFFSET][z+OFFSET]);
//    				tile->NoP_data_in[DIRECTION_MY] (NoP_data_to_py[x+OFFSET][y+OFFSET+1][z+OFFSET]);


                    tile->req_tx[DIR_Y] (req_to_my[x+OFFSET][y+OFFSET-1][z+OFFSET]);
                    tile->flit_tx[DIR_Y] (flit_to_my[x+OFFSET][y+OFFSET-1][z+OFFSET]);
                    tile->ack_tx[DIR_Y] (ack_to_py[x+OFFSET][y+OFFSET-1][z+OFFSET]);
                    tile->free_slots[DIR_Y](free_slots_to_my[x+OFFSET][y+OFFSET-1][z+OFFSET]);
//                    tile->NoP_data_out[DIRECTION_MY](NoP_data_to_my[x+OFFSET][y+OFFSET][z+OFFSET]);


                    tile->req_rx[DIR_Z] (req_to_pz[x+OFFSET][y+OFFSET][z+OFFSET]);
                    tile->flit_rx[DIR_Z] (flit_to_pz[x+OFFSET][y+OFFSET][z+OFFSET]);
                    tile->ack_rx[DIR_Z] (ack_to_mz[x+OFFSET][y+OFFSET][z+OFFSET]);
                    tile->free_slots_neighbor[DIR_Z] (free_slots_to_pz[x+OFFSET][y+OFFSET][z+OFFSET]);
//    				tile->NoP_data_in[DIRECTION_MZ] (NoP_data_to_pz[x+OFFSET][y+OFFSET][z+OFFSET+1]);


                    tile->req_tx[DIR_Z] (req_to_mz[x+OFFSET][y+OFFSET][z+OFFSET-1]);
                    tile->flit_tx[DIR_Z] (flit_to_mz[x+OFFSET][y+OFFSET][z+OFFSET-1]);
                    tile->ack_tx[DIR_Z] (ack_to_pz[x+OFFSET][y+OFFSET][z+OFFSET-1]);
                    tile->free_slots[DIR_Z](free_slots_to_mz[x+OFFSET][y+OFFSET][z+OFFSET-1]);
//                    tile->NoP_data_out[DIRECTION_MZ](NoP_data_to_mz[x+OFFSET][y+OFFSET][z+OFFSET]);

                }
                // +x
                NoximHMTile* n = NoximHexagon::getNeighborTile(x, y, z, DIRECTION_PX);
                if(n)
                {
                	tile->nTile[DIRECTION_PX] = n;
                }
                // -x
                n = NoximHexagon::getNeighborTile(x, y, z, DIRECTION_MX);
                if(n)
                {
                    tile->nTile[DIRECTION_MX] = n;
//                    // rx
//                    tile->req_rx[DIRECTION_MX] (req_to_px[x+OFFSET][y+OFFSET][z+OFFSET]);
//                    tile->flit_rx[DIRECTION_MX] (flit_to_px[x+OFFSET][y+OFFSET][z+OFFSET]);
//                    tile->ack_rx[DIRECTION_MX] (ack_to_mx[x+OFFSET][y+OFFSET][z+OFFSET]);
//                    // tx
//                    tile->req_tx[DIRECTION_MX] (req_to_mx[x+OFFSET][y+OFFSET][z+OFFSET]);
//                    tile->flit_tx[DIRECTION_MX] (flit_to_mx[x+OFFSET][y+OFFSET][z+OFFSET]);
//                    tile->ack_tx[DIRECTION_MX] (ack_to_px[x+OFFSET][y+OFFSET][z+OFFSET]);
                }
                n = NoximHexagon::getNeighborTile(x, y, z, DIRECTION_PY);
                if(n)
                {

                    tile->nTile[DIRECTION_PY] = n;


                }
                n = NoximHexagon::getNeighborTile(x, y, z, DIRECTION_MY);
                if(n)
                {
                    tile->nTile[DIRECTION_MY] = n;
//                    // rx
//                    tile->req_rx[DIRECTION_MY] (req_to_py[x+OFFSET][y+OFFSET][z+OFFSET]);
//                    tile->flit_rx[DIRECTION_MY] (flit_to_py[x+OFFSET][y+OFFSET][z+OFFSET]);
//                    tile->ack_rx[DIRECTION_MY] (ack_to_my[x+OFFSET][y+OFFSET][z+OFFSET]);
//                    // tx
//                    tile->req_tx[DIRECTION_MY] (req_to_my[x+OFFSET][y+OFFSET][z+OFFSET]);
//                    tile->flit_tx[DIRECTION_MY] (flit_to_my[x+OFFSET][y+OFFSET][z+OFFSET]);
//                    tile->ack_tx[DIRECTION_MY] (ack_to_py[x+OFFSET][y+OFFSET][z+OFFSET]);
                }
                n = NoximHexagon::getNeighborTile(x, y, z, DIRECTION_PZ);
                if(n)
                {
                    tile->nTile[DIRECTION_PZ] = n;

                }
                n = NoximHexagon::getNeighborTile(x, y, z, DIRECTION_MZ);
                if(n)
                {
                    tile->nTile[DIRECTION_MZ] = n;
//                    // rx
//                    tile->req_rx[DIRECTION_MZ] (req_to_pz[x+OFFSET][y+OFFSET][z+OFFSET]);
//                    tile->flit_rx[DIRECTION_MZ] (flit_to_pz[x+OFFSET][y+OFFSET][z+OFFSET]);
//                    tile->ack_rx[DIRECTION_MZ] (ack_to_mz[x+OFFSET][y+OFFSET][z+OFFSET]);
//                    // tx
//                    tile->req_tx[DIRECTION_MZ] (req_to_mz[x+OFFSET][y+OFFSET][z+OFFSET]);
//                    tile->flit_tx[DIRECTION_MZ] (flit_to_mz[x+OFFSET][y+OFFSET][z+OFFSET]);
//                    tile->ack_tx[DIRECTION_MZ] (ack_to_pz[x+OFFSET][y+OFFSET][z+OFFSET]);
                }
                // Map Rx signals
				//                tile->req_rx[DIRECTION_NORTH] (req_to_south[i][j]);
				//                tile->flit_rx[DIRECTION_NORTH] (flit_to_south[i][j]);
				//                tile->ack_rx[DIRECTION_NORTH] (ack_to_north[i][j]);
				//
				//                tile->req_rx[DIRECTION_EAST] (req_to_west[i + 1][j]);
				//                tile->flit_rx[DIRECTION_EAST] (flit_to_west[i + 1][j]);
				//                tile->ack_rx[DIRECTION_EAST] (ack_to_east[i + 1][j]);
				//
				//                tile->req_rx[DIRECTION_SOUTH] (req_to_north[i][j + 1]);
				//                tile->flit_rx[DIRECTION_SOUTH] (flit_to_north[i][j + 1]);
				//                tile->ack_rx[DIRECTION_SOUTH] (ack_to_south[i][j + 1]);
				//
				//                tile->req_rx[DIRECTION_WEST] (req_to_east[i][j]);
				//                tile->flit_rx[DIRECTION_WEST] (flit_to_east[i][j]);
				//                tile->ack_rx[DIRECTION_WEST] (ack_to_west[i][j]);

				// Map Tx signals
				//                tile->req_tx[DIRECTION_NORTH] (req_to_north[i][j]);
				//                tile->flit_tx[DIRECTION_NORTH] (flit_to_north[i][j]);
				//                tile->ack_tx[DIRECTION_NORTH] (ack_to_south[i][j]);

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