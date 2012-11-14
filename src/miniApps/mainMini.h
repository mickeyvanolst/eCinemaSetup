//
//  mainMini.h
//  eCinemaSetup
//
//  Created by Mick van Olst on 13-11-12.
//
//

#ifndef eCinemaSetup_mainMini_h
#define eCinemaSetup_mainMini_h

#include "ofMain.h"
#include "mpeClientTCP.h"

// this is our base app, this class should contain all functions and variables that would be needed throughout the mini apps. For instance the ability to use the MPE client and to play movies etc

class mainMini : ofBaseApp {
public:
    mainMini(mpeClientTCP *_cli);
    void setup(string id);
    void update();
    void reset();
    void draw();
    void done();
    
    mpeClientTCP *client;
    string appName;
    
    int appCounter;
    
    ofEvent<int>        doneEvent;
};

#endif
