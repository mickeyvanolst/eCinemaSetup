#pragma once

#ifndef _TEST_APP
#define _TEST_APP

#include "ofMain.h"
#include "mpeClientTCP.h"
#include "chapterHandler.h"
#include "customPlayer.h"
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
    
    // MPE stuff, need some sort of setup command after first contact
    void frameEvent();
    
    string appNameList[5];
    string appName;
    
    // was private, now public!
    mpeClientTCP  client;
    
    // handling chapters, reading the files and see whats in it
    handleChapters reader;
    
    // our custom videoPlayer, loads all movies and then decides which
    // to draw and play, also handles syphon
    customPlayer *player;
    
    // GUI stuff
    myGUI *gui;
    
private:
    
};

#endif
