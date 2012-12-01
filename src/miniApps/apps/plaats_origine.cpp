//
//  plaats_origine.cpp
//  eCinemaSetup
//
//  Created by Mick van Olst on 01-12-12.
//
//

#include "plaats_origine.h"

//--------------------------------------------------------------
plaats_origine::plaats_origine(mainMini *_mai){
    main = _mai;
}

//--------------------------------------------------------------
void plaats_origine::setup(){
    initTime = ofGetElapsedTimeMillis();

    
}

//--------------------------------------------------------------
void plaats_origine::update(){
    durTime = ofGetElapsedTimeMillis() - initTime;

    
    
    // just for now to show the end of an interactive event can be triggered by time
    // this should be the last thing you do in an update!!!!!!!
//    if (durTime > 10000) {
//        endOfMini();
//    }
}

//--------------------------------------------------------------
void plaats_origine::draw(){
    ofBackground(100, 200, 20);
    
    ofSetColor(255, 0, 0);
    ofFill();
    ofRect(main->client->getLWidth()-30, main->tv1pos, main->tv2pos, 100);
}

//--------------------------------------------------------------
void plaats_origine::endOfMini(){
    main->done();
}