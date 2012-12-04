//
//  zoetroop.cpp
//  eCinemaSetup
//
//  Created by Mick van Olst on 03-12-12.
//
//

#include "zoetroop.h"

//--------------------------------------------------------------
zoetroop::zoetroop(mainMini *_mai){
    main = _mai;
}

//--------------------------------------------------------------
void zoetroop::setup(){
    initTime = ofGetElapsedTimeMillis();
    
    ofBackground(255, 225, 255);
    
    ofQTKitDecodeMode decodeMode = OF_QTKIT_DECODE_PIXELS_AND_TEXTURE;
    
    slowMov.setPixelFormat(OF_PIXELS_RGB);
    slowMov.loadMovie("app_content/zoetroop/dog.mov", decodeMode);
    //slowMov.setLoopState(true);
    slowMov.play();
    
    normMov.setPixelFormat(OF_PIXELS_RGB);
    normMov.loadMovie("app_content/zoetroop/horse.mov", decodeMode);
    //normMov.setLoopState(true);
    normMov.play();
    
    fastMov.setPixelFormat(OF_PIXELS_RGB);
    fastMov.loadMovie("app_content/zoetroop/cheetah.mov", decodeMode);
    //fastMov.setLoopState(true);
    fastMov.play();
}

//--------------------------------------------------------------
void zoetroop::update(){
    durTime = ofGetElapsedTimeMillis() - initTime;
    tweener.update();
    
    slowMov.update();
    normMov.update();
    fastMov.update();
    
    // just for now to show the end of an interactive event can be triggered by time
    // this should be the last thing you do in an update!!!!!!!
    //    if (durTime > 10000) {
    //        endOfMini();
    //    }
}

//--------------------------------------------------------------
void zoetroop::draw(){
    ofBackground(200, 200, 20);
    
    
    
}

//--------------------------------------------------------------
void zoetroop::endOfMini(){
    main->done();
}