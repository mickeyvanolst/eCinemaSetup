//
//  myGUI.cpp
//  eCinemaSetup
//
//  Created by Mick van Olst on 08-11-12.
//
//

#include "myGUI.h"

//--------------------------------------------------------------
myGUI::myGUI()
{
    gui1 = NULL;
    gui2 = NULL;
    gui3 = NULL;
    
}

//--------------------------------------------------------------
void myGUI::init(mpeClientTCP *_cli, allHandler *_all)
{
    client      = _cli;
    handler     = _all;
}

//--------------------------------------------------------------
void myGUI::setup(string appID)
{
    appName = appID;
    
    // yay we can send events from class to class regardless of hiarachie!
    ofAddListener(handler->buildGUIEvent, this, &myGUI::buildGUI);
    
    prevMsg = "";
    prevMsgCounter = 0;
    
    tv1rotVal = 0;
    tv2rotVal = 0;
    
    tv1rotTotVal = 0;
    tv2rotTotVal = 0;
}

//--------------------------------------------------------------
void myGUI::guiEvent(ofxUIEventArgs &e)
{
	string name = e.widget->getName();
	int kind = e.widget->getKind();
    int id = e.widget->getID();
    
    // added a timer so if we get the same command within 200 millisec we dont use it.
    if (prevMsg == name && (ofGetElapsedTimeMillis() - prevMsgCounter) > 200) {
        prevMsgCounter = ofGetElapsedTimeMillis();
        prevMsg = "";

        // need to do this becuause for some reason buttons tend to send values too fast even if
        // you use them seconds later..
        
        if(name == "PLAY ALL")
        {
            ofxUIToggle *button = (ofxUIToggle *) e.widget;
            playAllBtn->setValue(!playAllBtn->getValue());
        }
        
        if(name == "LA DRAW")
        {
            ofxUIToggle *button = (ofxUIToggle *) e.widget;
            drawLaBtn->setValue(!drawLaBtn->getValue());
        }
        
        if(name == "RA DRAW")
        {
            ofxUIToggle *button = (ofxUIToggle *) e.widget;
            drawLaBtn->setValue(!drawRaBtn->getValue());
        }
        
        if(name == "MIDDLE FS")
        {
            ofxUIToggle *button = (ofxUIToggle *) e.widget;
            middleFsBtn->setValue(!middleFsBtn->getValue());
        }
        
        if(name == "TV 1 FS")
        {
            ofxUIToggle *button = (ofxUIToggle *) e.widget;
            tv1FsBtn->setValue(!tv1FsBtn->getValue());
        }
        
        if(name == "TV 2 FS")
        {
            ofxUIToggle *button = (ofxUIToggle *) e.widget;
            tv2FsBtn->setValue(!tv2FsBtn->getValue());
        }
        
        if(name == "LA SYPHON")
        {
            ofxUIToggle *button = (ofxUIToggle *) e.widget;
            syphonLaBtn->setValue(!syphonLaBtn->getValue());
        }
        
        if(name == "RA SYPHON")
        {
            ofxUIToggle *button = (ofxUIToggle *) e.widget;
            syphonRaBtn->setValue(!syphonRaBtn->getValue());
        }
        
    } else {
        
        cout << "got event from: " << name << endl;
        prevMsg = name;
        
        if(name == "SCAN FOLDER")
        {
            ofxUIButton *button = (ofxUIButton *) e.widget;
            client->broadcast("readDir,1");
        }
        
        if(name == "SHOOT")
        {
            ofxUIButton *button = (ofxUIButton *) e.widget;
            // trigger reader readDir function
            //reader.readDir();
            client->broadcast("shoot,1");
        }
        
        if(name == "PLAY BUTTON")
        {
            ofxUIButton *button = (ofxUIButton *) e.widget;
            client->broadcast("play,1");
        }
        
        if(name == "PAUSE BUTTON")
        {
            ofxUIButton *button = (ofxUIButton *) e.widget;
            client->broadcast("pause,1");
        }
        
        if(name == "NEXT BUTTON")
        {
            ofxUIButton *button = (ofxUIButton *) e.widget;
            client->broadcast("next,1");
        }
        
        if(name == "PREV BUTTON")
        {
            ofxUIButton *button = (ofxUIButton *) e.widget;
            client->broadcast("prev,1");
        }
        
        if (name == "TV_1_ROT") {
            ofxUIRotarySlider *button = (ofxUIRotarySlider *) e.widget;
            client->broadcast("tv1rot," + ofToString(tv1rot->getValue()));
        }
        
        if (name == "TV_2_ROT") {
            ofxUIRotarySlider *button = (ofxUIRotarySlider *) e.widget;
            client->broadcast("tv2rot," + ofToString(tv2rot->getValue()));
        }
        
        if(name == "PLAY ALL")
        {
            ofxUIToggle *button = (ofxUIToggle *) e.widget;
            if (playAllBtn->getValue() == 1) {
                client->broadcast("playAll,1");
            } else {
                client->broadcast("playAll,0");
            }
        }
        
        if(name == "LA DRAW")
        {
            ofxUIToggle *button = (ofxUIToggle *) e.widget;
            if (drawLaBtn->getValue() == 1) {
                client->broadcast("drawLa,1");
            } else {
                client->broadcast("drawLa,0");
            }
        }
        
        if(name == "RA DRAW")
        {
            ofxUIToggle *button = (ofxUIToggle *) e.widget;
            if (drawRaBtn->getValue() == 1) {
                client->broadcast("drawRa,1");
            } else {
                client->broadcast("drawRa,0");
            }
        }
        
        if(name == "MIDDLE FS")
        {
            ofxUIToggle *button = (ofxUIToggle *) e.widget;
            if (middleFsBtn->getValue() == 1) {
                client->broadcast("middleFs,1");
            } else {
                client->broadcast("middleFs,0");
            }
        }
        
        if(name == "TV 1 FS")
        {
            ofxUIToggle *button = (ofxUIToggle *) e.widget;
            if (tv1FsBtn->getValue() == 1) {
                client->broadcast("tv1Fs,1");
            } else {
                client->broadcast("tv1Fs,0");
            }
        }
        
        if(name == "TV 2 FS")
        {
            ofxUIToggle *button = (ofxUIToggle *) e.widget;
            if (tv2FsBtn->getValue() == 1) {
                client->broadcast("tv1Fs,1");
            } else {
                client->broadcast("tv2Fs,0");
            }
        }
        
        if(name == "LA SYPHON")
        {
            ofxUIToggle *button = (ofxUIToggle *) e.widget;
            if (syphonLaBtn->getValue() == 1) {
                client->broadcast("syphonLa,1");
            } else {
                client->broadcast("syphonLa,0");
            }
        }
        
        if(name == "RA SYPHON")
        {
            ofxUIToggle *button = (ofxUIToggle *) e.widget;
            if (syphonRaBtn->getValue() == 1) {
                client->broadcast("syphonRa,1");
            } else {
                client->broadcast("syphonRa,0");
            }
        }
        
        // custom event listeners for our chapter buttons
        for (int i = 0; i < handler->list.size(); i++) {
            if (name == handler->list[i].name) {
                    ofxUIButton *button = (ofxUIButton *) e.widget;
                    client->broadcast("handlerStart," + ofToString(handler->list[i].name) + "," + ofToString(handler->list[i].type));
            }
        }

    }
}

