/*
 * File:   NoximHexagon.cpp
 * Author: nan
 *
 * Created on 06 March 2011, 20:03
 */
#include <unistd.h>
#include <map>
#include "NoximHexagon.h"

// -------------STATIC-----------------------------
// id
static int id = -1;
static int getNextId() {
	return ++id;
}
int NoximHexagon::getLatestId() {
	return id;
}

static map<int, NoximHMTile*> idTileMap;

// tiles array
static NoximHMTile *a[20][20][20];

NoximHMTile* NoximHexagon::getTile(int x, int y, int z) {
	return a[x + OFFSET][y + OFFSET][z + OFFSET];
}

NoximHMTile* NoximHexagon::getTile(const NoximHMCoord& c) {
	return a[c.x + OFFSET][c.y + OFFSET][c.z + OFFSET];
}

NoximHMTile* NoximHexagon::getTile(int id) {
	NoximHMTile* tile = idTileMap[id];
	return tile;
}

void NoximHexagon::setTile(int x, int y, int z, NoximHMTile* tile) {
	a[x + OFFSET][y + OFFSET][z + OFFSET] = tile;
}

int NoximHexagon::coord2Id(const NoximHMCoord& c) {
	NoximHMTile* tile = NoximHexagon::getTile(c);
	if (tile) {
		return tile->getId();
	} else
		return -1;
}

NoximHMCoord NoximHexagon::id2Coord(int id) {
	NoximHMTile* tile = getTile(id);
	assert(tile != NULL);
	return tile->getCoord();
}

static int OFFSET_PX[3] = { 1, 0, 0 };
static int OFFSET_MX[3] = { -1, 0, 0 };
static int OFFSET_PY[3] = { 0, 1, 0 };
static int OFFSET_MY[3] = { 0, -1, 0 };
static int OFFSET_PZ[3] = { 0, 0, 1 };
static int OFFSET_MZ[3] = { 0, 0, -1 };

static int* getOffsetVector(int direction) {
	switch (direction) {
	case DIRECTION_PX:
		return OFFSET_PX;
	case DIRECTION_MX:
		return OFFSET_MX;
	case DIRECTION_PY:
		return OFFSET_PY;
	case DIRECTION_MY:
		return OFFSET_MY;
	case DIRECTION_PZ:
		return OFFSET_PZ;
	case DIRECTION_MZ:
		return OFFSET_MZ;
	}
	return NULL;
}

NoximHMTile* NoximHexagon::getNeighborTile(int x, int y, int z, int direction) {
	NoximHMTile* ret = NULL;
	int* offset = getOffsetVector(direction);
	ret = getTile(x + offset[0], y + offset[1], z + offset[2]);
	return ret;
}

NoximHMTile* NoximHexagon::getNeighborTile(NoximHMCoord* c, int direction) {
	return getNeighborTile(c->x, c->y, c->z, direction);
}

NoximHMTile* NoximHexagon::getNeighborTile(NoximHMCoord c, int direction) {
	return getNeighborTile(c.x, c.y, c.z, direction);
}

char* NoximHexagon::getDirectionStr(const int dir) {
	char* ret = (char*) malloc(2 * sizeof(char));
	switch (dir) {
	case DIRECTION_PX:
		ret = "+x";
		break;
	case DIRECTION_MX:
		ret = "-x";
		break;
	case DIRECTION_PY:
		ret = "+y";
		break;
	case DIRECTION_MY:
		ret = "-y";
		break;
	case DIRECTION_PZ:
		ret = "+z";
		break;
	case DIRECTION_MZ:
		ret = "-z";
		break;
	}
	return ret;
}


int NoximHexagon::fullDir2ReducedDir(const int fullDir) {
	if (fullDir == DIRECTION_PX || fullDir == DIRECTION_MX)
		return DIR_X;
	else if (fullDir == DIRECTION_PY || fullDir == DIRECTION_MY)
		return DIR_Y;
	else if (fullDir == DIRECTION_PZ || fullDir == DIRECTION_MZ)
		return DIR_Z;
	else {
		assert(false);
		return -1;
	}
}

