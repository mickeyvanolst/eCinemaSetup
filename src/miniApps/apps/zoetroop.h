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
#include "ofxMidi.h"

class zoetroop {
public:
    zoetroop();
    
    void init(mainMini *_mai, ofxMidiOut * _midi);
    void setup();
    void update();
    void draw();
    void endOfMini();
    
    string afterChapter;
    
    ofxMidiOut *midiOut;
    mainMini *main;
private:
    float sortaModulo(float timesFit, float totalVal);
    
    long int initTime;
    long int durTime;
    
    long int combTvPos;
    
    ofxImageSequence slowImgs;
    ofxImageSequence normImgs;
    ofxImageSequence fastImgs;
    
    int fastTime;
    
    float tTV;
    float tTVmod;
    float ptTV;
    
    float tweenTo;
};

#endif
