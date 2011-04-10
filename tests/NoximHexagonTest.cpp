#include "NoximHexagonTest.h"
#include "NoximHexagon.h"

void NoximHexagonTest::run(){
//    testConstructor();
//    testCreateNeighbor();
//    testBuildHexagonTree();
}

void NoximHexagonTest::testConstructor(){
    NoximHexagon* h = new NoximHexagon(1, "Root", NULL, 0, 0, 0);
    test_(h->getLevel() == 1);
    test_(h->getName() == "Root");
    test_(h->getCreator() == NULL);
    test_(h->getCoord().equals(0,0,0));

}

void NoximHexagonTest::testCreateNeighbor(){
    NoximHexagon* h = new NoximHexagon(1, "Root", NULL, 0, 0, 0);
    test_(h->n0 == NULL);
    test_(h->n1 == NULL);
    test_(h->n2 == NULL);
    test_(h->n3 == NULL);
    test_(h->n4 == NULL);
    test_(h->n5 == NULL);
    NoximHexagon* n = h->createNeighbor("n0");
    test_(n->getLevel() == 2);
    test_(n->getName() == "n0");
    test_(n->getCreator() == h);
    test_(n->getCoord().equals(1,-1,0));

    n = h->createNeighbor("n1");
    test_(n->getCoord().equals(1,0,-1));

    n = h->createNeighbor("n2");
    test_(n->getCoord().equals(0,1,-1));

    n = h->createNeighbor("n3");
    test_(n->getCoord().equals(-1,1,0));

    n = h->createNeighbor("n4");
    test_(n->getCoord().equals(-1,0,1));

    n = h->createNeighbor("n5");
    test_(n->getCoord().equals(0,-1,1));

}

void NoximHexagonTest::testBuildHexagonTree(){
    NoximHexagon::buildHexagonTree(1);
}