NoximHexagon* NoximHexagon::buildHexagonTree(int meshSize) {
	// reset
	id = -1;

	const int length = 20;
	for (int i = 0; i < length; i++)
		for (int j = 0; j < length; j++)
			for (int k = 0; k < length; k++)
				a[i][j][k] = NULL;

	std::cout << "------------buildHexagonTree-------------: meshSize = "
			<< meshSize << std::endl;
	// root hexagon
	NoximHexagon* rootHexagon = new NoximHexagon(1, "Root", NULL, 0, 0, 0);

	// queue for level-order creation
	Queue queue = Queue_createQueue(100);
	//    std::cout << "Queue for level order creation initilized" << std::endl;
	//    std::cout << "Empty? " << Queue_IsEmpty(queue) << ", isFull? " << Queue_IsFull(queue) << std::endl;
	Queue_enqueue(rootHexagon, queue);

	// statistics
	unsigned int levelCount = 1;
	unsigned int count = 0;
	unsigned short currentLevel = 0;

	while (!Queue_isEmpty(queue)) {
		NoximHexagon* h = (NoximHexagon*) Queue_frontAndDequeue(queue);
		// create next-level neighbors
		if (h) {
			// statistics
			if (h->getLevel() > currentLevel) {
				currentLevel = h->getLevel();
				count += levelCount;
				std::cout << levelCount << " hexagons for level: "
						<< currentLevel << " (total: " << count << ") created."
						<< std::endl;
				levelCount = 0;
			}
			if (h->getLevel() < meshSize) {
				const bool n0Created = h->n0 != NULL;
				const bool n1Created = h->n1 != NULL;
				const bool n2Created = h->n2 != NULL;
				const bool n3Created = h->n3 != NULL;
				const bool n4Created = h->n4 != NULL;
				const bool n5Created = h->n5 != NULL;

				if (!n0Created) {
					h->n0 = h->createNeighbor("n0");
					levelCount++;
					Queue_enqueue(h->n0, queue);
				}
				h->n0->n3 = h;

				if (!n1Created) {
					h->n1 = h->createNeighbor("n1");
					levelCount++;
					Queue_enqueue(h->n1, queue);
				}
				h->n1->n4 = h;
				h->n0->n2 = h->n1;
				h->n1->n5 = h->n0;

				if (!n2Created) {
					h->n2 = h->createNeighbor("n2");
					levelCount++;
					Queue_enqueue(h->n2, queue);
				}
				h->n2->n5 = h;
				h->n1->n3 = h->n2;
				h->n2->n0 = h->n1;

				if (!n3Created) {
					h->n3 = h->createNeighbor("n3");
					levelCount++;
					Queue_enqueue(h->n3, queue);
				}
				h->n3->n0 = h;
				h->n2->n4 = h->n3;
				h->n3->n1 = h->n2;

				if (!n4Created) {
					h->n4 = h->createNeighbor("n4");
					levelCount++;
					Queue_enqueue(h->n4, queue);
				}
				h->n4->n1 = h;
				h->n3->n5 = h->n4;
				h->n4->n2 = h->n3;

				if (!n5Created) {
					h->n5 = h->createNeighbor("n5");
					levelCount++;
					Queue_enqueue(h->n5, queue);
				}
				h->n5->n2 = h;
				h->n4->n0 = h->n5;
				h->n5->n3 = h->n4;

				// relationships between n5 and n0
				h->n5->n1 = h->n0;
				h->n0->n4 = h->n5;
			}
		}
	}
//	count += levelCount;
//	std::cout << levelCount << " hexagons for level: " << currentLevel
//			<< " (total: " << count << ") created." << std::endl;
	Queue_disposeQueue(queue);
	return rootHexagon;
}
// -------------STATIC----------END----------------

