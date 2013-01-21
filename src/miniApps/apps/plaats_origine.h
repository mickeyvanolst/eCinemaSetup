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
#include "ofxOsc.h"
#include "ofxXmlSettings.h"

class plaats_origine {
public:
    plaats_origine();
    
    void init(mainMini *_mai, ofxOscSender *_osc, ofxOscSender *_rpiOutA, ofxOscSender *_rpiOutB);
    void setup();
    void update();
    void draw();
    void endOfMini();
    bool isImageInViewport(int x, int w);
    void exit();
    
    string afterChapter;
    
    ofxOscSender *oscOut;
    mainMini *main;
    
    ofxOscSender *rpiOutA;
    ofxOscSender *rpiOutB;
private:
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

