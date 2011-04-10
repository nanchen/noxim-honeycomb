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

class NoximHexagon{

//typedef NoximHexagon ElementType;

public:
//    NoximHexagon(unsigned short level, string name);
    NoximHexagon(unsigned short level, string name, NoximHexagon* creator, int x, int y, int z);
    virtual ~NoximHexagon();

    static NoximHexagon* buildHexagonTree(int meshSize);

    static NoximHMTile* getTile(int x, int y, int z);
    static NoximHMTile* getTile(NoximHMCoord* c);
    static void setTile(int x, int y, int z, NoximHMTile* tile);

    static NoximHMTile* getNeighborTile(int x, int y, int z, int direction);
    static NoximHMTile* getNeighborTile(NoximHMCoord* c, int direction);

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
    NoximHMCoord getCoord(){return coord;}

    // link to neighbors
    NoximHexagon* n0;
    NoximHexagon* n1;
    NoximHexagon* n2;
    NoximHexagon* n3;
    NoximHexagon* n4;
    NoximHexagon* n5;

        // 3-dimensional tile array for 2D honeycomb mesh
    //static NoximHMTile *a[20][20][20];

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

