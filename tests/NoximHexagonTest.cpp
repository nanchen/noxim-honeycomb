#include "NoximHexagonTest.h"
#include "NoximHexagon.h"
#include "NoximHMTile.h"

void NoximHexagonTest::run() {
	testConstructor();
	testCreateNeighbor();
	testBuildHexagonTree();
	testTileType();
	testIdAndCoord();
	testNumberOfTiles();
}

void NoximHexagonTest::testConstructor() {
	NoximHexagon* h = new NoximHexagon(1, "Root", NULL, 0, 0, 0);
	test_(h->getLevel() == 1);
	test_(h->getName() == "Root");
	test_(h->getCreator() == NULL);
	test_(h->getCoord().equals(0,0,0));
}

void NoximHexagonTest::testCreateNeighbor() {
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

void NoximHexagonTest::testBuildHexagonTree() {
	NoximHexagon::buildHexagonTree(1);
}

void NoximHexagonTest::testTileType() {
	// px
	NoximHMTile* tile = NoximHexagon::getTile(1, 0, 0);
	//    std::cout << "type = " << tile->getType() << std::endl;
	test_(tile->getType() == NoximHMTile::POSITIVE);
	// mz
	tile = NoximHexagon::getTile(1, 1, 0);
	test_(tile->getType() == NoximHMTile::NEGATIVE);
	// py
	tile = NoximHexagon::getTile(0, 1, 0);
	test_(tile->getType() == NoximHMTile::POSITIVE);
	// mx
	tile = NoximHexagon::getTile(0, 1, 1);
	test_(tile->getType() == NoximHMTile::NEGATIVE);
	//pz
	tile = NoximHexagon::getTile(0, 0, 1);
	test_(tile->getType() == NoximHMTile::POSITIVE);
	//my
	tile = NoximHexagon::getTile(1, 0, 1);
	test_(tile->getType() == NoximHMTile::NEGATIVE);

}

void NoximHexagonTest::testIdAndCoord() {
	//    std::cout <<" id = "<< NoximHexagon::getLatestId() << std::endl;
	test_(NoximHexagon::getLatestId() == 5);
	NoximHMCoord c(1, 0, 0);
	//    std::cout << "px id = " << NoximHexagon::coord2Id(c) << std::endl;
	test_(NoximHexagon::coord2Id(c) == 0);
	test_(NoximHexagon::coord2Id(NoximHMCoord(0,1,1))==1);
	test_(NoximHexagon::coord2Id(NoximHMCoord(0,1,0))==2);
	test_(NoximHexagon::coord2Id(NoximHMCoord(1,0,1))==3);
	test_(NoximHexagon::coord2Id(NoximHMCoord(0,0,1))==4);
	test_(NoximHexagon::coord2Id(NoximHMCoord(1,1,0))==5);

	//    std::cout << NoximHexagon::id2Coord(0).toString() << std::endl;
	test_(NoximHexagon::id2Coord(0) == NoximHMCoord(1,0,0));
	test_(NoximHexagon::id2Coord(1) == NoximHMCoord(0,1,1));
	test_(NoximHexagon::id2Coord(2) == NoximHMCoord(0,1,0));
	test_(NoximHexagon::id2Coord(3) == NoximHMCoord(1,0,1));
	test_(NoximHexagon::id2Coord(4) == NoximHMCoord(0,0,1));
	test_(NoximHexagon::id2Coord(5) == NoximHMCoord(1,1,0));
}

void NoximHexagonTest::testNumberOfTiles() {
	for (int i = 1; i < 11; i++) {
		NoximHexagon::buildHexagonTree(i);
		test_(NoximHexagon::getLatestId() == 6*i*i -1);
	}
}
