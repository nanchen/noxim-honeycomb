#include "NoximHMRouterTest.h"
#include "NoximMain.h"
#include <vector>

void NoximHMRouterTest::run()
{
    testRoutingMinusXPlusZFirst();
}

void NoximHMRouterTest::testRoutingMinusXPlusZFirst()
{
    NoximHMRouter* router = new NoximHMRouter("router");

    //----------start from +x--------------
    //+x -> -z
    vector<int> ret = router->routingMinusXPlusZFirst(NoximHMCoord(1,0,0),NoximHMCoord(1,1,0));
    printVector(ret);
    test_(ret.size()==1);
    test_(ret[0] == DIRECTION_PY);

    //+x -> py
    ret = router->routingMinusXPlusZFirst(NoximHMCoord(1,0,0),NoximHMCoord(0,1,0));
    printVector(ret);
    test_(ret.size()==4);
    test_(ret[0] == DIRECTION_PZ);
    test_(ret[1] == DIRECTION_MX);
    test_(ret[2] == DIRECTION_PY);
    test_(ret[3] == DIRECTION_MZ);

    //+x -> -x
    ret = router->routingMinusXPlusZFirst(NoximHMCoord(1,0,0),NoximHMCoord(0,1,1));
    printVector(ret);
    test_(ret.size()==3);
    test_(ret[0] == DIRECTION_PZ);
    test_(ret[1] == DIRECTION_MX);
    test_(ret[2] == DIRECTION_PY);

    //+x -> +z
    ret = router->routingMinusXPlusZFirst(NoximHMCoord(1,0,0),NoximHMCoord(0,0,1));
    printVector(ret);
    test_(ret.size()==2);
    test_(ret[0] == DIRECTION_PZ);
    test_(ret[1] == DIRECTION_MX);

    //+x -> -y
    ret = router->routingMinusXPlusZFirst(NoximHMCoord(1,0,0),NoximHMCoord(1,0,1));
    printVector(ret);
    test_(ret.size()==1);
    test_(ret[0] == DIRECTION_PZ);

    //-------------start from -z------------
    //-z -> +y
    ret = router->routingMinusXPlusZFirst(NoximHMCoord(1,1,0),NoximHMCoord(0,1,0));
    printVector(ret);
    test_(ret.size()==1);
    test_(ret[0] == DIRECTION_MX);

    //-z -> -x
    ret = router->routingMinusXPlusZFirst(NoximHMCoord(1,1,0),NoximHMCoord(0,1,1));
    printVector(ret);
    test_(ret.size()==2);
    test_(ret[0] == DIRECTION_MX);
    test_(ret[1] == DIRECTION_PZ);

    //-z -> +z
    ret = router->routingMinusXPlusZFirst(NoximHMCoord(1,1,0),NoximHMCoord(0,0,1));
    printVector(ret);
    test_(ret.size()==3);
    test_(ret[0] == DIRECTION_MX);
    test_(ret[1] == DIRECTION_PZ);
    test_(ret[2] == DIRECTION_MY);

    //-z -> -y
    ret = router->routingMinusXPlusZFirst(NoximHMCoord(1,1,0),NoximHMCoord(1,0,1));
    printVector(ret);
    test_(ret.size()==4);
    test_(ret[0] == DIRECTION_MX);
    test_(ret[1] == DIRECTION_PZ);
    test_(ret[2] == DIRECTION_MY);
    test_(ret[3] == DIRECTION_PX);

    //-z -> +x
    ret = router->routingMinusXPlusZFirst(NoximHMCoord(1,1,0),NoximHMCoord(1,0,0));
    printVector(ret);
    test_(ret.size()==1);
    test_(ret[0] == DIRECTION_MY);

    //-------start from +y-------------
    // -> -x
    ret = router->routingMinusXPlusZFirst(NoximHMCoord(0,1,0),NoximHMCoord(0,1,1));
    printVector(ret);
    test_(ret.size()==1);
    test_(ret[0] == DIRECTION_PZ);

    // -> +z
    ret = router->routingMinusXPlusZFirst(NoximHMCoord(0,1,0),NoximHMCoord(0,0,1));
    printVector(ret);
    test_(ret.size()==2);
    test_(ret[0] == DIRECTION_PZ);
    test_(ret[1] == DIRECTION_MY);

    // -> -y
    ret = router->routingMinusXPlusZFirst(NoximHMCoord(0,1,0),NoximHMCoord(1,0,1));
    printVector(ret);
    test_(ret.size()==3);
    test_(ret[0] == DIRECTION_PZ);
    test_(ret[1] == DIRECTION_MY);
    test_(ret[2] == DIRECTION_PX);

    // -> +x
    ret = router->routingMinusXPlusZFirst(NoximHMCoord(0,1,0),NoximHMCoord(1,0,0));
    printVector(ret);
    test_(ret.size()==2);
    test_(ret[0] == DIRECTION_PX);
    test_(ret[1] == DIRECTION_MY);

    // -> -z
    ret = router->routingMinusXPlusZFirst(NoximHMCoord(0,1,0),NoximHMCoord(1,1,0));
    printVector(ret);
    test_(ret.size()==1);
    test_(ret[0] == DIRECTION_PX);

    //-------------start from -x------------
    // >> +z
    ret = router->routingMinusXPlusZFirst(NoximHMCoord(0,1,1),NoximHMCoord(0,0,1));
    printVector(ret);
    test_(ret.size()==1);
    test_(ret[0] == DIRECTION_MY);

    // >> -y
    ret = router->routingMinusXPlusZFirst(NoximHMCoord(0,1,1),NoximHMCoord(1,0,1));
    printVector(ret);
    test_(ret.size()==2);
    test_(ret[0] == DIRECTION_MY);
    test_(ret[1] == DIRECTION_PX);

    // >> +x
    ret = router->routingMinusXPlusZFirst(NoximHMCoord(0,1,1),NoximHMCoord(1,0,0));
    printVector(ret);
    test_(ret.size()==3);
    test_(ret[0] == DIRECTION_MY);
    test_(ret[1] == DIRECTION_PX);
    test_(ret[2] == DIRECTION_MZ);

    // >> -z
    ret = router->routingMinusXPlusZFirst(NoximHMCoord(0,1,1),NoximHMCoord(1,1,0));
    printVector(ret);
    test_(ret.size()==2);
    test_(ret[0] == DIRECTION_MZ);
    test_(ret[1] == DIRECTION_PX);

    // >> -y
    ret = router->routingMinusXPlusZFirst(NoximHMCoord(0,1,1),NoximHMCoord(0,1,0));
    printVector(ret);
    test_(ret.size()==1);
    test_(ret[0] == DIRECTION_MZ);

    // -------start from +z--------------
    // >> -x
    ret = router->routingMinusXPlusZFirst(NoximHMCoord(0,0,1),NoximHMCoord(1,0,1));
    printVector(ret);
    test_(ret.size()==1);
    test_(ret[0] == DIRECTION_PX);

    // >> +x
    ret = router->routingMinusXPlusZFirst(NoximHMCoord(0,0,1),NoximHMCoord(1,0,0));
    printVector(ret);
    test_(ret.size()==2);
    test_(ret[0] == DIRECTION_PX);
    test_(ret[1] == DIRECTION_MZ);

    // >> -z
    ret = router->routingMinusXPlusZFirst(NoximHMCoord(0,0,1),NoximHMCoord(1,1,0));
    printVector(ret);
    test_(ret.size()==3);
    test_(ret[0] == DIRECTION_PY);
    test_(ret[1] == DIRECTION_MZ);
    test_(ret[2] == DIRECTION_PX);

    // >> +y
    ret = router->routingMinusXPlusZFirst(NoximHMCoord(0,0,1),NoximHMCoord(0,1,0));
    printVector(ret);
    test_(ret.size()==2);
    test_(ret[0] == DIRECTION_PY);
    test_(ret[1] == DIRECTION_MZ);

    // >> -x
    ret = router->routingMinusXPlusZFirst(NoximHMCoord(0,0,1),NoximHMCoord(0,1,1));
    printVector(ret);
    test_(ret.size()==1);
    test_(ret[0] == DIRECTION_PY);

    //-------start from -y------------
    // >> +x
    ret = router->routingMinusXPlusZFirst(NoximHMCoord(1,0,1),NoximHMCoord(1,0,0));
    printVector(ret);
    test_(ret.size()==1);
    test_(ret[0] == DIRECTION_MZ);

    // >> -z
    ret = router->routingMinusXPlusZFirst(NoximHMCoord(1,0,1),NoximHMCoord(1,1,0));
    printVector(ret);
    test_(ret.size()==2);
    test_(ret[0] == DIRECTION_MZ);
    test_(ret[1] == DIRECTION_PY);

    // >> +y
    ret = router->routingMinusXPlusZFirst(NoximHMCoord(1,0,1),NoximHMCoord(0,1,0));
    printVector(ret);
    test_(ret.size()==3);
    test_(ret[0] == DIRECTION_MX);
    test_(ret[1] == DIRECTION_PY);
    test_(ret[2] == DIRECTION_MZ);

    // >> -x
    ret = router->routingMinusXPlusZFirst(NoximHMCoord(1,0,1),NoximHMCoord(0,1,1));
    printVector(ret);
    test_(ret.size()==2);
    test_(ret[0] == DIRECTION_MX);
    test_(ret[1] == DIRECTION_PY);

    // >> +z
    ret = router->routingMinusXPlusZFirst(NoximHMCoord(1,0,1),NoximHMCoord(0,0,1));
    printVector(ret);
    test_(ret.size()==1);
    test_(ret[0] == DIRECTION_MX);

    //arbitary
    ret = router->routingMinusXPlusZFirst(NoximHMCoord(3,0,-2),NoximHMCoord(-3,1,4));
    printVector(ret);
    test_(ret.size()==13);
    test_(ret[0] == DIRECTION_PZ);
    test_(ret[1] == DIRECTION_MX);
    test_(ret[2] == DIRECTION_PZ);
    test_(ret[3] == DIRECTION_MX);
    test_(ret[4] == DIRECTION_PZ);
    test_(ret[5] == DIRECTION_MX);
    test_(ret[6] == DIRECTION_PZ);
    test_(ret[7] == DIRECTION_MX);
    test_(ret[8] == DIRECTION_PZ);
    test_(ret[9] == DIRECTION_MX);
    test_(ret[10] == DIRECTION_PZ);
    test_(ret[11] == DIRECTION_MX);
    test_(ret[12] == DIRECTION_PY);
}

void NoximHMRouterTest::printVector(const vector<int>& v){
    cout << "vector: ";
    for(int i = 0;i<v.size();i++)
        cout << v[i] << ",";
    cout << endl;
}

