/*
 * Noxim - the NoC Simulator
 *
 * (C) 2005-2010 by the University of Catania
 * For the complete list of authors refer to file ../doc/AUTHORS.txt
 * For the license applied to these sources refer to file ../doc/LICENSE.txt
 *
 * This file contains the implementation of the router
 */

#include <stdio.h>
#include "NoximHMRouter.h"
#include "NoximHexagon.h"

void NoximHMRouter::rxProcess() {
    if (reset.read()) {
        // Clear outputs and indexes of receiving protocol
        for (int i = 0; i < DIRS + 1; i++) {
            ack_rx[i].write(0);
            current_level_rx[i] = 0;
        }
        reservation_table.clear();
        routed_flits = 0;
        local_drained = 0;
    } else {
        // For each channel decide if a new flit can be accepted
        //
        // This process simply sees a flow of incoming flits. All arbitration
        // and wormhole related issues are addressed in the txProcess()

        for (int i = 0; i < DIRS + 1; i++) {
            // To accept a new flit, the following conditions must match:
            //
            // 1) there is an incoming request
            // 2) there is a free slot in the input buffer of direction i

            if (req_rx[i].read() == 1 - current_level_rx[i]) {
                if (NoximGlobalParams::verbose_mode > VERBOSE_OFF) {
                    cout << sc_time_stamp().to_double() / 1000 << ": "
                            << this->toString() << ", RX request[" << i << "]";
                }

                if (!buffer[i].IsFull()) {
                    NoximFlit received_flit = flit_rx[i].read();

                    if (NoximGlobalParams::verbose_mode > VERBOSE_OFF) {
                        cout << ", Input[" << i << "], Received flit: "
                                << received_flit << endl;
                    }
                    // Store the incoming flit in the circular buffer
                    buffer[i].Push(received_flit);

                    // Negate the old value for Alternating Bit Protocol (ABP)
                    current_level_rx[i] = 1 - current_level_rx[i];

                    // Incoming flit
                    stats.power.Incoming();
                } else if (NoximGlobalParams::verbose_mode > VERBOSE_OFF)
                    cout << "\n\tBuffer[" << i << "] is full" << endl;

            }
            ack_rx[i].write(current_level_rx[i]);
        }
    }
    stats.power.Standby();
}

