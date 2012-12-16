//
//  interview_bezoekers.h
//  eCinemaSetup
//
//  Created by Mick van Olst on 16-12-12.
//
//

#ifndef eCinemaSetup_interview_bezoekers_h
#define eCinemaSetup_interview_bezoekers_h

#include "mainMini.h"
#include "ofxMidi.h"
#include "ofxXmlSettings.h"

class interview_bezoekers {
public:
    interview_bezoekers();
    
    void init(mainMini *_mai, ofxMidiOut *_midi);
    void setup();
    void update();
    void draw();
    void endOfMini();
    void loadXML(string file, bool printResult);
    void exit();
    
    string afterChapter;
    
    ofxMidiOut *midiOut;
    mainMini *main;
    
private:
    void drawViewPorts(int numPorts);
    
    int initTime;
    int durTime;
    
    int curVid;
    int rotateVideos;
    
    ofDirectory           files;
    ofxXmlSettings        XML;
    
    struct smartClip {
        ofVideoPlayer   vid;
        ofRectangle     viewport;
        bool            onScreen;
        int             inFrame;
        int             outFrame;
        string          filename;
    };
    vector<smartClip> videos;
};


#endif
