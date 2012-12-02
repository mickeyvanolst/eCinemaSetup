//
//  01_TestMini.cpp
//  eCinemaSetup
//
//  Created by Mick van Olst on 13-11-12.
//
//

#include "maker_kijker.h"

//--------------------------------------------------------------
maker_kijker::maker_kijker(mainMini *_mai){
    main = _mai;
}

//--------------------------------------------------------------
void maker_kijker::setup(){
    initTime = ofGetElapsedTimeMillis();
    
    ofBackground(255, 225, 255);
    
    ofQTKitDecodeMode decodeMode = OF_QTKIT_DECODE_PIXELS_AND_TEXTURE;
    
    tGl = 0;
    glitchMov.setPixelFormat(OF_PIXELS_BGRA);
    glitchMov.loadMovie("app_content/maker_kijker/glitch.mov", decodeMode);
    glitchMov.setPaused(true);
}

//--------------------------------------------------------------
void maker_kijker::update(){
    durTime = ofGetElapsedTimeMillis() - initTime;
    
    Tweener.update();
    
    glitchMov.update();
    Tweener.addTween(tGl, main->totalTv1pos, 5);
    
    float numRotVal = 0;
    while (numRotVal + 360 < main->totalTv1pos) {
        numRotVal += 360;
    }
    
    tGl = tGl - numRotVal;
    tGl = ofMap(tGl, 0, 360, 0, 1);
    
    glitchMov.setPosition(tGl);

    // just for now to show the end of an interactive event can be triggered by time
    // this should be the last thing you do in an update!!!!!!!
//    if (durTime > 10000) {
//        endOfMini();
//    }
}

//--------------------------------------------------------------
void maker_kijker::draw(){
    ofBackground(200, 200, 20);

    if (glitchMov.isLoaded()) {
        ofSetColor(255, 255, 255);
        glitchMov.draw(0, 0);
    }
}
//--------------------------------------------------------------
void maker_kijker::endOfMini(){
    main->done();
}