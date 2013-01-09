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
void zoetroop::init(mainMini *_mai, ofxOscSender *_osc){
    main = _mai;
    oscOut = _osc;
}

//--------------------------------------------------------------
void zoetroop::setup(){
    initTime = ofGetElapsedTimeMillis();
    ofClear(255, 255, 255);
    ofBackground(0, 0, 0);
    
    if(XML.loadFile("app_content/zoetroop/loadSettings.xml")){
        XML.pushTag("settings");
            normMov.loadMovie("app_content/zoetroop/" + XML.getValue(main->appName + ":intro", "", 0));
            fastMov.loadMovie("app_content/zoetroop/" + XML.getValue(main->appName + ":repeat", "", 0));
        XML.popTag();
    } else {
        cout << "no loadSettings.xml found..\n";
    }
        
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
    tTVmod = tTV - main->sortaModulo(360, tTV);
    
    normMov.update();
    fastMov.update();
    
    // just for now to show the end of an interactive event can be triggered by time
    // this should be the last thing you do in an update!!!!!!!
    if (fastTime > 200 && int(*main->totalTv1pos) != int(main->totalTv1prevPos) && int(*main->totalTv2pos) != int(main->totalTv2prevPos)) {
        cout << "yaaay\n";
        endOfMini();
    }
}

//--------------------------------------------------------------
void zoetroop::draw(){
    ofBackground(0, 0, 0);
    
    float curPercent = ofMap(tTVmod, 0, 360.0, 0, 1, true);
    
    if (curPercent > 1.0) {
        curPercent = 1.0;
    } else if(curPercent < 0) {
        curPercent = 0;
    }
    
    if (tTV - ptTV > 25) {
        fastMov.setPosition(curPercent);
        fastMov.draw(main->client->getXoffset(), 0);
        fastTime++;
    } else {
        normMov.setPosition(curPercent);
        normMov.draw(main->client->getXoffset(), 0);
    }
    
    
    bool visualTest = false;
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
    
    ptTV = tTV;
}

//--------------------------------------------------------------
void zoetroop::exit(){
    
}

//--------------------------------------------------------------
void zoetroop::endOfMini(){
    main->done();
}