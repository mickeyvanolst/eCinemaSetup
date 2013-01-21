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
void zoetroop::init(mainMini *_mai, ofxOscSender *_osc, ofxOscSender *_rpiOutA, ofxOscSender *_rpiOutB){
    main = _mai;
    oscOut = _osc;
    rpiOutA = _rpiOutA;
    rpiOutB = _rpiOutB;
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
        
    if (main->bOsc) {
        ofxOscMessage rA;
        rA.setAddress("/chapter");
        rA.addIntArg(2);
        rpiOutA->sendMessage(rA);
        
        ofxOscMessage rB;
        rB.setAddress("/chapter");
        rB.addIntArg(2);
        rpiOutB->sendMessage(rB);
    }
    
    tTV = *main->totalTv1pos + *main->totalTv2pos;
    ptTV = tTV;
    ptTV = tTVmod;
    
    fastTime = 0;
    initTime = ofGetElapsedTimeMillis();
}

//--------------------------------------------------------------
void zoetroop::update(){
    durTime = ofGetElapsedTimeMillis() - initTime;
    Tweener.update();
    
    combTvPos = (*main->totalTv1pos + *main->totalTv2pos)*1.5;
    cout << "combTvPos: " << combTvPos << "\n";
    prevTTV = tTV;
    Tweener.addTween(tTV, combTvPos, 8,  &ofxTransitions::easeOutSine);
//    bool negativeTVmod = false;
//    if (tTVmod < 0) {
//        negativeTVmod = true;
//        tTVmod = tTVmod * -1;
//    }
    cout << "tTV: " << tTV << "\n";
    tTVmod = tTV - main->sortaModulo(360, tTV);
//    if (negativeTVmod) {
//        tTVmod = tTVmod * -1;
//    }
    
    normMov.update();
    fastMov.update();
    
    if (main->bOsc) {
        ofxOscMessage m;
        float speedPer;
        
        speedPer = ofMap(tTV - ptTV, 0, 30, 0.0, 100.0);
        Tweener.addTween(speedPer, speedPer, 0.5,  &ofxTransitions::easeOutSine);
        
        if (speedPer < 0.0) {
            speedPer = speedPer * -1;
        }
        if (speedPer > 100.0) {
            speedPer = 100.0;
        }
        
        m.setAddress("/zoetroop");
        m.addFloatArg(speedPer);
        oscOut->sendMessage(m);
        
        if (durTime > 200) {
            initTime = ofGetElapsedTimeMillis();
            ofxOscMessage rA;
            rA.setAddress("/zo/position");
            rA.addIntArg(speedPer);
            rpiOutA->sendMessage(rA);
            
            ofxOscMessage rB;
            rB.setAddress("/zo/position");
            rB.addIntArg(speedPer);
            rpiOutB->sendMessage(rB);
        }
        

        
        prevSpeedPer = speedPer;
    }
    
    // just for now to show the end of an interactive event can be triggered by time
    // this should be the last thing you do in an update!!!!!!!
    //if (fastTime > 200 && int(*main->totalTv1pos) != int(main->totalTv1prevPos) && int(*main->totalTv2pos) != int(main-/>totalTv2prevPos)) {
       // cout << "yaaay\n";
      //  endOfMini();
    //}
}

//--------------------------------------------------------------
void zoetroop::draw(){
    ofBackground(0, 0, 0);
    cout << "tTVmod: " << tTVmod << "\n";
    if (tTVmod < 0) {
        // weird stuff going on here.. just tying to do things right, backwards doesn't handle very well without this
        if ((tTVmod * -1) > 360.0) {
            tTVmod = tTVmod - main->sortaModulo(360, tTVmod*-1);
            tTVmod = 360.0 - tTVmod;
            tTVmod = tTVmod - main->sortaModulo(360, tTVmod);
        } else {
            tTVmod = 360.0 - (tTVmod * -1);
        }
        cout << "micktry tTVmod: " << tTVmod << "\n";
    }
    
    
    float curPercent = ofMap(tTVmod, 0, 360.0, 0, 1, true);
//    if (curPercent > 1.0) {
//        curPercent = 1.0;
//    } else if(curPercent < 0) {
//        curPercent = 0.0;
//    }
    if (tTV - ptTV > 25) {
        fastMov.setPosition(curPercent);
        fastMov.draw(main->client->getXoffset(), 0);
        fastTime++;
    } else {
        cout << "curPercent: " << curPercent << "\n";
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