/*
 * Noxim - the NoC Simulator
 *
 * (C) 2005-2010 by the University of Catania
 * For the complete list of authors refer to file ../doc/AUTHORS.txt
 * For the license applied to these sources refer to file ../doc/LICENSE.txt
 *
 * This file contains the implementaton of the global statistics
 */

#include "NoximHMGlobalStats.h"
using namespace std;

NoximHMGlobalStats::NoximHMGlobalStats(const NoximHMNoC * _noc) {
    noc = _noc;

#ifdef TESTING
    drained_total = 0;
#endif
}

double NoximHMGlobalStats::getAverageDelay() {
    unsigned int total_packets = 0;
    double avg_delay = 0.0;

    for (int i = 0; i <= NoximHexagon::getLatestId(); i++) {
        unsigned int received_packets =
                NoximHexagon::getTile(i)->r->stats.getReceivedPackets();

        if (received_packets) {
            avg_delay += received_packets
                    * NoximHexagon::getTile(i)->r->stats.getAverageDelay();
            total_packets += received_packets;
        }
    }
    avg_delay /= (double) total_packets;
    return avg_delay;
}

double NoximHMGlobalStats::getAverageDelay(const int src_id, const int dst_id) {
    NoximHMTile* tile = NoximHexagon::getTile(dst_id);
    assert(tile != NULL);
    return tile->r->stats.getAverageDelay(src_id);
}

double NoximHMGlobalStats::getMaxDelay() {
    double maxd = -1.0;
    for (int i = 0; i <= NoximHexagon::getLatestId(); i++) {
        double d = getMaxDelay(i);
        if (d > maxd)
            maxd = d;
    }
    return maxd;
}

double NoximHMGlobalStats::getMaxDelay(const int node_id) {
    unsigned int receivedPackets =
            NoximHexagon::getTile(node_id)->r->stats.getReceivedPackets();
    if (receivedPackets)
        return NoximHexagon::getTile(node_id)->r->stats.getMaxDelay();
    else
        return -1.0;
}

vector<vector<double> > NoximHMGlobalStats::getMaxDelayMtx() {
    vector<vector<double> > mtx;
    return mtx;
}

double NoximHMGlobalStats::getAverageThroughput(const int src_id,
        const int dst_id) {
    NoximHMTile* tile = NoximHexagon::getTile(dst_id);
    assert(tile != NULL);
    return tile->r->stats.getAverageThroughput(src_id);
}

double NoximHMGlobalStats::getAverageThroughput() {
    unsigned int total_comms = 0;
    double avg_throughput = 0.0;

    for (int i = 0; i <= NoximHexagon::getLatestId(); i++) {
        unsigned int ncomms =
                NoximHexagon::getTile(i)->r->stats.getTotalCommunications();

        if (ncomms) {
            avg_throughput += ncomms
                    * NoximHexagon::getTile(i)->r->stats.getAverageThroughput();
            total_comms += ncomms;
        }
    }
    avg_throughput /= (double) total_comms;
    return avg_throughput;
}

unsigned int NoximHMGlobalStats::getReceivedPackets() {
    unsigned int n = 0;
    for (int i = 0; i <= NoximHexagon::getLatestId(); i++) {
        //		cout << "getReceivedPackets   " << NoximHexagon::getTile(i)->r->stats.getReceivedPackets() << endl;
        n += NoximHexagon::getTile(i)->r->stats.getReceivedPackets();
    }
    return n;
}

unsigned int NoximHMGlobalStats::getReceivedFlits() {
    unsigned int n = 0;
    for (int i = 0; i <= NoximHexagon::getLatestId(); i++) {
        n += NoximHexagon::getTile(i)->r->stats.getReceivedFlits();
#ifdef TESTING
        drained_total += NoximHexagon::getTile(i)->r->local_drained;
#endif
    }
    return n;
}

