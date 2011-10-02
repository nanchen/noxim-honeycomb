/*
 * File:   NoximHexagon.h
 * Author: nan
 *
 * Created on 06 March 2011, 20:03
 */

#ifndef NOXIMHEXAGON_H
#define	NOXIMHEXAGON_H

#include <stdlib.h>

#include <string>

#include "NoximHMNoC.h"
#include "NoximHMTile.h"

extern "C" {
#include "queue.h"
}

//#include "../gui/noximguimanager.h"

using namespace std;

class NoximHexagon {

	//typedef NoximHexagon ElementType;

public:
	// -------------STATIC-----------------------------
	static const int OFFSET = 20;
	static NoximHexagon* buildHexagonTree(int meshSize);

	static NoximHMTile* getTile(int x, int y, int z);
	static NoximHMTile* getTile(const NoximHMCoord& c);
	static NoximHMTile* getTile(int id);
	static void setTile(int x, int y, int z, NoximHMTile* tile);

	static NoximHMTile* getNeighborTile(int x, int y, int z, int direction);
	static NoximHMTile* getNeighborTile(NoximHMCoord* c, int direction);
	static NoximHMTile* getNeighborTile(NoximHMCoord c, int direction);
	static char* getDirectionStr(const int dir);

	static int getLatestId();
	static int coord2Id(const NoximHMCoord& c);
	static NoximHMCoord id2Coord(int id);

	static int fullDir2ReducedDir(const int fullDir);
	// ------END-----STATIC----------------------------

	//    NoximHexagon(unsigned short level, string name);
	NoximHexagon(unsigned short level, string name, NoximHexagon* creator,
			int x, int y, int z);
	virtual ~NoximHexagon();

	NoximHexagon* createNeighbor(string name);

	void createTiles();

	string toString();

	void setCreator(NoximHexagon* creator);
	NoximHexagon* getCreator() const;
	void setName(string name);
	string getName() const;
	unsigned short getLevel() const;
	void setLevel(unsigned short level);
	void setCoord(int x, int y, int z);
	NoximHMCoord getCoord() {
		return coord;
	}

	// link to neighbors
	NoximHexagon* n0;
	NoximHexagon* n1;
	NoximHexagon* n2;
	NoximHexagon* n3;
	NoximHexagon* n4;
	NoximHexagon* n5;

private:
	unsigned short level;
	string name;
	// tiles as vertices
	NoximHMTile* pxTile;
	NoximHMTile* mxTile;
	NoximHMTile* pyTile;
	NoximHMTile* myTile;
	NoximHMTile* pzTile;
	NoximHMTile* mzTile;

	// link to creator
	NoximHexagon* creator;

	// coordinate
	NoximHMCoord coord;

	// GUI
	//NoximGuiManager* guiManager;

};

#endif	/* NOXIMHEXAGON_H */

