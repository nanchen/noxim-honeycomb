#include "NoximHMNoCTest.h"
#include "NoximHexagon.h"

#include "systemc.h"


void NoximHMNoCTest::run(){
    testBuildHoneycombMesh();
}

void NoximHMNoCTest::testBuildHoneycombMesh(){
/*
    sc_signal<bool>   enable;
    cout << "enable " << enable.read() << endl;
    cout << "@" << sc_time_stamp() <<" Asserting reset\n" << endl;
    enable.write(1);
    enable =1;
    sc_start();
    cout << "enable " << enable.read() << endl;
*/
    NoximHMNoC *n = new NoximHMNoC("NoC");
    cout << "Test: NoximHMNoc intialized" << endl;
    // -y (dir px) = mz
    NoximHMTile* py = NoximHexagon::getTile(0,1,0);
    test_(py!=NULL);
    NoximHMTile* mz = NoximHexagon::getTile(1,1,0);
    test_(mz!=NULL);

/*
    cout << "py->req_tx[DIRECTION_PX] = " << py->req_tx[DIRECTION_PX].read() << endl;
    cout << "mz->req_rx[DIRECTION_MX] = " << mz->req_rx[DIRECTION_MX].read() << endl;

    cout << "n->req_to_px[1][1][0] = " << n->req_to_px[1][1][0] << endl;
    n->req_to_px[1][1][0]=1;
    sc_start();
    cout << "n->req_to_px[1][1][0] = " << n->req_to_px[1][1][0] << endl;

    cout << "py->req_tx[DIRECTION_PX] = " << py->req_tx[DIRECTION_PX].read() << endl;
    cout << "mz->req_rx[DIRECTION_MX] = " << mz->req_rx[DIRECTION_MX].read() << endl;
*/

  //  test_((&(py->req_tx[DIRECTION_PX])) == (&(mz->req_rx[DIRECTION_MX])));
}
