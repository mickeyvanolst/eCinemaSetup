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
    
//    ofQTKitDecodeMode decodeMode = OF_QTKIT_DECODE_PIXELS_AND_TEXTURE;
//    
//    slowMov.setPixelFormat(OF_PIXELS_RGB);
//    slowMov.loadMovie("app_content/zoetroop/dog.mov", decodeMode);
//    //slowMov.setLoopState(true);
//    slowMov.play();
//    
//    normMov.setPixelFormat(OF_PIXELS_RGB);
//    normMov.loadMovie("app_content/zoetroop/horse.mov", decodeMode);
//    //normMov.setLoopState(true);
//    normMov.play();
//    
//    fastMov.setPixelFormat(OF_PIXELS_RGB);
//    fastMov.loadMovie("app_content/zoetroop/cheetah.mov", decodeMode);
//    //fastMov.setLoopState(true);
//    fastMov.play();
    
    slowImgs.loadSequence("app_content/zoetroop/dog_img");
    normImgs.loadSequence("app_content/zoetroop/horse_img");
    fastImgs.loadSequence("app_content/zoetroop/cheetah_img");
    
	//sequence.loadSequence("frame", "png", 1, 11, 2);
	slowImgs.preloadAllFrames();	//this way there is no stutter when loading frames
    normImgs.preloadAllFrames();
    fastImgs.preloadAllFrames();
    
	//slowImgs.setFrameRate(10); //set to ten frames per second for Muybridge's horse.
    
    
}

//--------------------------------------------------------------
void zoetroop::update(){
    durTime = ofGetElapsedTimeMillis() - initTime;
    tweener.update();
//
//    slowMov.update();
//    normMov.update();
//    fastMov.update();
//    
//    slowMov.setSpeed(ofMap(main->tv1speed, 0, 30, 0.5, 1.5));
//    normMov.setSpeed(ofMap(main->tv1speed, 0, 30, 0.5, 1.5));
//    fastMov.setSpeed(ofMap(main->tv1speed, 0, 30, 0.5, 1.5));
//    
    // just for now to show the end of an interactive event can be triggered by time
    // this should be the last thing you do in an update!!!!!!!
    //    if (durTime > 10000) {
    //        endOfMini();
    //    }
}

//--------------------------------------------------------------
void zoetroop::draw(){
    ofBackground(200, 200, 20);
//    
//    if (main->tv1speed >=0 && main->tv1speed < 10) {
//        slowMov.draw(0,0);
//    } else if(main->tv1speed >=10 && main->tv1speed < 20) {
//        normMov.draw(0,0);
//    } else if(main->tv1speed >=20) {
//        fastMov.draw(0,0);
//    }
    
    float percent = ofMap(main->tv1pos, 0, 360, 0, 1.0, true);
    
    if (main->tv1speed >=0 && main->tv1speed < 15) {
        slowImgs.getFrameAtPercent(percent)->draw(0, 0);
    } else if(main->tv1speed >=15 && main->tv1speed < 30) {
        normImgs.getFrameAtPercent(percent)->draw(0, 0);
    } else if(main->tv1speed >=30) {
        fastImgs.getFrameAtPercent(percent)->draw(0, 0);
    }
}

//--------------------------------------------------------------
void zoetroop::endOfMini(){
    main->done();
}