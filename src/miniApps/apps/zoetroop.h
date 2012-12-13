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
    
    ofxImageSequence slowImgs;
    ofxImageSequence normImgs;
    ofxImageSequence fastImgs;
    
    float tTV1;
    float ptTV1;
    
    float tweenTo;
};

#endif
