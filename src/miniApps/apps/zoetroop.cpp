//
//  zoetroop.cpp
//  eCinemaSetup
//
//  Created by Mick van Olst on 03-12-12.
//
//

#include "zoetroop.h"

//--------------------------------------------------------------
zoetroop::zoetroop(){
    
}

//--------------------------------------------------------------
void zoetroop::init(mainMini *_mai){
    main = _mai;
}

//--------------------------------------------------------------
void zoetroop::setup(){
    initTime = ofGetElapsedTimeMillis();
    
    ofBackground(255, 225, 255);
    
    slowImgs.loadSequence("app_content/zoetroop/dog_img");
    normImgs.loadSequence("app_content/zoetroop/horse_img");
    fastImgs.loadSequence("app_content/zoetroop/cheetah_img");
    
	//sequence.loadSequence("frame", "png", 1, 11, 2);
	slowImgs.preloadAllFrames();	//this way there is no stutter when loading frames
    normImgs.preloadAllFrames();
    fastImgs.preloadAllFrames();
    
	//slowImgs.setFrameRate(10); //set to ten frames per second for Muybridge's horse.
    
    tTV1 = main->totalTv1pos;
    ptTV1 = tTV1;
}

//--------------------------------------------------------------
void zoetroop::update(){
    durTime = ofGetElapsedTimeMillis() - initTime;
    Tweener.update();
    
    
    Tweener.addTween(tTV1, main->totalTv1pos, 8,  &ofxTransitions::easeOutSine);
    //Tweener.addTween(tTV1, main->totalTv1pos, 8);
    tTV1 = tTV1 - sortaModulo(360, tTV1);
    
    
    // just for now to show the end of an interactive event can be triggered by time
    // this should be the last thing you do in an update!!!!!!!
//    if (durTime > 10000) {
//        endOfMini();
//    }
}

//--------------------------------------------------------------
void zoetroop::draw(){
    ofBackground(200, 200, 20);
    
    float curPercent = ofMap(tTV1, 0, 360.0, 0, 1, true);
    
    if (curPercent > 1.0) {
        curPercent = 1.0;
    } else if(curPercent < 0) {
        curPercent = 0;
    }
    
    cout << "total TV1: " << main->totalTv1pos << "\n";
    
//    printf("tTV1: %f\n",tTV1);
//    printf("ptTV1: %f\n",ptTV1);
    
    if (tTV1 - ptTV1 > 18) {
        fastImgs.getFrameAtPercent(curPercent)->draw(0, 0);
    } else if(tTV1 - ptTV1 > 12) {
        normImgs.getFrameAtPercent(curPercent)->draw(0, 0);
    } else {
        slowImgs.getFrameAtPercent(curPercent)->draw(0, 0);
    }

    ofSetColor(255, 0, 0);
    
    ofPushMatrix();
    ofTranslate(150, 150);
    ofRotate(tTV1);
    ofRect(-25, -25, 50, 50);
    ofPopMatrix();
    
    ptTV1 = tTV1;
}

//--------------------------------------------------------------
void zoetroop::endOfMini(){
    main->done();
}

//--------------------------------------------------------------
float zoetroop::sortaModulo(float timesFit, float totalVal){
    float returnVal = 0.0;
    while (returnVal + timesFit < totalVal) {
        returnVal += timesFit;
    }
    return returnVal;
}