//--------------------------------------------------------------
void myGUI::resetRotation()
{
    tv1rotVal       = 0;
    tv2rotVal       = 0;
    tv1rotTotVal    = 0;
    tv2rotTotVal    = 0;
    tv1rot->setValue(0);
    tv2rot->setValue(0);
}

//--------------------------------------------------------------
void myGUI::exit()
{
	delete gui1;
	delete gui2;
	delete gui3;
    
    gui1 = NULL;
    gui2 = NULL;
    gui3 = NULL;
}

//--------------------------------------------------------------
void myGUI::keyPressed(int key){
    switch (key) {
            
		case 'h':
            gui1->toggleVisible();
            gui2->toggleVisible();
            gui3->toggleVisible();
			break;
            
        default:
            break;
    }
}

//--------------------------------------------------------------
void myGUI::buildGUI(int & i){
    
    if (gui1 != NULL && gui2 != NULL && gui3 != NULL) {
        exit();
    }
    
    // setup GUI
    setGUI1();
    setGUI2();
    setGUI3();
    
    gui1->setDrawBack(false);
    gui2->setDrawBack(false);
    gui3->setDrawBack(false);
    
    if (appName != "left") {
        gui1->toggleVisible();
        gui2->toggleVisible();
        gui3->toggleVisible();
    }
    printf("build GUI\n");
}