double NoximHMGlobalStats::getThroughput() {
    int total_cycles = NoximGlobalParams::simulation_time
            - NoximGlobalParams::stats_warm_up_time;

    //  int number_of_ip = NoximGlobalParams::mesh_dim_x * NoximGlobalParams::mesh_dim_y;
    //  return (double)getReceivedFlits()/(double)(total_cycles * number_of_ip);

    unsigned int n = 0;
    unsigned int trf = 0;
    for (int i = 0; i <= NoximHexagon::getLatestId(); i++) {
        unsigned int rf = NoximHexagon::getTile(i)->r->stats.getReceivedFlits();

        if (rf != 0)
            n++;

        trf += rf;
    }
    return (double) trf / (double) (total_cycles * n);
    return 0.0;

}

vector<vector<unsigned long> > NoximHMGlobalStats::getRoutedFlitsMtx() {
    vector<vector<unsigned long> > mtx;
    return mtx;
}

double NoximHMGlobalStats::getPower() {
    double power = 0.0;
    for (int i = 0; i <= NoximHexagon::getLatestId(); i++) {
        power += NoximHexagon::getTile(i)->r->getPower();
    }
    return power;
}

void NoximHMGlobalStats::showStats(std::ostream & out, bool detailed) {
    out << "% Total received packets: " << getReceivedPackets() << endl;
    out << "% Total received flits: " << getReceivedFlits() << endl;
    out << "% Global average delay (cycles): " << getAverageDelay() << endl;
    out << "% Global average throughput (flits/cycle): "
            << getAverageThroughput() << endl;
    out << "% Throughput (flits/cycle/IP): " << getThroughput() << endl;
    out << "% Max delay (cycles): " << getMaxDelay() << endl;
    out << "% Total energy (J): " << getPower() << endl;

    if (detailed) {
        out << endl << "detailed = [" << endl;
        for (int i = 0; i <= NoximHexagon::getLatestId(); i++) {
            NoximHexagon::getTile(i)->r->stats.showStats(i, out, true);
        }
        out << "];" << endl;
    }
}
//------------------------Calculated statistics----------------------------

struct Hops {
    double average;
    int max;
    int maxSrcId;
    int maxDstId;
    int min;
    int minSrcId;
    int minDstId;
    int pathCount;
};

struct Hops calcHopsStats() {
    unsigned int count = 0;
    int hopsSum = 0;
    struct Hops hs;
    hs.max = -1;
    hs.min = 999999;
    for (int i = 0; i <= NoximHexagon::getLatestId(); i++) {
        for (int j = 0; j <= NoximHexagon::getLatestId(); j++) {
            if (i != j) {
                vector<int> dirs = NoximHMRouter::estimateRouting(i, j);
                int hops = dirs.size();
                hopsSum += hops;
                count++;
                if (hops > hs.max) {
                    hs.max = hops;
                    hs.maxSrcId = i;
                    hs.maxDstId = j;
                }
                if (hops < hs.min) {
                    hs.min = hops;
                    hs.minSrcId = i;
                    hs.minDstId = j;
                }
            }
        }
    }
    hs.average = (double) hopsSum / count;
    hs.pathCount = count;
    return hs;
}

void printVector(vector<int> v) {
    cout << "vector: ";
    for (int i = 0; i < v.size(); i++)
        cout << v[i] << ",";
    cout << endl;
}

void NoximHMGlobalStats::showCalcStats(std::ostream & out) {
    struct Hops hs = calcHopsStats();
    out << "Calculated for " << hs.pathCount << " paths" << endl;
    out << "\t Average hops = " << hs.average << endl;
    //max
    out << "\t Max hops = " << hs.max << endl;
    const vector<int> maxDirs = NoximHMRouter::estimateRouting(hs.maxSrcId,
            hs.maxDstId);
    out << "\t\t e.g.: src = " << NoximHexagon::id2Coord(hs.maxSrcId)
            << ", dst = " << NoximHexagon::id2Coord(hs.maxDstId) << endl;
    out << "\t\t Routing result direction ";
    printVector(maxDirs);
    //min
    out << "\t Min hops = " << hs.min << endl;
    const vector<int> minDirs = NoximHMRouter::estimateRouting(hs.minSrcId,
            hs.minDstId);
    out << "\t\t e.g.: src = " << NoximHexagon::id2Coord(hs.minSrcId)
            << ", dst = " << NoximHexagon::id2Coord(hs.minDstId) << endl;
    out << "\t\t Routing result direction ";
    printVector(minDirs);
}
//------------------------Calculated statistics----------------------------
