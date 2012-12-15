//
//  miniHandler.cpp
//  eCinemaSetup
//
//  Created by Mick van Olst on 13-11-12.
//
//

#include "miniHandler.h"

//--------------------------------------------------------------
miniHandler::miniHandler(){
    curMiniApp = "";
    pauseApp = false;
    appActive = false;
}

//--------------------------------------------------------------
void miniHandler::init(mpeClientTCP * _cli, ofxMidiOut * _midi, bool *_bMidi, float *_tv1rotVal, float *_tv2rotVal, float *_tv1rotTotVal, float *_tv2rotTotVal){
    client = _cli;
    main.init(_cli, _bMidi, _tv1rotVal, _tv2rotVal, _tv1rotTotVal, _tv2rotTotVal);
    midiOut = _midi;
    bMidi = _bMidi;
    myZoetroop.init(&main, _midi);
    myPlaats_origine.init(&main, _midi);
    myIntentie_interpretatie.init(&main, _midi);
}

//--------------------------------------------------------------
void miniHandler::setup(string id){
    appName = id;
    main.setup(appName);
    
    // Listeners
    ofAddListener(main.doneEvent, this, &miniHandler::doneMini);
}

//--------------------------------------------------------------
void miniHandler::update(){
    main.update(); // so the speedvals of the rotation get calculated
    
    // not yet finished here, need to figure out how to do this with several mini apps etc
    if (!pauseApp) {
        if (curMiniApp == "plaats_origine" && appActive) {
            myPlaats_origine.update();
        }
        if (curMiniApp == "zoetroop" && appActive) {
            myZoetroop.update();
        }
        if (curMiniApp == "intentie_interpretatie" && appActive) {
            myIntentie_interpretatie.update();
        }
    }
}

//--------------------------------------------------------------
void miniHandler::draw(){
    // not yet finished here, need to figure out how to do this with several mini apps etc
    if (!pauseApp) {
        if (curMiniApp == "plaats_origine" && appActive) {
            myPlaats_origine.draw();
        }
        if (curMiniApp == "zoetroop" && appActive) {
            myZoetroop.draw();
        }
        if (curMiniApp == "intentie_interpretatie" && appActive) {
            myIntentie_interpretatie.draw();
        }
    }
}

//--------------------------------------------------------------
string miniHandler::appComesAfter(string prevChapter){
    if(prevChapter == "03_Plaats en origine") {
        return "plaats_origine";
    }
    else if(prevChapter == "04_Test chapter") {
        return "zoetroop";
    }
    else if(prevChapter == "02_New test") {
        return "intentie_interpretatie";
    }
    else {
        return "";
    }
}

//--------------------------------------------------------------
void miniHandler::startMini(string wichApp){
    string tempApp = wichApp;
    appActive = true;
    // for some reason the string is not always recognized as the same, but does printf the same
    if (tempApp.compare("plaats_origine") == 0) {
        myPlaats_origine.setup();
        curMiniApp = tempApp;
        pauseApp = false;
    }
    if (tempApp.compare("zoetroop") == 0) {
        myZoetroop.setup();
        curMiniApp = tempApp;
        pauseApp = false;
    }
    if (tempApp.compare("intentie_interpretatie") == 0) {
        myIntentie_interpretatie.setup();
        curMiniApp = tempApp;
        pauseApp = false;
    }
    printf("tempApp: %s\n",tempApp.c_str());
    printf("curMiniApp: %s\n",curMiniApp.c_str());
}

//--------------------------------------------------------------
void miniHandler::stopMini(int & i){
    // not yet finished here, need to figure out how to do this with several mini apps etc
    printf("mini app said it's time for bed\n");
    appActive = false;
    
    setNull();
}

//--------------------------------------------------------------
void miniHandler::doneMini(int & i){
    // not yet finished here, need to figure out how to do this with several mini apps etc
    
    setNull();
    
    int myInt;
    ofNotifyEvent(doneEvent,myInt,this);
}
//--------------------------------------------------------------
void miniHandler::setNull(){
    // no longer needed
}

//--------------------------------------------------------------
void miniHandler::pauseMini(){
    pauseApp = true;
}

//--------------------------------------------------------------
void miniHandler::playMini(){
    pauseApp = false;
}

//--------------------------------------------------------------
void miniHandler::killMini(){

}