void myGUI::setGUI1()
{	
	float dim = 16;
	float xInit = OFX_UI_GLOBAL_WIDGET_SPACING;
    float length = 255-xInit;
	hideGUI = false;
    
	gui1 = new ofxUICanvas(0, 0, length+xInit, ofGetHeight());
    
	gui1->addWidgetDown(new ofxUILabel(appName, OFX_UI_FONT_LARGE));
    
    gui1->addSpacer(length-xInit, 2);
    gui1->addWidgetDown(new ofxUILabel("DRAW SCREEN", OFX_UI_FONT_MEDIUM));
    drawLaBtn = (ofxUIToggle *) gui1->addWidgetDown(new ofxUIToggle("LA DRAW", true, dim, dim));
    drawRaBtn = (ofxUIToggle *) gui1->addWidgetRight(new ofxUIToggle("RA DRAW", true, dim, dim));

    gui1->addSpacer(length-xInit, 2);
    gui1->addWidgetDown(new ofxUILabel("FULLSCREEN", OFX_UI_FONT_MEDIUM));
    middleFsBtn = (ofxUIToggle *) gui1->addWidgetDown(new ofxUIToggle("MIDDLE FS", false, dim, dim));
    tv1FsBtn = (ofxUIToggle *) gui1->addWidgetDown(new ofxUIToggle("TV 1 FS", false, dim, dim));
    tv2FsBtn = (ofxUIToggle *) gui1->addWidgetDown(new ofxUIToggle("TV 2 FS", false, dim, dim));
    
    gui1->addSpacer(length-xInit, 2);
	gui1->addWidgetDown(new ofxUILabel("SYPHON OUTPUT", OFX_UI_FONT_MEDIUM));
    syphonLaBtn = (ofxUIToggle *) gui1->addWidgetDown(new ofxUIToggle("LA SYPHON", true, dim, dim));
    syphonRaBtn = (ofxUIToggle *) gui1->addWidgetRight(new ofxUIToggle("RA SYPHON", true, dim, dim));
    
    gui1->addSpacer(length-xInit, 2);
    gui1->addWidgetDown(new ofxUILabel("FPS ALL APPS", OFX_UI_FONT_MEDIUM));
    fpsLaSlider = (ofxUISlider *) gui1->addSlider("LA", 0.0, 100.0, 0.0, length-xInit, dim);
    fpsMaSlider = (ofxUISlider *) gui1->addSlider("MA", 0.0, 100.0, 0.0, length-xInit, dim);
    fpsRaSlider = (ofxUISlider *) gui1->addSlider("RA", 0.0, 100.0, 0.0, length-xInit, dim);
    fps01Slider = (ofxUISlider *) gui1->addSlider("01", 0.0, 100.0, 0.0, length-xInit, dim);
    fps02Slider = (ofxUISlider *) gui1->addSlider("02", 0.0, 100.0, 0.0, length-xInit, dim);
    
    gui1->addSpacer(length-xInit, 2);
    gui1->addLabelButton("SHOOT", false, length-xInit);
    
	ofAddListener(gui1->newGUIEvent,this,&myGUI::guiEvent);
}

