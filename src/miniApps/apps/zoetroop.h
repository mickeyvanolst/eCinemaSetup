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
    
    void init(mainMini *_mai, ofxOscSender * _osc);
    void setup();
    void update();
    void draw();
    void endOfMini();
    void exit();
    
    string afterChapter;
    
    ofxOscSender *oscOut;
    mainMini *main;
private:
    
    ofxXmlSettings XML;
    
    long int initTime;
    long int durTime;
    
    long int combTvPos;
    
    ofVideoPlayer normMov;
    ofVideoPlayer fastMov;
    
    int fastTime;
    
    float tTV;
    float tTVmod;
    float ptTV;
    
    float tweenTo;
};

#endif
