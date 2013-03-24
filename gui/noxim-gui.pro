
INCLUDEPATH += /home/nan/dev/systemc-2.2.0/include/
LIBS += /home/nan/dev/systemc-2.2.0/lib-linux64/libsystemc.a

HEADERS       = renderarea.h \
                window.h \
    ../src/NoximTile.h \
    ../src/NoximStats.h \
    ../src/NoximRouter.h \
    ../src/NoximReservationTable.h \
    ../src/NoximProcessingElement.h \
    ../src/NoximPower.h \
    ../src/NoximNoC.h \
    ../src/NoximMain.h \
    ../src/NoximLocalRoutingTable.h \
    ../src/NoximHexagon.h \
    ../src/NoximGlobalTrafficTable.h \
    ../src/NoximGlobalStats.h \
    ../src/NoximGlobalRoutingTable.h \
    ../src/NoximCmdLineParser.h \
    ../src/NoximBuffer.h \
    noximguimanager.h \
    params.h \
    mainwindow.h \
    diagramtextitem.h \
    diagramscene.h \
    diagramitem.h \
    arrow.h \
    noximthread.h
SOURCES       = main.cpp \
                renderarea.cpp \
                window.cpp \
    ../src/NoximStats.cpp \
    ../src/NoximRouter.cpp \
    ../src/NoximReservationTable.cpp \
    ../src/NoximProcessingElement.cpp \
    ../src/NoximPower.cpp \
    ../src/NoximNoC.cpp \
    ../src/NoximMain.cpp \
    ../src/NoximLocalRoutingTable.cpp \
    ../src/NoximHexagon.cpp \
    ../src/NoximGlobalTrafficTable.cpp \
    ../src/NoximGlobalStats.cpp \
    ../src/NoximGlobalRoutingTable.cpp \
    ../src/NoximCmdLineParser.cpp \
    ../src/NoximBuffer.cpp \
    noximguimanager.cpp \
    mainwindow.cpp \
    diagramtextitem.cpp \
    diagramscene.cpp \
    diagramitem.cpp \
    arrow.cpp \
    noximthread.cpp

RESOURCES += \
    diagramscene.qrc
