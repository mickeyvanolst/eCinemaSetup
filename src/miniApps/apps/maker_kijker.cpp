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
    
    tTV1 = 0;
    tv1Mov.setPixelFormat(OF_PIXELS_RGB);
    //glitchMov.loadMovie("app_content/maker_kijker/glitch.mov", decodeMode);
    tv1Mov.loadMovie("content/01_introductie/L_Introductie.MOV", decodeMode);
    tv1Mov.play();

    tTV2 = 0;
    tv2Mov.setPixelFormat(OF_PIXELS_RGB);
    //glitchMov.loadMovie("app_content/maker_kijker/glitch.mov", decodeMode);
    tv2Mov.loadMovie("app_content/maker_kijker/glitch.mov", decodeMode);
    tv2Mov.play();
    
    // setting viewport rectangle
	viewportTV1.x = 0;
	viewportTV1.y = 0;
	viewportTV1.width = 300;
	viewportTV1.height = main->client->getLHeight();
    
    viewportTV2.x = 0;
	viewportTV2.y = 0;
	viewportTV2.width = 300;
	viewportTV2.height = main->client->getLHeight();
}

//--------------------------------------------------------------
void maker_kijker::update(){
    durTime = ofGetElapsedTimeMillis() - initTime;
    
    Tweener.update();
    
    tv1Mov.update();
    Tweener.addTween(tTV1, main->totalTv1pos, 5);
    
    float numRot1Val = 0;
    while (numRot1Val + 360 < main->totalTv1pos) {
        numRot1Val += 360;
    }

    tTV1 = tTV1 - numRot1Val;
    viewportTV1.x = ofMap(tTV1, 0, 360, 0, main->client->getMWidth()) - main->client->getXoffset();

    
    
    tv2Mov.update();
    Tweener.addTween(tTV2, main->totalTv2pos, 5);
    
    float numRot2Val = 0;
    while (numRot2Val + 360 < main->totalTv2pos) {
        numRot2Val += 360;
    }
    
    tTV2 = tTV2 - numRot2Val;
    viewportTV2.x = ofMap(tTV2, 0, 360, 0, main->client->getMWidth()) - main->client->getXoffset();
    
    
    
    // just for now to show the end of an interactive event can be triggered by time
    // this should be the last thing you do in an update!!!!!!!
//    if (durTime > 10000) {
//        endOfMini();
//    }
}

//--------------------------------------------------------------
void maker_kijker::draw(){
    ofBackground(200, 200, 20);
    

    
    // keep a copy of your viewport and transform matrices for later
	ofPushView();
    
	// tell OpenGL to change your viewport. note that your transform matrices will now need setting up
	ofViewport(viewportTV1);
    
	// setup transform matrices for normal oF-style usage, i.e.
	//  0,0=left,top
	//  ofGetViewportWidth(),ofGetViewportHeight()=right,bottom
	ofSetupScreen();
    
    if (tv1Mov.isLoaded()) {
        ofSetColor(255, 255, 255);
        //tv1Mov.draw(ofMap(tTV1, 0, 360, 0, main->client->getMWidth()) * -1, 0);
        tv1Mov.draw((ofMap(tTV1, 0, 360, 0, main->client->getMWidth()) * -1) + main->client->getXoffset(), 0);
    }
    
	// restore the old viewport (now full view and oF coords)
	ofPopView();
	//--
    
    
    
    // keep a copy of your viewport and transform matrices for later
	ofPushView();
    
	// tell OpenGL to change your viewport. note that your transform matrices will now need setting up
	ofViewport(viewportTV2);
    
	// setup transform matrices for normal oF-style usage, i.e.
	//  0,0=left,top
	//  ofGetViewportWidth(),ofGetViewportHeight()=right,bottom
	ofSetupScreen();
    
    if (tv2Mov.isLoaded()) {
        ofSetColor(255, 255, 255);
        tv2Mov.draw((ofMap(tTV2, 0, 360, 0, main->client->getMWidth()) * -1) + main->client->getXoffset(), 0);
    }
    
	// restore the old viewport (now full view and oF coords)
	ofPopView();
	//--
    
    
    
}

//--------------------------------------------------------------
bool maker_kijker::viewInRange(){
    
}

//--------------------------------------------------------------
void maker_kijker::endOfMini(){
    main->done();
}