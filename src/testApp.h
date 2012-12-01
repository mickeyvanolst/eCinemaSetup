#pragma once

#ifndef _TEST_APP
#define _TEST_APP

#include "ofMain.h"
#include "mpeClientTCP.h"
#include "ofxSyphon.h"
#include "chapterHandler.h"
#include "allHandler.h"
#include "ofxOsc.h"
#include "myGUI.h"


class testApp : public ofBaseApp, public mpeClientListener {
    
public:
    testApp();
    void setup();
    void update();
    void draw();
    
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y);
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void handleMessages();
    
    // MPE stuff, need some sort of setup command after first contact
    void frameEvent();
    
    string appNameList[5];
    string appName;
    
    long int nextCounter;
    
    // needed for touch osc to simulate our interactive objects
    ofxOscReceiver receiver;
    
    ofFbo appFbo;
    
    // was private, now public!
    mpeClientTCP client;
    
    // handling chapters, reading the files and see whats in it
    handleChapters reader;
    
    // allHandler, deals with both apps and video's
    allHandler *handler;
    
    ofxSyphonServer syphonServer;
    bool syphonOut;
    bool drawScreen;
    
    long int fpsCounter;
    
    bool firstFrameEvent;
    
    // GUI stuff
    myGUI *gui;
    string outputString;
    
private:
    
};

#endif
