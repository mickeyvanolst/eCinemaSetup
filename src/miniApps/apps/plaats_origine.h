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
//#include "ofxThreadedImageLoader.h"
#include "plaats_origine.h"
#include "ofxTweener.h"

class plaats_origine {
public:
    plaats_origine(mainMini *_mai);
    void setup();
    void update();
    void draw();
    void endOfMini();
    void scaleByHeight(int oW, int oH, int *nW, int nH);
    bool isImageInViewport(int x, int w);
    /*
    ofDirectory files;

    ofxThreadedImageLoader loader;
    vector<ofImage*> artImg;
    vector<ofImage*> worldImg;
    */
    
    ofQTKitPlayer bgMov;
    ofQTKitPlayer artMov;
    
    float tBg;
    float tArt;
    
    string afterChapter;
    
    mainMini *main;
private:
    int initTime;
    int durTime;
};

#endif

