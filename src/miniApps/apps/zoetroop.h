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

class zoetroop {
public:
    zoetroop(mainMini *_mai);
    void setup();
    void update();
    void draw();
    void endOfMini();
    
    string afterChapter;
    
    mainMini *main;
private:
    long int initTime;
    long int durTime;
    
//    ofQTKitPlayer slowMov;
//    ofQTKitPlayer normMov;
//    ofQTKitPlayer fastMov;
    
    ofxImageSequence slowImgs;
    ofxImageSequence normImgs;
    ofxImageSequence fastImgs;
    
    bool playing;
    
    ofxTweener tweener;
    float tOne;
};

#endif