void NoximHMRouter::txProcess() {
    if (reset.read()) {
        // Clear outputs and indexes of transmitting protocol
        for (int i = 0; i < DIRS + 1; i++) {
            req_tx[i].write(0);
            current_level_tx[i] = 0;
        }
    } else {
        // 1st phase: Reservation
        for (int j = 0; j < DIRS + 1; j++) {
            int i = (start_from_port + j) % (DIRS + 1);

            if (!buffer[i].IsEmpty()) {
                NoximFlit flit = buffer[i].Front();

                if (flit.flit_type == FLIT_TYPE_HEAD) {
                    // prepare data for routing
                    NoximRouteData route_data;
                    route_data.current_id = local_id;
                    route_data.src_id = flit.src_id;
                    route_data.dst_id = flit.dst_id;
                    route_data.dir_in = i;

                    int fullOutDir = route(route_data);
                    int o = NoximHexagon::fullDir2ReducedDir(fullOutDir);

                    if (NoximGlobalParams::verbose_mode > VERBOSE_OFF) {
                        cout << sc_time_stamp().to_double() / 1000 << ": "
                                << this->toString() << ", Input[" << i << "] ("
                                << buffer[i]. Size() << " flits)";
                    }
                    if (reservation_table.isAvailable(o)) {
                        reservation_table.reserve(i, o);
                        if (NoximGlobalParams::verbose_mode > VERBOSE_OFF) {
                            cout << ", reserved Output[" << o << "], flit: "
                                    << flit << endl;
                        }
                    } else if (NoximGlobalParams::verbose_mode > VERBOSE_OFF)
                        cout << "\n\tOutput[" << o << "] is not available"
                            << endl;
                }
            }
        }
        start_from_port++;

        // 2nd phase: Forwarding
        for (int i = 0; i < DIRS + 1; i++) {
            if (!buffer[i].IsEmpty()) {
                NoximFlit flit = buffer[i].Front();

                int o = reservation_table.getOutputPort(i);
                if (NoximGlobalParams::verbose_mode > VERBOSE_OFF) {
                    cout << sc_time_stamp().to_double() / 1000 << ": "
                            << this->toString() << ", Input[" << i
                            << "] forwarding.. ";
                }
                if (o != NOT_RESERVED) {
                    if (current_level_tx[o] == ack_tx[o].read()) {
                        if (NoximGlobalParams::verbose_mode > VERBOSE_OFF)
                            cout << ", forward to Output[" << o << "], flit: "
                                << flit << endl;

                        flit_tx[o].write(flit);
                        current_level_tx[o] = 1 - current_level_tx[o];
                        req_tx[o].write(current_level_tx[o]);
                        buffer[i].Pop();

                        stats.power.Forward();

                        if (flit.flit_type == FLIT_TYPE_TAIL)
                            reservation_table.release(o);

                        // Update stats
                        if (o == DIR_LOCAL) {
                            stats.receivedFlit(
                                    sc_time_stamp(). to_double() / 1000, flit);
                            if (NoximGlobalParams::max_volume_to_be_drained) {
                                if (drained_volume
                                        >= NoximGlobalParams::max_volume_to_be_drained)
                                    sc_stop();
                                else {
                                    drained_volume++;
                                    local_drained++;
                                }
                            }
                        } else if (i != DIR_LOCAL) {
                            // Increment routed flits counter
                            routed_flits++;
                        }
                    }
                } else if (NoximGlobalParams::verbose_mode > VERBOSE_OFF)
                    cout << "\n\tOutput not reserved!" << endl;
            }
        }
    } // else
    stats.power.Standby();
}

void NoximHMRouter::bufferMonitor() {
    if (reset.read()) {
        for (int i = 0; i < DIRS + 1; i++)
            free_slots[i].write(buffer[i].GetMaxBufferSize());
    }
}

vector<int> NoximHMRouter::routingFunction(const NoximRouteData & route_data) {
    int dir_in = route_data.dir_in;
    NoximHMCoord hmPosition = NoximHexagon::id2Coord(route_data.current_id);
    NoximHMCoord hmDstCoord = NoximHexagon::id2Coord(route_data.dst_id);

    switch (NoximGlobalParams::routing_algorithm) {

        case ROUTING_MXPZ:
            return routingMXPZFirst(hmPosition, hmDstCoord);
        case ROUTING_MIN:
            return routingMin(hmPosition, hmDstCoord);
        default:
            assert(false);
    }

    // something weird happened, you shouldn't be here
    return (vector<int>) (0);
}

int NoximHMRouter::route(const NoximRouteData & route_data) {
    stats.power.Routing();

    if (route_data.dst_id == local_id)
        return DIRECTION_HM_LOCAL;

    vector<int> candidate_channels = routingFunction(route_data);
    return selectionFunction(candidate_channels, route_data);
}

int NoximHMRouter::selectionRandom(const vector<int>&directions) {
    return directions[rand() % directions.size()];
}

int NoximHMRouter::selectionFunction(const vector<int>&directions,
        const NoximRouteData & route_data) {
    // not so elegant but fast escape ;)
    if (directions.size() == 1)
        return directions[0];

    stats.power.Selection();
    switch (NoximGlobalParams::selection_strategy) {
        case SEL_RANDOM:
            return selectionRandom(directions);
        default:
            assert(false);
    }

    return 0;
}

