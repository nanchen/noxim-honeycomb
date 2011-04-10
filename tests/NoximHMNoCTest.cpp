#include "NoximHMNoCTest.h"
#include "NoximHexagon.h"


void NoximHMNoCTest::run(){
    testBuildHoneycombMesh();
}

void NoximHMNoCTest::testBuildHoneycombMesh(){
    NoximHMNoC *n = new NoximHMNoC("NoC");
    // -y (dir px) = mz
    NoximHMTile* py = NoximHexagon::getTile(0,1,0);
    test_(py!=NULL);
    NoximHMTile* mz = NoximHexagon::getTile(1,1,0);
    test_(mz!=NULL);
    cout << "py->req_tx[DIRECTION_PX] = " << &py->req_tx[DIRECTION_PX] << endl;
    cout << "mz->req_rx[DIRECTION_MZ] = " << &mz->req_rx[DIRECTION_MZ] << endl;
    test_(py->req_tx[DIRECTION_PX] == mz->req_rx[DIRECTION_MZ]);
}
