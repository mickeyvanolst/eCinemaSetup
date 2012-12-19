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
    myInterview_bezoekers.init(&main, _midi);
    
    if (XML.loadFile("app_content/contentOrder.xml")) {
        XML.pushTag("order");
        plaats_origineComesAfter            = XML.getValue("plaas_origine:after", "", 0);
        zoetroopComesAfter                  = XML.getValue("zoetroop:after", "", 0);
        intentie_interpretatieComesAfter    = XML.getValue("intentie_interpretatie:after", "", 0);
        interview_bezoekersComesAfter       = XML.getValue("interview_bezoekers:after", "", 0);
        XML.popTag();
    } else {
        cout << "loading contentOrder.xml failed, needed for interactive chapters\n";
    }
    
    
    // setting after what chapter the interactive chapter should play.
//    plaats_origineComesAfter            = "shizzaaah";
//    zoetroopComesAfter                  = "4 Hoofdstuk Techniek";
//    intentie_interpretatieComesAfter    = "7 Hoofdstuk Maker vs Kijker";
//    interview_bezoekersComesAfter       = "2_2 Hoofdstuk Identiteit";
    
    // manually setting class names into strings..
    plaats_origine                      = "myPlaats_origine";
    zoetroop                            = "zoetroop";
    intentie_interpretatie              = "intentie_interpretatie";
    interview_bezoekers                 = "interview_bezoekers";
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
        if (curMiniApp == plaats_origine && appActive) {
            myPlaats_origine.update();
        }
        if (curMiniApp == zoetroop && appActive) {
            myZoetroop.update();
        }
        if (curMiniApp == intentie_interpretatie && appActive) {
            myIntentie_interpretatie.update();
        }
        if (curMiniApp == interview_bezoekers && appActive) {
            myInterview_bezoekers.update();
        }
    }
}

//--------------------------------------------------------------
void miniHandler::draw(){
    // not yet finished here, need to figure out how to do this with several mini apps etc
    if (!pauseApp) {
        if (curMiniApp == plaats_origine && appActive) {
            myPlaats_origine.draw();
        }
        if (curMiniApp == zoetroop && appActive) {
            myZoetroop.draw();
        }
        if (curMiniApp == intentie_interpretatie && appActive) {
            myIntentie_interpretatie.draw();
        }
        if (curMiniApp == interview_bezoekers && appActive) {
            myInterview_bezoekers.draw();
        }
    }
}

//--------------------------------------------------------------
string miniHandler::appComesAfter(string prevChapter){
    if(prevChapter == plaats_origineComesAfter) {
        return plaats_origine;
    }
    else if(prevChapter == zoetroopComesAfter) {
        return zoetroop;
    }
    else if(prevChapter == intentie_interpretatieComesAfter) {
        return intentie_interpretatie;
    }
    else if(prevChapter == interview_bezoekersComesAfter) {
        return interview_bezoekers;
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
    if (tempApp.compare(plaats_origine) == 0) {
        myPlaats_origine.setup();
        curMiniApp = tempApp;
        pauseApp = false;
    }
    if (tempApp.compare(zoetroop) == 0) {
        myZoetroop.setup();
        curMiniApp = tempApp;
        pauseApp = false;
    }
    if (tempApp.compare(intentie_interpretatie) == 0) {
        myIntentie_interpretatie.setup();
        curMiniApp = tempApp;
        pauseApp = false;
    }
    if (tempApp.compare(interview_bezoekers) == 0) {
        myInterview_bezoekers.setup();
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
    
    if (curMiniApp == plaats_origine) {
        myPlaats_origine.exit();
    }
    if (curMiniApp == zoetroop) {
        myZoetroop.exit();
    }
    if (curMiniApp == intentie_interpretatie) {
        myIntentie_interpretatie.exit();
    }
    if (curMiniApp == interview_bezoekers) {
        myInterview_bezoekers.exit();
    }
}

//--------------------------------------------------------------
void miniHandler::doneMini(int & i){
    // not yet finished here, need to figure out how to do this with several mini apps etc
    
    int myInt;
    stopMini(myInt);
    ofNotifyEvent(doneEvent,myInt,this);
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