void myGUI::setGUI2()
{
	float dim = 16;
	float xInit = OFX_UI_GLOBAL_WIDGET_SPACING;
    float length = 255-xInit;
    
    gui2 = new ofxUICanvas(length+xInit+2, 5, length+xInit, ofGetHeight());
	gui2->addWidgetDown(new ofxUILabel("Press 'h' to Hide GUIs", OFX_UI_FONT_LARGE));
    
    gui2->addSpacer(length-xInit, 2);
    gui2->addWidgetDown(new ofxUILabel("CHAPTER CURRENT TIME", OFX_UI_FONT_MEDIUM));
    
    chapCurPercent = (ofxUISlider *) gui2->addSlider("PROCENT", 0.0, 100.0, 0.0, length-xInit, dim);
    chapTotalTime = (ofxUILabel *) gui2->addWidgetDown(new ofxUILabel("TOTAL: ", OFX_UI_FONT_SMALL));
    
    gui2->addSpacer(length-xInit, 2);
    gui2->addWidgetDown(new ofxUILabel("CHAPTER LIST", OFX_UI_FONT_MEDIUM));
    
    // clearing the buttons if they are here
    if(listBtn.size() > 0){
        listBtn.erase(listBtn.begin(), listBtn.end());
        listBtn.clear();
        listBtn.resize(0);
    }
    
    for (int i = 0; i < handler->list.size(); i++) {
        listBtn.push_back(BtnList());
        listBtn[listBtn.size()-1].btn = (ofxUILabelButton *) gui2->addLabelButton(handler->list[i].name, false, length-xInit);
    }
    
    gui2->addSpacer(length-xInit, 2);
    prevBtn = (ofxUIImageButton *) gui2->addWidgetDown(new ofxUIImageButton(dim*2, dim*2, false, "GUI/prev.png", "PREV BUTTON"));
    pauseBtn = (ofxUIImageButton *) gui2->addWidgetRight(new ofxUIImageButton(dim*2, dim*2, false, "GUI/pause.png", "PAUSE BUTTON"));
    playBtn = (ofxUIImageButton *) gui2->addWidgetRight(new ofxUIImageButton(dim*2, dim*2, false, "GUI/play.png", "PLAY BUTTON"));
    prevBtn = (ofxUIImageButton *) gui2->addWidgetRight(new ofxUIImageButton(dim*2, dim*2, false, "GUI/next.png", "NEXT BUTTON"));
    
    playAllBtn = (ofxUIToggle *) gui2->addLabelToggle("PLAY ALL", true);
    
	ofAddListener(gui2->newGUIEvent,this,&myGUI::guiEvent);
}
    
void myGUI::setGUI3()
{
	float dim = 16;
	float xInit = OFX_UI_GLOBAL_WIDGET_SPACING;
    float length = 255-xInit;
	gui3 = new ofxUICanvas(length*2+xInit*2+4, 21, length+xInit, ofGetHeight());
	gui3->addWidgetDown(new ofxUILabel(" ", OFX_UI_FONT_LARGE));
    
    gui3->addSpacer(length-xInit, 2);
	gui3->addWidgetDown(new ofxUILabel("TOTAL PROGRESS", OFX_UI_FONT_MEDIUM));
	totalPercent = (ofxUISlider *) gui3->addSlider("PROCENT", 0.0, 100.0, 0.0, length-xInit, dim);
    
    gui3->addSpacer(length-xInit, 2);
    gui3->addWidgetDown(new ofxUILabel("INTERACTIVE OBJECTS", OFX_UI_FONT_MEDIUM));
    tv1rot = (ofxUIRotarySlider *) gui3->addWidgetDown(new ofxUIRotarySlider(80, 0.0, 360.0, tv1rotVal, "TV_1_ROT"));
    tv2rot = (ofxUIRotarySlider *) gui3->addWidgetRight(new ofxUIRotarySlider(80, 0.0, 360.0, tv2rotVal, "TV_2_ROT"));
    
    gui3->addSpacer(length-xInit, 2);
    gui3->addLabelButton("SCAN FOLDER", false, length-xInit);
    
    gui3->addSpacer(length-xInit, 2);
    gui3->addWidgetDown(new ofxUILabel("INTERVIEW SYNCER", OFX_UI_FONT_MEDIUM));
    gui3->addLabelButton("OPEN INTERVIEW FILES", false, length-xInit);
    gui3->addLabelButton("SYNCHRONISE!", false, length-xInit);
    
    gui3->addSpacer(length-xInit, 2);
    gui3->addWidgetDown(new ofxUILabel("MESSAGE OUTPUT", OFX_UI_FONT_MEDIUM));
    
    gui3->addSpacer(length-xInit, 2);
    outputFrame = (ofxUITextArea *) gui3->addWidgetDown(new ofxUITextArea("OUTPUT FRAME", "", length-xInit, 128));
    
	ofAddListener(gui3->newGUIEvent,this,&myGUI::guiEvent);
}