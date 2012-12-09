//
//  myGUI.h
//  eCinemaSetup
//
//  Created by Mick van Olst on 08-11-12.
//
//

#ifndef eCinemaSetup_myGUI_h
#define eCinemaSetup_myGUI_h

#include "ofxUI.h"
#include "mpeClientTCP.h"
#include "chapterHandler.h"
#include "allHandler.h"
#include "miniHandler.h"

class myGUI {

public:
    myGUI(mpeClientTCP *_cli, allHandler *_all);

    void setup(string appID);
    void keyPressed(int key);
    
    void buildGUI(int & i);
    void guiEvent(ofxUIEventArgs &e);

    void setGUI1();
	void setGUI2();
	void setGUI3();
    void exit();
    
    void resetRotation();
    
    ofxUICanvas *gui1;
	ofxUICanvas *gui2;
	ofxUICanvas *gui3;
    
    bool hideGUI;
    
    mpeClientTCP *client;
    allHandler *handler;
    
    string appName;
    
    // needed for a check if things are send double
    string      prevMsg;
    bool        prevState;
    long int    prevMsgCounter;
    
    // All needed values
    ofxUISlider     *totalPercent;
    
    ofxUIToggle     *drawLaBtn;
    ofxUIToggle     *drawRaBtn;
    
    ofxUIToggle     *middleFsBtn;
    ofxUIToggle     *tv1FsBtn;
    ofxUIToggle     *tv2FsBtn;
    
    ofxUIToggle     *syphonLaBtn;
    ofxUIToggle     *syphonRaBtn;
    
    int             activeChap;
    
    struct BtnList {
        string              label;
        ofxUILabelButton    *btn;
    };
    vector<BtnList> listBtn;
    
    ofxUIButton     *prevBtn;
    ofxUIButton     *pauseBtn;
    ofxUIButton     *playBtn;
    ofxUIButton     *nextBtn;

    ofxUIToggle     *playAllBtn;
    
    ofxUIRotarySlider   *tv1rot;
    ofxUIRotarySlider   *tv2rot;
    
    float tv1rotVal;
    float tv2rotVal;
    float tv1rotTotVal;
    float tv2rotTotVal;
    
    ofxUISlider     *chapCurPercent;
    float           chapCurTime;
    ofxUILabel      *chapTotalTime;
    
    ofxUISlider     *fpsLaSlider;
    ofxUISlider     *fpsMaSlider;
    ofxUISlider     *fpsRaSlider;
    ofxUISlider     *fps01Slider;
    ofxUISlider     *fps02Slider;
    
    ofxUITextArea   *outputFrame;
    
    // interview syncer still to be made
    
    
private:
    
};

#endif
