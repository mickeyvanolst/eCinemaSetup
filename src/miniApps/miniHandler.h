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

#include "maker_kijker.h"
#include "plaats_origine.h"
#include "zoetroop.h"

class miniHandler {
public:
    miniHandler(mpeClientTCP * _cli);
    void setup(string id);
    void update();
    void draw();
    void startMini(string wichApp);
    void stopMini(int & i);
    void doneMini(int & i);
    void pauseMini();
    void playMini();
    void killMini();
    string appComesAfter(string prevChapter);
    
    ofFbo appFbo;
    
    string appName;
    string curMiniApp;
    bool pauseApp;
    bool appActive;
    
    mpeClientTCP *client;
    mainMini *main;
    
    ofEvent<int>        doneEvent;
    
    // ----- List of mini Apps -----
    maker_kijker        *myMaker_kijker; // after 02_Rise of the Guardians
    plaats_origine      *myPlaats_origine; // after 03_Plaats en origine
    zoetroop            *myZoetroop;
    
private:
    void setNull();
};

#endif