vector<int> NoximHMRouter::routingMXPZFirst(const NoximHMCoord & current,
        const NoximHMCoord & destination) {
    vector<int> directions;
    const int cx = current.x;
    const int cy = current.y;
    const int cz = current.z;
    const int dx = destination.x;
    const int dy = destination.y;
    const int dz = destination.z;

    const NoximHMTile* t = NoximHexagon::getTile(cx, cy, cz);
    const NoximHMTile* dstTile = NoximHexagon::getTile(dx, dy, dz);
    if (dstTile == NULL) {
        cout << "Destination tile is NULL! return" << endl;
        assert(false);
        return directions;
    }
    // -x +z first phase
    if (t && t != dstTile && cx > dx || cz < dz) {
        if (t->nTile[DIRECTION_MX] != NULL) {
            directions.push_back(DIRECTION_MX);
        } else if (t->nTile[DIRECTION_PZ] != NULL) {
            directions.push_back(DIRECTION_PZ);
        } else if (t->nTile[DIRECTION_PY] != NULL && cy < dy) {
            directions.push_back(DIRECTION_PY);
        } else if (t->nTile[DIRECTION_MY] != NULL && cy > dy) {
            directions.push_back(DIRECTION_MY);
        }
        if (directions.size() > 0)
            return directions;
    }
    // minimal routing phase
    if (t && t != dstTile) {
        if (t->nTile[DIRECTION_PY] != NULL && cy < dy) {
            directions.push_back(DIRECTION_PY);
        }
        if (t->nTile[DIRECTION_MY] != NULL && cy > dy) {
            directions.push_back(DIRECTION_MY);
        }
        if (t->nTile[DIRECTION_PX] != NULL && cx < dx) {
            directions.push_back(DIRECTION_PX);
        }
        if (t->nTile[DIRECTION_MZ] != NULL && cz > dz) {
            directions.push_back(DIRECTION_MZ);
        }
    }
    return directions;
}

vector<int> NoximHMRouter::estimateRoutingMXPZFirst(
        const NoximHMCoord & current, const NoximHMCoord & destination) {
    vector<int> directions;
    // current -> dst include -x or +z hop
    // -x / +y direction possibilities in current

    int cx = current.x;
    int cy = current.y;
    int cz = current.z;
    const int dx = destination.x;
    const int dy = destination.y;
    const int dz = destination.z;

    NoximHMTile* t = NoximHexagon::getTile(cx, cy, cz);
    const NoximHMTile* dstTile = NoximHexagon::getTile(dx, dy, dz);
    if (dstTile == NULL) {
        cout << "Destination tile is NULL! return" << endl;
        return directions;
    }
    // -x +z first
    unsigned int count = 0;
    while (t && t != dstTile && t->getCoord().x > dx || t->getCoord().z < dz) {
        count++;
        if (count > 500) {
            cerr << "loop detected!" << endl;
            break;
        }

        if (t->nTile[DIRECTION_MX] != NULL) {
            directions.push_back(DIRECTION_MX);
            t = NoximHexagon::getNeighborTile(t->getCoord(), DIRECTION_MX);
            continue;
        }

        if (t->nTile[DIRECTION_PZ] != NULL) {
            directions.push_back(DIRECTION_PZ);
            t = NoximHexagon::getNeighborTile(t->getCoord(), DIRECTION_PZ);
            continue;
        }

        if (t->nTile[DIRECTION_PY] != NULL && t->getCoord().y < dy) {
            directions.push_back(DIRECTION_PY);
            t = NoximHexagon::getNeighborTile(t->getCoord(), DIRECTION_PY);
            continue;
        }

        if (t->nTile[DIRECTION_MY] != NULL && t->getCoord().y > dy) {
            directions.push_back(DIRECTION_MY);
            t = NoximHexagon::getNeighborTile(t->getCoord(), DIRECTION_MY);
            continue;
        }
    }
    if (t == NULL) {
        cout << "Current tile is NULL! return" << endl;
        return directions;
    }
    // minimal routing algorithm
    count = 0;
    while (t && t != dstTile) {
        if (count > 500) {
            cerr << "loop detected!" << endl;
            break;
        }
        if (t->nTile[DIRECTION_PY] != NULL && t->getCoord().y < dy) {
            directions.push_back(DIRECTION_PY);
            t = NoximHexagon::getNeighborTile(t->getCoord(), DIRECTION_PY);
            continue;
        }

        if (t->nTile[DIRECTION_MY] != NULL && t->getCoord().y > dy) {
            directions.push_back(DIRECTION_MY);
            t = NoximHexagon::getNeighborTile(t->getCoord(), DIRECTION_MY);
            continue;
        }

        if (t->nTile[DIRECTION_PX] != NULL && t->getCoord().x < dx) {
            directions.push_back(DIRECTION_PX);
            t = NoximHexagon::getNeighborTile(t->getCoord(), DIRECTION_PX);
            continue;
        }

        if (t->nTile[DIRECTION_MZ] != NULL && t->getCoord().z > dz) {
            directions.push_back(DIRECTION_MZ);
            t = NoximHexagon::getNeighborTile(t->getCoord(), DIRECTION_MZ);
            continue;
        }
    }
    return directions;
}

