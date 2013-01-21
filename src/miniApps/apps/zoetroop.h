//
//  zoetroop.h
//  eCinemaSetup
//
//  Created by Mick van Olst on 03-12-12.
//
//

#ifndef eCinemaSetup_zoetroop_h
#define eCinemaSetup_zoetroop_h

#include "mainMini.h"
#include "ofxTweener.h"
#include "ofxImageSequence.h"
#include "ofxOsc.h"

class zoetroop {
public:
    zoetroop();
    
    void init(mainMini *_mai, ofxOscSender * _osc, ofxOscSender *_rpiOutA, ofxOscSender *_rpiOutB);
    void setup();
    void update();
    void draw();
    void endOfMini();
    void exit();
    
    string afterChapter;
    
    ofxOscSender *oscOut;
    mainMini *main;
    
    ofxOscSender *rpiOutA;
    ofxOscSender *rpiOutB;
private:
    
    ofxXmlSettings XML;
    
    long int initTime;
    long int durTime;
    
    long int combTvPos;
    
    ofVideoPlayer normMov;
    ofVideoPlayer fastMov;
    
    int fastTime;
    
    float tTV;
    float prevTTV;
    float tTVmod;
    float ptTV;
    
    float prevSpeedPer;
    
    float tweenTo;
};

#endif
