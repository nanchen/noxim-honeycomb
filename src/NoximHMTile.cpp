#include "NoximHMTile.h"

void NoximHMTile::setCoord(int x, int y, int z) {
	coord.x = x;
	coord.y = y;
	coord.z = z;
}
char* NoximHMTile::toString() const {
	char* ret = (char*) malloc(50 * sizeof(char));
	sprintf(ret, "Tile[%d] at %s", id, coord.toString());
	return ret;
}

NoximHMTile::TileType NoximHMTile::getType() const {
	return type;
}

unsigned int NoximHMTile::getId() const {
	assert(id>=0);
	return id;
}

