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
    startCurAppCounter = 0;
}

//--------------------------------------------------------------
void miniHandler::setup(string id){
    appName = id;
    main->setup(appName);

    startMini(); // should be called from an upper level or something include which app in some way
    
    // Listeners
    ofAddListener(main->doneEvent, this, &miniHandler::stopMini);
}

//--------------------------------------------------------------
void miniHandler::update(){
    // so we know how long a mini app has been running
    curAppCounter = ofGetElapsedTimeMillis() - startCurAppCounter;
    main->update(curAppCounter);
    
    // not yet finished here, need to figure out how to do this with several mini apps etc
    if (myTestMini == NULL) {
        // nuthing..
    } else {
        myTestMini->update();
    }
}

//--------------------------------------------------------------
void miniHandler::draw(){
    // not yet finished here, need to figure out how to do this with several mini apps etc
    if (myTestMini == NULL) {
        // nuthing..
    } else {
        myTestMini->draw();
    }
}

//--------------------------------------------------------------
void miniHandler::startMini(){
    startCurAppCounter = ofGetElapsedTimeMillis();
    
    // not yet finished here, need to figure out how to do this with several mini apps etc
    myTestMini = new testMini(main);
    myTestMini->setup();
}

//--------------------------------------------------------------
void miniHandler::stopMini(int & i){
    // not yet finished here, need to figure out how to do this with several mini apps etc
    printf("mini app said it's time for bed\n");
    delete myTestMini;
    myTestMini = NULL;
}

//--------------------------------------------------------------
void miniHandler::killMini(){
    
}