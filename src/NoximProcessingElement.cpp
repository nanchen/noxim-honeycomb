/*
 * Noxim - the NoC Simulator
 *
 * (C) 2005-2010 by the University of Catania
 * For the complete list of authors refer to file ../doc/AUTHORS.txt
 * For the license applied to these sources refer to file ../doc/LICENSE.txt
 *
 * This file contains the implementation of the processing element
 */

#include "NoximProcessingElement.h"
#include "NoximHexagon.h"

int NoximProcessingElement::randInt(int min, int max) {
    return min + (int) ((double) (max - min + 1) * rand() / (RAND_MAX + 1.0));
}

void NoximProcessingElement::rxProcess() {
    if (reset.read()) {
        ack_rx.write(0);
        current_level_rx = 0;
    } else {
        if (req_rx.read() == 1 - current_level_rx) {
            NoximFlit flit_tmp = flit_rx.read();
            if (NoximGlobalParams::verbose_mode > VERBOSE_OFF) {
                cout << sc_simulation_time() << ": " << this->toString()
                        << " RECEIVING " << flit_tmp << endl;
            }
            current_level_rx = 1 - current_level_rx; // Negate the old value for Alternating Bit Protocol (ABP)
        }
        ack_rx.write(current_level_rx);
    }
}

void NoximProcessingElement::txProcess() {
    if (reset.read()) {
        req_tx.write(0);
        current_level_tx = 0;
        transmittedAtPreviousCycle = false;
    } else {
        NoximPacket packet;

        if (canShot(packet)) {
            packet_queue.push(packet);
            transmittedAtPreviousCycle = true;
        } else
            transmittedAtPreviousCycle = false;

        if (ack_tx.read() == current_level_tx) {
            if (!packet_queue.empty()) {
                NoximFlit flit = nextFlit(); // Generate a new flit
                if (NoximGlobalParams::verbose_mode > VERBOSE_OFF) {
                    cout << sc_time_stamp().to_double() / 1000 << ": "
                            << this->toString() << " SENDING " << flit << endl;
                }
                flit_tx->write(flit); // Send the generated flit
                current_level_tx = 1 - current_level_tx; // Negate the old value for Alternating Bit Protocol (ABP)
                req_tx.write(current_level_tx);
            }
        }
    }
}

NoximFlit NoximProcessingElement::nextFlit() {
    NoximFlit flit;
    NoximPacket packet = packet_queue.front();

    flit.src_id = packet.src_id;
    flit.dst_id = packet.dst_id;
    flit.timestamp = packet.timestamp;
    flit.sequence_no = packet.size - packet.flit_left;
    flit.hop_no = 0;
    //  flit.payload     = DEFAULT_PAYLOAD;

    if (packet.size == packet.flit_left)
        flit.flit_type = FLIT_TYPE_HEAD;
    else if (packet.flit_left == 1)
        flit.flit_type = FLIT_TYPE_TAIL;
    else
        flit.flit_type = FLIT_TYPE_BODY;

    packet_queue.front().flit_left--;
    if (packet_queue.front().flit_left == 0)
        packet_queue.pop();

    return flit;
}

bool NoximProcessingElement::canShot(NoximPacket & packet) {
    bool shot;
    double threshold;

    if (NoximGlobalParams::traffic_distribution != TRAFFIC_TABLE_BASED) {
        if (!transmittedAtPreviousCycle)
            threshold = NoximGlobalParams::packet_injection_rate;
        else
            threshold = NoximGlobalParams::probability_of_retransmission;

        shot = (((double) rand()) / RAND_MAX < threshold);
        if (shot) {
            switch (NoximGlobalParams::traffic_distribution) {
                case TRAFFIC_RANDOM:
                    packet = trafficRandom();
                    break;
                default:
                    assert(false);
            }
        }
    } else { // Table based communication traffic
        if (never_transmit)
            return false;

        double now = sc_time_stamp().to_double() / 1000;
        bool use_pir = (transmittedAtPreviousCycle == false);
        vector<pair<int, double> > dst_prob;
        double threshold = traffic_table->getCumulativePirPor(local_id,
                (int) now, use_pir, dst_prob);

        double prob = (double) rand() / RAND_MAX;
        shot = (prob < threshold);
        if (shot) {
            for (unsigned int i = 0; i < dst_prob.size(); i++) {
                if (prob < dst_prob[i].second) {
                    packet.make(local_id, dst_prob[i].first, now,
                            getRandomSize());
                    break;
                }
            }
        }

    }
    return shot;
}

NoximPacket NoximProcessingElement::trafficRandom() {
    NoximPacket p;
    p.src_id = local_id;
    double rnd = rand() / (double) RAND_MAX;
    double range_start = 0.0;

    //cout << "\n " << sc_time_stamp().to_double()/1000 << " PE " << local_id << " rnd = " << rnd << endl;
    const int max_id = NoximHexagon::getLatestId();
    // Random destination distribution
    do {
        p.dst_id = randInt(0, max_id);

        // check for hotspot destination
        for (uint i = 0; i < NoximGlobalParams::hotspots.size(); i++) {
            //cout << sc_time_stamp().to_double()/1000 << " PE " << local_id << " Checking node " << NoximGlobalParams::hotspots[i].first << " with P = " << NoximGlobalParams::hotspots[i].second << endl;

            if (rnd >= range_start && rnd < range_start
                    + NoximGlobalParams::hotspots[i].second) {
                if (local_id != NoximGlobalParams::hotspots[i].first) {
                    //cout << sc_time_stamp().to_double()/1000 << " PE " << local_id <<" That is ! " << endl;
                    p.dst_id = NoximGlobalParams::hotspots[i].first;
                }
                break;
            } else
                range_start += NoximGlobalParams::hotspots[i].second; // try next
        }
    } while (p.dst_id == p.src_id);

    p.timestamp = sc_time_stamp().to_double() / 1000;
    p.size = p.flit_left = getRandomSize();

    return p;
}

string NoximProcessingElement::toString() const {
    char* ret = (char*) malloc(50 * sizeof (char));
    sprintf(ret, "PE[%d] at %s", local_id, coord.toString());
    string strRet(ret);
    return strRet;
}

int NoximProcessingElement::getRandomSize() {
    return randInt(NoximGlobalParams::min_packet_size,
            NoximGlobalParams::max_packet_size);
}
