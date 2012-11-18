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
    void startMini(string wichApp);
    void stopMini(int & i);
    void pauseMini();
    void playMini();
    void killMini();
    string appComesAfter(string prevChapter);
    
    string appName;
    string curMiniApp;
    bool pauseApp;
    bool appActive;
    
    mpeClientTCP *client;
    mainMini *main;
    
    ofEvent<int>        doneEvent;
    
    // ----- List of mini Apps -----
    testMini *myTestMini; // 01_TestMini - after 02_Rise of the Guardians
    
private:
};

#endif
