#include "NoximHMTile.h"

void NoximHMTile::setCoord(int x, int y, int z) {
	coord.x = x;
	coord.y = y;
	coord.z = z;
	if (r)
		r->setCoord(coordF);
	if (pe)
		pe->coord = coord;
}
char* NoximHMTile::toString() const {
	char* ret = (char*) malloc(50 * sizeof(char));
	sprintf(ret, "Tile[%d] at %s", id, coord.toString());
	return ret;
}

NoximHMTile::TileType NoximHMTile::getType() const {
	return type;
}

void NoximHMTile::setId(int aId){
	assert(id>=0);
	id = aId;
	if(r)
		r->local_id = getId();
	if(pe)
		pe->local_id = getId();
}

int NoximHMTile::getId() const {
	assert(id>=0);
	return id;
}

