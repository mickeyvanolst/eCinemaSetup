#pragma once

#ifndef _TEST_APP
#define _TEST_APP

#include "ofMain.h"
#include "mpeClientTCP.h"
#include "chapterHandler.h"
#include "ofxUI.h"

class testApp : public ofBaseApp, public mpeClientListener {
    
public:
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
    
    // handling chapters, reading the files and see whats in it
    handleChapters reader;
    
    // MPE stuff, need some sort of setup command after first contact
    void frameEvent();
    
    string appNameList[5];
    string appName;
    
    // GUI stuff
    void setGUI1();
	void setGUI2();
	void setGUI3();
    void exit();
	
	ofxUICanvas *gui1;
	ofxUICanvas *gui2;
	ofxUICanvas *gui3;
    
    void buildGUI(int & i);
    void guiEvent(ofxUIEventArgs &e);
    
    bool hideGUI;
    float red, green, blue;
    
    
private:
    mpeClientTCP  client;
};

#endif