NoximHexagon::NoximHexagon(unsigned short level, string name,
		NoximHexagon* creator, int x, int y, int z) {
	setLevel(level);
	setName(name);
	n0 = NULL;
	n1 = NULL;
	n2 = NULL;
	n3 = NULL;
	n4 = NULL;
	n5 = NULL;
	setCreator(creator);
	setCoord(x, y, z);

	NoximHMCoord c = getCoord();
	cout << "Hexagon: [" << toString() << "] created." << endl;
	createTiles();
}

NoximHexagon::~NoximHexagon() {
}

NoximHexagon* NoximHexagon::createNeighbor(string name) {
	unsigned short nextLevel = getLevel() + 1;

	// coordinate
	NoximHMCoord currentCoord = this->getCoord();
	int x, y, z;
	x = currentCoord.x;
	y = currentCoord.y;
	z = currentCoord.z;

	// create neighbour according to positioin
	NoximHexagon* n;
	if (name == "n0")
		n = new NoximHexagon(nextLevel, name, this, x + 1, y - 1, z);
	else if (name == "n1")
		n = new NoximHexagon(nextLevel, name, this, x + 1, y, z - 1);
	else if (name == "n2")
		n = new NoximHexagon(nextLevel, name, this, x, y + 1, z - 1);
	else if (name == "n3")
		n = new NoximHexagon(nextLevel, name, this, x - 1, y + 1, z);
	else if (name == "n4")
		n = new NoximHexagon(nextLevel, name, this, x - 1, y, z + 1);
	else if (name == "n5")
		n = new NoximHexagon(nextLevel, name, this, x, y - 1, z + 1);

	//usleep(1000000);
	//guiManager->createHexagon(level);
	return n;
}

static NoximHMTile* createTileIntoMatrix(int x, int y, int z,
		bool isTypePositive) {
	NoximHMTile* tile = NULL;
	tile = NoximHexagon::getTile(x, y, z);
	if (tile == NULL) {
		char tile_name[20];
		sprintf(tile_name, "Tile[%02d][%02d][%02d]", x, y, z);
		tile = new NoximHMTile(tile_name);
		tile->setCoord(x, y, z);
		tile->setId(getNextId());
		//type
		if (isTypePositive)
			tile->type = NoximHMTile::POSITIVE;
		else
			tile->type = NoximHMTile::NEGATIVE;
		NoximHexagon::setTile(x, y, z, tile);
		idTileMap[tile->getId()] = tile;
		cout << "\t" << tile->toString() << " created" << endl;
	} else {
		//        cout << "\tTry to look for Tile: (" << x << ", " << y << ", " << z << ")" ;
		//        NoximHMCoord* c=tile->coord;
		//        cout <<", but Tile: (" << c->toString() << ") already exists" << endl;
	}
	return tile;
}

void NoximHexagon::createTiles() {
	NoximHMCoord c = this->getCoord();
	int x, y, z;
	x = c.x;
	y = c.y;
	z = c.z;

	pxTile = createTileIntoMatrix(x + 1, y, z, true);
	mxTile = createTileIntoMatrix(x, y + 1, z + 1, false);
	pyTile = createTileIntoMatrix(x, y + 1, z, true);
	myTile = createTileIntoMatrix(x + 1, y, z + 1, false);
	pzTile = createTileIntoMatrix(x, y, z + 1, true);
	mzTile = createTileIntoMatrix(x + 1, y + 1, z, false);
}

string NoximHexagon::toString() {
	string ret = getName() + " at " + getCoord().toString();
	if (getCreator()) {
		ret += " <= " + getCreator()->toString();
	}
	return ret;
}

void NoximHexagon::setCreator(NoximHexagon* creator) {
	this->creator = creator;
}

NoximHexagon* NoximHexagon::getCreator() const {
	return creator;
}

void NoximHexagon::setName(string name) {
	this->name = name;
}

string NoximHexagon::getName() const {
	return name;
}

unsigned short NoximHexagon::getLevel() const {
	return level;
}

void NoximHexagon::setLevel(unsigned short level) {
	this->level = level;
}
void NoximHexagon::setCoord(int x, int y, int z) {
	NoximHMCoord c(x, y, z);
	coord = c;
}
