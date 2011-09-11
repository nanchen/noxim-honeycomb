#include "NoximHMTile.h"

void NoximHMTile::setCoord(int x, int y, int z) {
	coord->x = x;
	coord->y = y;
	coord->z = z;
}
char* NoximHMTile::toString() const {
	char* ret = (char*) malloc(50 * sizeof(char));
	sprintf(ret, "Tile at %s", coord->toString());
	return ret;
}

NoximHMTile::TileType NoximHMTile::getType() const
{
	return type;
}



