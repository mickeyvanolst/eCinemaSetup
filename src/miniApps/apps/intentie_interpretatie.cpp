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
    
    allIsWell = true;
}

//--------------------------------------------------------------
void intentie_interpretatie::setup(){
    initTime = ofGetElapsedTimeMillis();
    ofBackground(255, 225, 255);
    
    if (main->appName == "left") {
        Vid1.loadMovie("app_content/intentie_interpretatie/c1.mov");
        Vid2.loadMovie("app_content/intentie_interpretatie/c2.mov");
    } else if(main->appName == "right") {
        Vid1.loadMovie("app_content/intentie_interpretatie/c3.mov");
        Vid2.loadMovie("app_content/intentie_interpretatie/c4.mov");
    } else {
        allIsWell = false;
    }
    
    Vid1.setLoopState(OF_LOOP_NONE);
    Vid2.setLoopState(OF_LOOP_NONE);
    
    // setting viewport rectangle
	viewport1.x = main->client->getXoffset();
	viewport1.y = main->client->getYoffset();
	viewport1.width = main->client->getLWidth()/2;
	viewport1.height = main->client->getLHeight();
    
    viewport2.x = main->client->getXoffset() + (main->client->getLWidth()/2);
	viewport2.y = main->client->getYoffset();
	viewport2.width = main->client->getLWidth()/2;
	viewport2.height = main->client->getLHeight();
}

//--------------------------------------------------------------
void intentie_interpretatie::update(){
    durTime = ofGetElapsedTimeMillis() - initTime;
    
    Vid1.update();
    Vid2.update();
    
    // just for now to show the end of an interactive event can be triggered by time
    // this should be the last thing you do in an update!!!!!!!
    //    if (durTime > 10000) {
    //        endOfMini();
    //    }
}

//--------------------------------------------------------------
void intentie_interpretatie::draw(){
    if (main->appName != "middle") {
        ofBackground(20, 200, 20);
        int tempWidth = 0;
        scaleByHeight(Vid1.getWidth(), Vid1.getHeight(), &tempWidth, main->client->getLHeight());
        ofPushView();
            ofPushView();
                ofViewport(viewport1);
                ofSetupScreen();
                
                if (Vid1.isLoaded()) {
                    ofSetColor(255, 255, 255);
                    // notice we're not using getXoffset, this is because the viewport gets fucked up then..
                    Vid1.draw(0 - ((tempWidth - viewport1.width)), main->client->getYoffset(), tempWidth, main->client->getMHeight());
                }
            ofPopView();
            //--
            
            ofPushView();
                ofViewport(viewport2);
                ofSetupScreen();
                
                if (Vid2.isLoaded()) {
                    ofSetColor(255, 255, 255);
                    // notice we're not using getXoffset, this is because the viewport gets fucked up then..
                    Vid2.draw(0 - ((tempWidth - viewport2.width)), main->client->getYoffset(), tempWidth, main->client->getMHeight());
                }
            ofPopView();
        ofPopView();
        //--
        

    } else {
        ofBackground(0, 0, 0);
    }
}

//--------------------------------------------------------------
void intentie_interpretatie::scaleByHeight(int oW, int oH, int *nW, int nH){
    //original width / original height x new height = new width
    float newVal = (float(oW) / float(oH)) * float(nH);
    (*nW) = int(newVal);
}

//--------------------------------------------------------------
void intentie_interpretatie::endOfMini(){
    main->done();
}
