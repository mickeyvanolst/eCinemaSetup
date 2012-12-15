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
#include "mainMini.h" // our mini app base app, all mini apps will be connected to this one
#include "ofxMidi.h"

#include "plaats_origine.h"
#include "zoetroop.h"
#include "intentie_interpretatie.h"

class miniHandler {
public:
    miniHandler();
    
    void init(mpeClientTCP * _cli, ofxMidiOut * _midi, bool *_bMidi, float *_tv1rotVal, float *_tv2rotVal, float *_tv1rotTotVal, float *_tv2rotTotVal);
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
    bool *bMidi;
    
    mpeClientTCP *client;
    ofxMidiOut *midiOut;
    mainMini main;
    
    ofEvent<int>        doneEvent;
    
    // ----- List of mini Apps -----
    plaats_origine          myPlaats_origine; // after 03_Plaats en origine
    zoetroop                myZoetroop; // after 04_Test chapter
    intentie_interpretatie  myIntentie_interpretatie; // after 02_New test
    
    
private:
    void setNull();
};

#endif