vector<int> NoximHMRouter::estimateRouting(const int srcId, const int dstId) {
    vector<int> dirs;
    NoximHMTile* curTile = NoximHexagon::getTile(srcId);
    while (curTile->getId() != dstId) {
        NoximHMRouter* curRouter = curTile->r;
        NoximRouteData rd;
        rd.current_id = curTile->getId();
        rd.src_id = srcId;
        rd.dst_id = dstId;
        int dir = curRouter->route(rd);
        dirs.push_back(dir);
        curTile = curTile->nTile[dir];
    }
    return dirs;
}

vector<int> NoximHMRouter::routingMin(const NoximHMCoord & current,
        const NoximHMCoord & destination) {
    vector<int> directions;
    const int cx = current.x;
    const int cy = current.y;
    const int cz = current.z;
    const int dx = destination.x;
    const int dy = destination.y;
    const int dz = destination.z;

    const NoximHMTile* t = NoximHexagon::getTile(cx, cy, cz);
    const NoximHMTile* dstTile = NoximHexagon::getTile(dx, dy, dz);
    if (dstTile == NULL) {
        cout << "Destination tile is NULL! return" << endl;
        assert(false);
    }

    if (t && t != dstTile) {
        if (t->nTile[DIRECTION_PX] != NULL && cx < dx)
            directions.push_back(DIRECTION_PX);
        if (t->nTile[DIRECTION_MX] != NULL && cx > dx)
            directions.push_back(DIRECTION_MX);
        if (t->nTile[DIRECTION_PY] != NULL && cy < dy)
            directions.push_back(DIRECTION_PY);
        if (t->nTile[DIRECTION_MY] != NULL && cy > dy)
            directions.push_back(DIRECTION_MY);
        if (t->nTile[DIRECTION_PZ] != NULL && cz < dz)
            directions.push_back(DIRECTION_PZ);
        if (t->nTile[DIRECTION_MZ] != NULL && cz > dz)
            directions.push_back(DIRECTION_MZ);
    }
    return directions;
}

void NoximHMRouter::configure(const int _id, const double _warm_up_time,
        const unsigned int _max_buffer_size, NoximGlobalRoutingTable & grt) {
    //	local_id = _id;
    stats.configure(_id, _warm_up_time);

    start_from_port = DIR_LOCAL;
    if (grt.isValid())
        routing_table.configure(grt, _id);

    for (int i = 0; i < DIRS + 1; i++)
        buffer[i].SetMaxBufferSize(_max_buffer_size);
}

unsigned long NoximHMRouter::getRoutedFlits() {
    return routed_flits;
}

unsigned int NoximHMRouter::getFlitsCount() {
    unsigned count = 0;

    for (int i = 0; i < DIRS + 1; i++)
        count += buffer[i].Size();

    return count;
}

double NoximHMRouter::getPower() {
    return stats.power.getPower();
}

string NoximHMRouter::toString() const {
    char* ret = (char*) malloc(50 * sizeof (char));
    sprintf(ret, " Router [%d] at %s", local_id, getCoord().toString());
    string strRet(ret);
    return strRet;
}
