#pragma once

#ifndef _TEST_APP
#define _TEST_APP

#include "ofMain.h"
#include "mpeClientTCP.h"
#include "ofxSyphon.h"
#include "chapterHandler.h"
#include "allHandler.h"
#include "ofxOsc.h"
#include "ofxMidi.h"
#include "myGUI.h"

class testApp : public ofBaseApp, public mpeClientListener {
public:
    testApp();
    void setup();
    void update();
    void draw();
    void exit();
    
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y);
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void handleMessages();
    
    // MIDI for the audio connection (on a nother laptop)
    ofxMidiOut midiOut;
	int midiChannel;
    
    // MPE's way of doing an update/draw, everything updates from here
    void frameEvent();
    
    //int global = singleton::Instance();
    
    // was private, now public!
    mpeClientTCP        client; 
    // allHandler, deals with both apps and video's
    allHandler          handler;
    // handleChapters, reading the files and see whats in it, also writes to XML
    handleChapters      reader;
    // GUI stuff
    myGUI               gui;
    // needed for touch osc (iPad) to simulate our interactive objects
    ofxOscReceiver      receiver;
    // we draw into this and send the Fbo trough syphon
    ofFbo               appFbo;
    ofxSyphonServer     syphonServer;
    
    string appNameList[5];
    string appName;
    
    string outputString; // string connected to the output window of the GUI
    
    long int fpsCounter; // every app may only send fps once every 200ms
    long int nextCounter; // counter so the next button doesn't get triggerd by every app that thinks it's time to move to another chapter
    
    bool syphonOut;
    bool drawScreen;
    
private:
    
};

#endif
