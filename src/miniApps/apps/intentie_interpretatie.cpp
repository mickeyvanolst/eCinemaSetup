//
//  intentie_interpretatie.cpp
//  eCinemaSetup
//
//  Created by Mick van Olst on 14-12-12.
//
//

#include "intentie_interpretatie.h"
//--------------------------------------------------------------
intentie_interpretatie::intentie_interpretatie(){
    
}

//--------------------------------------------------------------
void intentie_interpretatie::init(mainMini *_mai, ofxMidiOut *_midi){
    main = _mai;
    midiOut = _midi;
}

//--------------------------------------------------------------
void intentie_interpretatie::setup(){
    initTime = ofGetElapsedTimeMillis();
    
    ofBackground(255, 225, 255);
}

//--------------------------------------------------------------
void intentie_interpretatie::update(){
    durTime = ofGetElapsedTimeMillis() - initTime;
    
    // just for now to show the end of an interactive event can be triggered by time
    // this should be the last thing you do in an update!!!!!!!
    //    if (durTime > 10000) {
    //        endOfMini();
    //    }
}

//--------------------------------------------------------------
void intentie_interpretatie::draw(){
    ofBackground(200, 200, 20);
    
}

//--------------------------------------------------------------
void intentie_interpretatie::endOfMini(){
    main->done();
}
