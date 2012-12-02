//
//  01_TestMini.h
//  eCinemaSetup
//
//  Created by Mick van Olst on 13-11-12.
//
//

#ifndef eCinemaSetup__1_maker_kijkeri_h
#define eCinemaSetup__1_maker_kijker_h

#include "mainMini.h"
#include "ofxTweener.h"

class maker_kijker {
public:
    maker_kijker(mainMini *_mai);
    void setup();
    void update();
    void draw();
    void endOfMini();
    
    ofQTKitPlayer glitchMov;
    float tGl;
    
    string afterChapter;
    
    mainMini *main;
private:
    int initTime;
    int durTime;
};

#endif
