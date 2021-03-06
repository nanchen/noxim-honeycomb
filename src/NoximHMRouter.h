/*
 * Noxim - the NoC Simulator
 *
 * (C) 2005-2010 by the University of Catania
 * For the complete list of authors refer to file ../doc/AUTHORS.txt
 * For the license applied to these sources refer to file ../doc/LICENSE.txt
 *
 * This file contains the declaration of the router
 */

#ifndef __NOXIMHMROUTER_H__
#define __NOXIMHMROUTER_H__

#include <systemc.h>
#include "NoximMain.h"
#include "NoximBuffer.h"
#include "NoximStats.h"
#include "NoximGlobalRoutingTable.h"
#include "NoximLocalRoutingTable.h"
#include "NoximReservationTable.h"

using namespace std;

extern unsigned int drained_volume;

SC_MODULE(NoximHMRouter) {

    // I/O Ports
    sc_in_clk clock; // The input clock for the router
    sc_in<bool> reset; // The reset signal for the router

    sc_in<NoximFlit> flit_rx[DIRS + 1]; // The input channels (including local one)
    sc_in<bool> req_rx[DIRS + 1]; // The requests associated with the input channels
    sc_out<bool> ack_rx[DIRS + 1]; // The outgoing ack signals associated with the input channels

    sc_out<NoximFlit> flit_tx[DIRS + 1]; // The output channels (including local one)
    sc_out<bool> req_tx[DIRS + 1]; // The requests associated with the output channels
    sc_in<bool> ack_tx[DIRS + 1]; // The outgoing ack signals associated with the output channels

    sc_out<int> free_slots[DIRS + 1];
    sc_in<int> free_slots_neighbor[DIRS + 1];

    int local_id; // Unique ID
    int routing_type; // Type of routing algorithm
    int selection_type;
    NoximBuffer buffer[DIRS + 1]; // Buffer for each input channel
    bool current_level_rx[DIRS + 1]; // Current level for Alternating Bit Protocol (ABP)
    bool current_level_tx[DIRS + 1]; // Current level for Alternating Bit Protocol (ABP)
    NoximStats stats; // Statistics
    NoximLocalRoutingTable routing_table; // Routing table
    NoximReservationTable reservation_table; // Switch reservation table
    int start_from_port; // Port from which to start the reservation cycle
    unsigned long routed_flits;

    // Functions
    void rxProcess(); // The receiving process
    void txProcess(); // The transmitting process
    void bufferMonitor();
    void configure(const int _id, const double _warm_up_time,
            const unsigned int _max_buffer_size, NoximGlobalRoutingTable & grt);

    unsigned long getRoutedFlits(); // Returns the number of routed flits
    unsigned int getFlitsCount(); // Returns the number of flits into the router
    double getPower(); // Returns the total power dissipated by the router

    // Constructor

    SC_CTOR(NoximHMRouter) {
        SC_METHOD(rxProcess);
        sensitive << reset;
        sensitive << clock.pos();

        SC_METHOD(txProcess);
        sensitive << reset;
        sensitive << clock.pos();

        SC_METHOD(bufferMonitor);
        sensitive << reset;
        sensitive << clock.pos();
    }

    public:
    // performs actual routing + selection
    int route(const NoximRouteData & route_data);

    vector<int> routingFunction(const NoximRouteData & route_data);

    static vector<int> estimateRoutingMXPZFirst(const NoximHMCoord & current,
            const NoximHMCoord & destination);

    static vector<int> estimateRouting(const int srcId, const int dstId);

    vector<int> routingMXPZFirst(const NoximHMCoord & current,
            const NoximHMCoord & destination);

    vector<int> routingMin(const NoximHMCoord & current,
            const NoximHMCoord & destination);

    string toString() const;

    NoximHMCoord getCoord() const {
        return coord;
    }

    void setCoord(NoximHMCoord c) {
        coord = c;
    }

    inline ostream & operator <<(ostream & os) {
        os << toString();
        return os;
    }

    private:

    // wrappers
    int selectionFunction(const vector<int> &directions,
            const NoximRouteData & route_data);

    // selection strategies
    int selectionRandom(const vector<int> & directions);
    public:

    unsigned int local_drained;

    private:
    NoximHMCoord coord;

};

#endif
