//
//  miniHandler.cpp
//  eCinemaSetup
//
//  Created by Mick van Olst on 13-11-12.
//
//

#include "miniHandler.h"

//--------------------------------------------------------------
miniHandler::miniHandler(mpeClientTCP * _cli){
    client = _cli;
    main = new mainMini(client);
    curMiniApp = "";
    pauseApp = false;
    appActive = false;
    
    myMaker_kijker = NULL; // otherwise it will trigger anyways..
    myPlaats_origine = NULL;
    myZoetroop = NULL;
}

//--------------------------------------------------------------
void miniHandler::setup(string id){
    appName = id;
    main->setup(appName);
    
    //startMini("01_TestMini"); // for some reason this only works when triggered at start, not through broadcast..
    
    // Listeners
    ofAddListener(main->doneEvent, this, &miniHandler::doneMini);
}

//--------------------------------------------------------------
void miniHandler::update(){
    main->update(); // so the speedvals of the rotation get calculated
    
    // not yet finished here, need to figure out how to do this with several mini apps etc
    if (!pauseApp) {
        if (myMaker_kijker != NULL && curMiniApp == "maker_kijker" && appActive) {
            myMaker_kijker->update();
        }
        if (myPlaats_origine != NULL && curMiniApp == "plaats_origine" && appActive) {
            myPlaats_origine->update();
        }
        if (myZoetroop != NULL && curMiniApp == "zoetroop" && appActive) {
            myZoetroop->update();
        }
    }
}

//--------------------------------------------------------------
void miniHandler::draw(){
    // not yet finished here, need to figure out how to do this with several mini apps etc
    if (!pauseApp) {
        if (myMaker_kijker != NULL && curMiniApp == "maker_kijker" && appActive) {
            myMaker_kijker->draw();
        }
        if (myPlaats_origine != NULL && curMiniApp == "plaats_origine" && appActive) {
            myPlaats_origine->draw();
        }
        if (myZoetroop != NULL && curMiniApp == "zoetroop" && appActive) {
            myZoetroop->draw();
        }
    }
}

//--------------------------------------------------------------
string miniHandler::appComesAfter(string prevChapter){
    if (prevChapter == "02_Rise of the Guardians") {
        return "maker_kijker";
    }
    else if(prevChapter == "03_Plaats en origine") {
        return "plaats_origine";
    }
    else if(prevChapter == "04_Test chapter") {
        return "zoetroop";
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
    if (tempApp.compare("maker_kijker") == 0) {
        myMaker_kijker = new maker_kijker(main);
        myMaker_kijker->setup();
        curMiniApp = tempApp;
        pauseApp = false;
    }
    if (tempApp.compare("plaats_origine") == 0) {
        myPlaats_origine = new plaats_origine(main);
        myPlaats_origine->setup();
        curMiniApp = tempApp;
        pauseApp = false;
    }
    if (tempApp.compare("zoetroop") == 0) {
        myZoetroop = new zoetroop(main);
        myZoetroop->setup();
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
    if (curMiniApp == "maker_kijker" && myMaker_kijker != NULL) {
        delete myMaker_kijker;
        myMaker_kijker = NULL;
        curMiniApp = "";
    }
    if (curMiniApp == "plaats_origine" && myPlaats_origine != NULL) {
        delete myPlaats_origine;
        myPlaats_origine = NULL;
        curMiniApp = "";
    }
    if (curMiniApp == "zoetroop" && myZoetroop != NULL) {
        delete myZoetroop;
        myZoetroop = NULL;
        curMiniApp = "";
    }
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