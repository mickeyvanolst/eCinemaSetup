//
//  intentie_interpretatie.h
//  eCinemaSetup
//
//  Created by Mick van Olst on 14-12-12.
//
//

#ifndef eCinemaSetup_intentie_interpretatie_h
#define eCinemaSetup_intentie_interpretatie_h

#include "mainMini.h"
#include "ofxMidi.h"

class intentie_interpretatie {
public:
    intentie_interpretatie();
    
    void init(mainMini *_mai, ofxMidiOut * _midi);
    void setup();
    void update();
    void draw();
    void scaleByHeight(int oW, int oH, int *nW, int nH);
    void endOfMini();
    
    string afterChapter;
    
    ofxMidiOut *midiOut;
    mainMini *main;
    
private:
    bool allIsWell;
    
    long int initTime;
    long int durTime;
    
    ofVideoPlayer Vid1;
    ofVideoPlayer Vid2;
    
    ofRectangle viewport1;
    ofRectangle viewport2;
};

#endif
