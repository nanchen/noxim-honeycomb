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
        printf("\t==>[@%s] %s\n", NoximHexagon::getDirectionStr(dir),
            nTile->toString());
    else
        printf("\t==>[@%s] NULL\n", NoximHexagon::getDirectionStr(dir));
}

void NoximHMNoC::buildHoneycombMesh(int hmSize) {
    assert(hmSize > 0);

    // Check for routing table availability
    if (NoximGlobalParams::routing_algorithm == ROUTING_TABLE_BASED)
        assert(grtable.load(NoximGlobalParams::routing_table_filename));

    // Check for traffic table availability
    if (NoximGlobalParams::traffic_distribution == TRAFFIC_TABLE_BASED)
        assert(gttable.load(NoximGlobalParams::traffic_table_filename));

    // Create honeycomb mesh
    NoximHexagon* root = NoximHexagon::buildHexagonTree(hmSize);

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
            // tx
            tile->req_tx[DIR_X](req_to_px[x + 1][y][z]);
            tile->flit_tx[DIR_X](flit_to_px[x + 1][y][z]);
            tile->ack_tx[DIR_X](ack_to_mx[x + 1][y][z]);
            tile->free_slots[DIR_X](free_slots_to_px[x + 1][y][z]);
            printNeighborSignals(DIRECTION_PY, tile);
            // rx
            tile->req_rx[DIR_Y](req_to_my[x][y][z]);
            tile->flit_rx[DIR_Y](flit_to_my[x][y][z]);
            tile->ack_rx[DIR_Y](ack_to_py[x][y][z]);
            tile->free_slots_neighbor[DIR_Y](free_slots_to_my[x][y][z]);
            // tx
            tile->req_tx[DIR_Y](req_to_py[x][y + 1][z]);
            tile->flit_tx[DIR_Y](flit_to_py[x][y + 1][z]);
            tile->ack_tx[DIR_Y](ack_to_my[x][y + 1][z]);
            tile->free_slots[DIR_Y](free_slots_to_py[x][y + 1][z]);
            printNeighborSignals(DIRECTION_PZ, tile);
            // rx
            tile->req_rx[DIR_Z](req_to_mz[x][y][z]);
            tile->flit_rx[DIR_Z](flit_to_mz[x][y][z]);
            tile->ack_rx[DIR_Z](ack_to_pz[x][y][z]);
            tile->free_slots_neighbor[DIR_Z](free_slots_to_mz[x][y][z]);
            // tx
            tile->req_tx[DIR_Z](req_to_pz[x][y][z + 1]);
            tile->flit_tx[DIR_Z](flit_to_pz[x][y][z + 1]);
            tile->ack_tx[DIR_Z](ack_to_mz[x][y][z + 1]);
            tile->free_slots[DIR_Z](free_slots_to_pz[x][y][z + 1]);
        } else if (tile->getType() == NoximHMTile::NEGATIVE) {
            printNeighborSignals(DIRECTION_MX, tile);
            //rx
            tile->req_rx[DIR_X](req_to_px[x][y][z]);
            tile->flit_rx[DIR_X](flit_to_px[x][y][z]);
            tile->ack_rx[DIR_X](ack_to_mx[x][y][z]);
            tile->free_slots_neighbor[DIR_X](free_slots_to_px[x][y][z]);
            //tx
            tile->req_tx[DIR_X](req_to_mx[x - 1][y][z]);
            tile->flit_tx[DIR_X](flit_to_mx[x - 1][y][z]);
            tile->ack_tx[DIR_X](ack_to_px[x - 1][y][z]);
            tile->free_slots[DIR_X](free_slots_to_mx[x - 1][y][z]);
            printNeighborSignals(DIRECTION_MY, tile);
            //rx
            tile->req_rx[DIR_Y](req_to_py[x][y][z]);
            tile->flit_rx[DIR_Y](flit_to_py[x][y][z]);
            tile->ack_rx[DIR_Y](ack_to_my[x][y][z]);
            tile->free_slots_neighbor[DIR_Y](free_slots_to_py[x][y][z]);
            //tx
            tile->req_tx[DIR_Y](req_to_my[x][y - 1][z]);
            tile->flit_tx[DIR_Y](flit_to_my[x][y - 1][z]);
            tile->ack_tx[DIR_Y](ack_to_py[x][y - 1][z]);
            tile->free_slots[DIR_Y](free_slots_to_my[x][y - 1][z]);
            printNeighborSignals(DIRECTION_MZ, tile);
            //rx
            tile->req_rx[DIR_Z](req_to_pz[x][y][z]);
            tile->flit_rx[DIR_Z](flit_to_pz[x][y][z]);
            tile->ack_rx[DIR_Z](ack_to_mz[x][y][z]);
            tile->free_slots_neighbor[DIR_Z](free_slots_to_pz[x][y][z]);
            //tx
            tile->req_tx[DIR_Z](req_to_mz[x][y][z - 1]);
            tile->flit_tx[DIR_Z](flit_to_mz[x][y][z - 1]);
            tile->ack_tx[DIR_Z](ack_to_pz[x][y][z - 1]);
            tile->free_slots[DIR_Z](free_slots_to_mz[x][y][z - 1]);
        }
    }
}
