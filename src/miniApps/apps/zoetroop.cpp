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
void zoetroop::init(mainMini *_mai, ofxMidiOut *_midi){
    main = _mai;
    midiOut = _midi;
}

//--------------------------------------------------------------
void zoetroop::setup(){
    initTime = ofGetElapsedTimeMillis();
    
    ofBackground(255, 225, 255);
    
    //    if (main->appName == "left") {
    //        normImgs.loadSequence("app_content/zoetroop/Zoe_Int_Left");
    //    } else if(main->appName == "middle") {
    //        normImgs.loadSequence("app_content/zoetroop/Zoe_Int_Middle");
    //    } else if(main->appName == "right") {
    //        normImgs.loadSequence("app_content/zoetroop/Zoe_Int_Right");
    //    }

    
    slowImgs.loadSequence("app_content/zoetroop/archief/dog_img");
    normImgs.loadSequence("app_content/zoetroop/archief/horse_img");
    fastImgs.loadSequence("app_content/zoetroop/archief/cheetah_img");
    
	slowImgs.preloadAllFrames();	//this way there is no stutter when loading frames
    normImgs.preloadAllFrames();
    fastImgs.preloadAllFrames();
    
	//slowImgs.setFrameRate(10); //set to ten frames per second
    
    tTV = *main->totalTv1pos + *main->totalTv2pos;
    ptTV = tTV;
    ptTV = tTVmod;
    
    fastTime = 0;
}

//--------------------------------------------------------------
void zoetroop::update(){
    durTime = ofGetElapsedTimeMillis() - initTime;
    Tweener.update();
    
    combTvPos = (*main->totalTv1pos + *main->totalTv2pos)*1.5;
    
    Tweener.addTween(tTV, combTvPos, 8,  &ofxTransitions::easeOutSine);
    tTVmod = tTV - sortaModulo(360, tTV);
    
    if (*main->bMidi && main->appName == "left") {
        //midiOut->sendNoteOn(1,45, tTV1);
        //cout << "midi!\n";
    }
    
    //cout << (tTV - ptTV) << "\n";
    
    // just for now to show the end of an interactive event can be triggered by time
    // this should be the last thing you do in an update!!!!!!!
    if (fastTime > 200) {
        cout << "yaaay\n";
        endOfMini();
    }
}

//--------------------------------------------------------------
void zoetroop::draw(){
    ofBackground(200, 200, 20);
    
    float curPercent = ofMap(tTVmod, 0, 360.0, 0, 1, true);
    
    if (curPercent > 1.0) {
        curPercent = 1.0;
    } else if(curPercent < 0) {
        curPercent = 0;
    }
    
    if (tTVmod - ptTV > 28) {
        fastImgs.getFrameAtPercent(curPercent)->draw(0, 0);
        fastTime++;
    } else if(tTVmod - ptTV > 20) {
        normImgs.getFrameAtPercent(curPercent)->draw(0, 0);
    } else {
        slowImgs.getFrameAtPercent(curPercent)->draw(0, 0);
    }

    
    //normImgs.getFrameAtPercent(curPercent)->draw(0, 0);
    bool visualTest = true;
    if (visualTest) {
        ofSetColor(255, 0, 0);
        ofPushMatrix();
        ofTranslate(150, 150);
        ofRotate(tTV);
        ofRect(-25, -25, 50, 50);
        ofPopMatrix();
        
        ofPushMatrix();
        ofTranslate(250, 150);
        ofRotate(float(*main->totalTv1pos));
        ofRect(-25, -25, 50, 50);
        ofPopMatrix();
        
        ofPushMatrix();
        ofTranslate(350, 150);
        ofRotate(*main->totalTv2pos);
        ofRect(-25, -25, 50, 50);
        ofPopMatrix();
    }
    
    ptTV = tTVmod;
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