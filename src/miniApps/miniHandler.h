//
//  miniHandler.h
//  eCinemaSetup
//
//  Created by Mick van Olst on 13-11-12.
//
//

#ifndef eCinemaSetup_miniHandler_h
#define eCinemaSetup_miniHandler_h

#include "mpeClientTCP.h"
#include "mainMini.h" // our mini app base app, all mini apps will derive from this one

#include "01_TestMini.h"

class miniHandler {
public:
    miniHandler(mpeClientTCP * _cli);
    void setup(string id);
    void update();
    void draw();
    void startMini();
    void stopMini(int & i);
    void killMini();
    
    string appName;
    long int curAppCounter;
    
    mpeClientTCP *client;
    mainMini *main;
    
    // ----- List of mini Apps -----
    testMini *myTestMini;
    
private:
    long int startCurAppCounter;
};

#endif
