#include "noximguimanager.h"

NoximGuiManager* NoximGuiManager::instance = 0;

NoximGuiManager* NoximGuiManager::getInstance(){

            if(!NoximGuiManager::instance)
                instance = new NoximGuiManager;
            return instance;

}

NoximGuiManager::NoximGuiManager(QObject *parent) :
    QObject(parent)
{
}

void NoximGuiManager::createHexagon(int count){
    emit hexagonCreated(count);
}

