#include "noximthread.h"
#include "params.h"
#include "../src/NoximMain.h"

void NoximThread::run(){
    sc_main(argCount,argValue);
}
