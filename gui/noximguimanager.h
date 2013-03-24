#ifndef NOXIMGUIMANAGER_H
#define NOXIMGUIMANAGER_H

#include <QObject>

class NoximGuiManager : public QObject
{
    Q_OBJECT
public:
    explicit NoximGuiManager(QObject *parent = 0);
    static NoximGuiManager* getInstance();

//    static NoximGuiManager& getInstance(){
//        static NoximGuiManager obj;
//        return obj;
//    }

    void createHexagon(int);

signals:
    void hexagonCreated(int);

public slots:

private:
    static NoximGuiManager* instance;

};

#endif // NOXIMGUIMANAGER_H
