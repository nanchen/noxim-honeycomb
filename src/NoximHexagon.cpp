/*
 * File:   NoximHexagon.cpp
 * Author: nan
 *
 * Created on 06 March 2011, 20:03
 */
#include <unistd.h>

#include "NoximHexagon.h"

// tiles array
static NoximHMTile *a[20][20][20];
//#define OFFSET 10;
static const int OFFSET = 10;
NoximHMTile* NoximHexagon::getTile(int x, int y, int z){
    return a[x+OFFSET][y+OFFSET][z+OFFSET];
}
void NoximHexagon::setTile(int x, int y, int z, NoximHMTile* tile){
    a[x+OFFSET][y+OFFSET][z+OFFSET] = tile;
}
static int OFFSET_PX[3] = {1, 0, 0};
static int OFFSET_MX[3] = {-1, 0, 0};
static int OFFSET_PY[3] = {0, 1, 0};
static int OFFSET_MY[3] = {0, -1, 0};
static int OFFSET_PZ[3] = {0, 0, 1};
static int OFFSET_MZ[3] = {0, 0, -1};

static int* getOffsetVector(int direction)
{
    switch(direction)
    {
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

NoximHMTile* NoximHexagon::getNeighborTile(int x, int y, int z, int direction){
    NoximHMTile* ret = NULL;
    int* offset = getOffsetVector(direction);
    ret = getTile(x+offset[0], y+offset[1], z+offset[3]);
    return ret;
}

//NoximHexagon::NoximHexagon(unsigned short level, string name)
//{
//    setLevel(level);
//    setName(name);
//    n0 = NULL;
//    n1 = NULL;
//    n2 = NULL;
//    n3 = NULL;
//    n4 = NULL;
//    n5 = NULL;
//    creator = NULL;
//    //guiManager = NoximGuiManager::getInstance();
//    //initilizeTiles();
//}

NoximHexagon::NoximHexagon(unsigned short level, string name, NoximHexagon* creator, int x, int y, int z)
{
    setLevel(level);
    setName(name);
    n0 = NULL;
    n1 = NULL;
    n2 = NULL;
    n3 = NULL;
    n4 = NULL;
    n5 = NULL;
    setCreator(creator);
    setCoord(x,y,z);

    NoximHMCoord c=getCoord();
    cout << "Hexagon: [" << toString() << "] created, central coordinate: " << "x:"<<c.x << " y:"<<c.y << " z:"<< c.z << endl;
    createTiles();
}


NoximHexagon::~NoximHexagon() {}

NoximHexagon* NoximHexagon::buildHexagonTree(int meshSize)
{

    const int length=20;
    for(int i=0;i<length;i++)
        for(int j=0;j<length;j++)
            for(int k=0;k<length;k++)
                a[i][j][k] = NULL;

    std::cout << "------------buildHexagonTree-------------: meshSize = " << meshSize << std::endl;
    // root hexagon
    unsigned short level = 1;
    NoximHexagon* rootHexagon = new NoximHexagon(level, "Root", NULL, 0, 0, 0);

    // queue for level-order creation
    Queue queue = Queue_CreateQueue(100);
//    std::cout << "Queue for level order creation initilized" << std::endl;
//    std::cout << "Empty? " << Queue_IsEmpty(queue) << ", isFull? " << Queue_IsFull(queue) << std::endl;
    Queue_Enqueue(rootHexagon, queue);

    // statistics
    unsigned int levelCount =  1;
    unsigned int count = 0;
    unsigned short currentLevel = 0;

    while(!Queue_IsEmpty(queue))
    {
        NoximHexagon* h = (NoximHexagon*) Queue_FrontAndDequeue(queue);
        // create next-level neighbors
        if (h && h->getLevel() < meshSize)
        {
            // statistics
            if(h->getLevel()>currentLevel)
            {
                currentLevel =  h->getLevel();
                count +=levelCount;
                std::cout << levelCount << " hexagons for level: " << currentLevel << " (total: " << count << ") created." << std::endl;
                levelCount = 0;
            }
            const bool n0Created = h->n0 != NULL;
            const bool n1Created = h->n1 != NULL;
            const bool n2Created = h->n2 != NULL;
            const bool n3Created = h->n3 != NULL;
            const bool n4Created = h->n4 != NULL;
            const bool n5Created = h->n5 != NULL;

            if (!n0Created)
            {
                h->n0 = h->createNeighbor("n0");
                levelCount++;
                Queue_Enqueue(h->n0, queue);
            }
            h->n0->n3 = h;

            if (!n1Created)
            {
                h->n1 = h->createNeighbor("n1");
                levelCount++;
                Queue_Enqueue(h->n1, queue);
            }
            h->n1->n4 = h;
            h->n0->n2 = h->n1;
            h->n1->n5 = h->n0;

            if (!n2Created)
            {
                h->n2 = h->createNeighbor("n2");
                levelCount++;
                Queue_Enqueue(h->n2, queue);
            }
            h->n2->n5 = h;
            h->n1->n3 = h->n2;
            h->n2->n0 = h->n1;

            if (!n3Created)
            {
                h->n3 = h->createNeighbor("n3");
                levelCount++;
                Queue_Enqueue(h->n3, queue);
            }
            h->n3->n0 = h;
            h->n2->n4 = h->n3;
            h->n3->n1 = h->n2;

            if (!n4Created)
            {
                h->n4 = h->createNeighbor("n4");
                levelCount++;
                Queue_Enqueue(h->n4, queue);
            }
            h->n4->n1 = h;
            h->n3->n5 = h->n4;
            h->n4->n2 = h->n3;

            if (!n5Created)
            {
                h->n5 = h->createNeighbor("n5");
                levelCount++;
                Queue_Enqueue(h->n5, queue);
            }
            h->n5->n2 = h;
            h->n4->n0 = h->n5;
            h->n5->n3 = h->n4;

            // relationships between n5 and n0
            h->n5->n1 = h->n0;
            h->n0->n4 = h->n5;
        }
    }
    count +=levelCount;
    std::cout << levelCount << " hexagons for level: " << currentLevel << " (total: " << count << ") created." << std::endl;
    Queue_DisposeQueue(queue);
    return rootHexagon;
}

NoximHexagon* NoximHexagon::createNeighbor(string name)
{
    unsigned short nextLevel = level + 1;

    // coordinate
    NoximHMCoord currentCoord=this->getCoord();
    int x,y,z;
    x = currentCoord.x;
    y = currentCoord.y;
    z = currentCoord.z;

    // create neighbour according to positioin
    NoximHexagon* n;
    if(name=="n0")
        n = new NoximHexagon(nextLevel, name, this, x+1, y-1, z);
    else if(name == "n1")
        n = new NoximHexagon(nextLevel, name, this, x+1, y, z-1);
    else if(name == "n2")
        n = new NoximHexagon(nextLevel, name, this, x, y+1, z-1);
    else if(name == "n3")
        n = new NoximHexagon(nextLevel, name, this, x-1, y+1, z);
    else if(name == "n4")
        n = new NoximHexagon(nextLevel, name, this, x-1, y, z+1);
    else if(name == "n5")
        n = new NoximHexagon(nextLevel, name, this, x, y-1, z+1);

    //usleep(1000000);
    //guiManager->createHexagon(level);
    return n;
}


static NoximHMTile* fillTileIntoArray(int x, int y, int z)
{
    NoximHMTile* tile =  NULL;
    tile = NoximHexagon::getTile(x,y,z);
    if(tile == NULL)
    {
        char tile_name[20];
        sprintf(tile_name, "Tile[%02d][%02d][%02d]", x, y, z);
        tile = new NoximHMTile(tile_name);
        tile->setCoord(x,y,z);
        NoximHexagon::setTile(x,y,z,tile);
        cout << "\t" << tile_name << " created"<< endl;
    }else
    {
 //       cout << "\tTry to look for Tile: (" << x << ", " << y << ", " << z << ")" ;
 //       NoximHMCoord* c=tile->coord;
//        cout <<", but Tile: (" << c->x << ", " << c->y << ", " << c->z << ") already exists" << endl;
    }
    return tile;
}

void NoximHexagon::createTiles()
{
    NoximHMCoord c = this->getCoord();
    int x,y,z;
    x = c.x;
    y = c.y;
    z = c.z;

    pxTile = fillTileIntoArray(x+1,y,z);
    mxTile = fillTileIntoArray(x,y+1,z+1);
    pyTile = fillTileIntoArray(x,y+1,z);
    myTile = fillTileIntoArray(x+1,y,z+1);
    pzTile = fillTileIntoArray(x,y,z+1);
    mzTile = fillTileIntoArray(x+1,y+1,z);
}


string NoximHexagon::toString()
{
    string ret = getName();
    if(getCreator())
    {
        ret += " <= " + getCreator()->toString();
    }
    return ret;
}

void NoximHexagon::setCreator(NoximHexagon* creator)
{
    this->creator = creator;
}

NoximHexagon* NoximHexagon::getCreator() const
{
    return creator;
}

void NoximHexagon::setName(string name)
{
    this->name = name;
}

string NoximHexagon::getName() const
{
    return name;
}

unsigned short NoximHexagon::getLevel() const
{
    return level;
}

void NoximHexagon::setLevel(unsigned short level)
{
    this->level = level;
}
void NoximHexagon::setCoord(int x, int y, int z)
{
    NoximHMCoord c;
    c.x = x;
    c.y = y;
    c.z = z;
    coord = c;
}
