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
    myGUI();
    
    void init(mpeClientTCP *_cli, allHandler *_all, float *_tv1rotVal, float *_tv2rotVal, float *_tv1rotTotVal, float *_tv2rotTotVal, string *_oscOutIp, int *_oscOutPort);
    void setup(string appID);
    void keyPressed(int key);
    
    void buildGUI(int & i);
    void guiEvent(ofxUIEventArgs &e);

    void setGUI1();
	void setGUI2();
	void setGUI3();
    void exit();
    void windowResized(int w, int h);
    
    ofxUICanvas *gui1;
	ofxUIScrollableCanvas *gui2;
	ofxUICanvas *gui3;
    
    bool hideGUI;
    
    mpeClientTCP    *client;
    allHandler      *handler;
    customPlayer    *player;
    
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
    ofxUIToggle     *syphonMaBtn;
    ofxUIToggle     *syphonRaBtn;
    
    ofxUIToggle     *oscOutBtn;
    ofxUIToggle     *ignoreOscBtn;
    
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
    
    float *tv1rotVal;
    float *tv2rotVal;
    float *tv1rotTotVal;
    float *tv2rotTotVal;
    
    string *oscOutIp;
    int *oscOutPort;
    
    ofxUISlider     *chapCurPercent;
    float           chapCurTime;
    ofxUILabel      *chapTotalTime;
    
    ofxUISlider     *fpsLaSlider;
    ofxUISlider     *fpsMaSlider;
    ofxUISlider     *fpsRaSlider;
    
    ofxUITextArea   *outputFrame;
    
    // interview syncer still to be made
    
    
private:
    
};

#endif
