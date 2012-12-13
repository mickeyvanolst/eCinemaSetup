//
//  plaats_origine.h
//  eCinemaSetup
//
//  Created by Mick van Olst on 01-12-12.
//
//

#ifndef eCinemaSetup_plaats_origine_h
#define eCinemaSetup_plaats_origine_h

#include "mainMini.h"
#include "plaats_origine.h"
#include "ofxTweener.h"
#include "ofxMidi.h"
#include "ofxXmlSettings.h"

class plaats_origine {
public:
    plaats_origine();
    
    void init(mainMini *_mai, ofxMidiOut *_midi);
    void setup();
    void update();
    void draw();
    void endOfMini();
    void scaleByHeight(int oW, int oH, int *nW, int nH);
    bool isImageInViewport(int x, int w);
    
    string afterChapter;
    
    ofxMidiOut *midiOut;
    mainMini *main;
private:
    float sortaModulo(float timesFit, float totalVal);
    void loadXML(string file, bool printResult);
    double floor0( double value );
    
    int initTime;
    int durTime;
    
    ofQTKitPlayer bgMov;
    ofQTKitPlayer artMov;
    
    ofxXmlSettings XML;
    
    struct Cuepoints {
        string name;
        int frame;
    };
    
    vector<Cuepoints> artCuePoints;
    vector<Cuepoints> bgCuePoints;
    
    int curArt;
    int curBg;
    
    float tBg;
    float tArt;
    
    string output;
};

#endif

