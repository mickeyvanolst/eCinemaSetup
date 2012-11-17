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
    
    myTestMini = NULL; // otherwise it will trigger anyways..
}

//--------------------------------------------------------------
void miniHandler::setup(string id){
    appName = id;
    main->setup(appName);
    
    //startMini("01_TestMini"); // for some reason this only works when triggered at start, not through broadcast..
    
    // Listeners
    ofAddListener(main->doneEvent, this, &miniHandler::stopMini);
}

//--------------------------------------------------------------
void miniHandler::update(){
    // not yet finished here, need to figure out how to do this with several mini apps etc
    if (!pauseApp) {
        if (myTestMini != NULL && curMiniApp == "01_TestMini") {
            myTestMini->update();
        }
    }

}

//--------------------------------------------------------------
void miniHandler::draw(){
    // not yet finished here, need to figure out how to do this with several mini apps etc
    if (myTestMini != NULL && curMiniApp == "01_TestMini") {
        myTestMini->draw();
    }
}

//--------------------------------------------------------------
string miniHandler::appComesAfter(string prevChapter){
    if (prevChapter == "02_Rise of the Guardians") {
        return "01_TestMini";
    } else {
        return "";
    }
}

//--------------------------------------------------------------
void miniHandler::startMini(string wichApp){
    string tempApp = wichApp;
    appActive = true;
    // for some reason the string is not always recognized as the same, but does printf the same
    if (tempApp.compare("01_TestMini") == 0) {
        myTestMini = new testMini(main);
        myTestMini->setup();
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
    
    if (curMiniApp == "01_TestMini" && myTestMini != NULL) {
        delete myTestMini;
        myTestMini = NULL;
        curMiniApp = "";
    }
    
    appActive = false;
    
    //int myInt;
    //ofNotifyEvent(doneEvent,myInt,this);
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