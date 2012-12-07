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

class zoetroop {
public:
    zoetroop(mainMini *_mai);
    void setup();
    void update();
    void draw();
    void endOfMini();
    
    ofQTKitPlayer slowMov;
    ofQTKitPlayer normMov;
    ofQTKitPlayer fastMov;
    
    ofxTweener tweener;
    
    string afterChapter;
    
    mainMini *main;
private:
    long int initTime;
    long int durTime;
};

#endif
