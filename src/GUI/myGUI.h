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
#include "miniHandler.h"

class myGUI {

public:
    myGUI(mpeClientTCP *_cli, handleChapters *_rea, miniHandler *_min);

    void setup(string appID);
    void keyPressed(int key);
    
    void buildGUI(int & i);
    void guiEvent(ofxUIEventArgs &e);

    void setGUI1();
	void setGUI2();
	void setGUI3();
    void exit();
    
    ofxUICanvas *gui1;
	ofxUICanvas *gui2;
	ofxUICanvas *gui3;
    
    bool hideGUI;
    
    mpeClientTCP *client;
    handleChapters *reader;
    miniHandler *miniApp;
    
    string appName;
    
    // needed for a check if things are send double
    string      prevMsg;
    long int    prevMsgCounter;
    
    // All needed values
    ofxUISlider     *totalPercent;
    bool            appLaCon;
    bool            appMaCon;
    bool            appRaCon;
    bool            appObj1Con;
    bool            appObj2Con;
    
    ofxUIToggle     *syphonLaBtn;
    ofxUIToggle     *syphonRaBtn;
    
    int             activeChap;
    
    struct BtnList {
        string              label;
        bool                active;
        bool                isVideo; // either app or video
        ofxUILabelButton    *btn;
    };
    vector<BtnList> listBtn;
    
    ofxUIButton     *prevBtn;
    ofxUIButton     *pauseBtn;
    ofxUIButton     *playBtn;
    ofxUIButton     *nextBtn;

    
    ofxUISlider     *chapCurPercent;
    float           chapCurTime;
    ofxUILabel      *chapTotalTime;
    
    ofxUISlider     *fpsLaSlider;
    ofxUISlider     *fpsMaSlider;
    ofxUISlider     *fpsRaSlider;
    ofxUISlider     *fps01Slider;
    ofxUISlider     *fps02Slider;
    
    ofxUITextArea   *outputFrame;
    
    // obj simulator still to be made
    
    // interview syncer still to be made
    
    
private:
    
};

#endif
