//
//  myGUI.cpp
//  eCinemaSetup
//
//  Created by Mick van Olst on 08-11-12.
//
//

#include "myGUI.h"

//--------------------------------------------------------------
myGUI::myGUI(mpeClientTCP *_cli, handleChapters *_rea, miniHandler *_min)
{
    client  = _cli;
    reader  = _rea;
    miniApp = _min;
    
    gui1 = NULL;
    gui2 = NULL;
    gui3 = NULL;
    
}

//--------------------------------------------------------------
void myGUI::setup(string appID)
{
    appName = appID;
    
    // yay we can send events from class to class regardless of hiarchie!
    ofAddListener(reader->buildGUIEvent, this, &myGUI::buildGUI);
    
    prevMsg = "";
    prevMsgCounter = 0;
    
}

//--------------------------------------------------------------
void myGUI::guiEvent(ofxUIEventArgs &e)
{
	string name = e.widget->getName();
	int kind = e.widget->getKind();
    int id = e.widget->getID();
    
    // added a timer so if we get the same command withing 200 millisec we dont use it.
    if (prevMsg == name && (ofGetElapsedTimeMillis() - prevMsgCounter) > 200) {
        prevMsgCounter = ofGetElapsedTimeMillis();
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
//        for (int i = 0; i < reader->chapters.size(); i++) {
//            if(reader->chapters[i].inOrder && reader->chapters[i].complete) {
//                if(name == reader->chapters[i].name) {
//                    ofxUIButton *button = (ofxUIButton *) e.widget;
//                    client->broadcast("playChapter," + ofToString(reader->chapters[i].name) + "," + ofToString(i+1));
//                }
//            }
//        }
        
        // this is where it loops trough all the chapters to see if one has been pressed, including miniApps
        for (int i = 0; i < reader->chapters.size(); i++) {
            if(reader->chapters[i].inOrder && reader->chapters[i].complete) {
                if(name == reader->chapters[i].name) {
                    ofxUIButton *button = (ofxUIButton *) e.widget;
                    client->broadcast("playChapter," + ofToString(reader->chapters[i].name) + "," + ofToString(i+1));
                    for (int l = 0; l < listBtn.size(); l++) {
                        if (name == listBtn[l].label) {
                            listBtn[l].active = true;
                        } else {
                            listBtn[l].active = false;
                        }
                    }
                }
            }
            // checking for miniApps being pressed
            if (name == miniApp->appComesAfter(reader->chapters[i].name)) {
                ofxUIButton *button = (ofxUIButton *) e.widget;
                client->broadcast("playMiniApp," + ofToString(miniApp->appComesAfter(reader->chapters[i].name)));
                for (int l = 0; l < listBtn.size(); l++) {
                    if (name == listBtn[l].label) {
                        listBtn[l].active = true;
                    } else {
                        listBtn[l].active = false;
                    }
                }
            }
        }

    }
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
	gui1->addWidgetDown(new ofxUILabel("CONNECTIE ALLE APPS", OFX_UI_FONT_MEDIUM));
    gui1->addWidgetDown(new ofxUIButton("LA", false, dim, dim));
    gui1->addWidgetRight(new ofxUIButton("MA", false, dim, dim));
    gui1->addWidgetRight(new ofxUIButton("RA", false, dim, dim));
    gui1->addWidgetDown(new ofxUIButton("01", false, dim, dim));
    gui1->addWidgetRight(new ofxUIButton("02", false, dim, dim));
    
    gui1->addSpacer(length-xInit, 2);
	gui1->addWidgetDown(new ofxUILabel("SYPHON OUTPUT", OFX_UI_FONT_MEDIUM));
    syphonLaBtn = (ofxUIToggle *) gui1->addWidgetDown(new ofxUIToggle("LA SYPHON", true, dim, dim));
    syphonRaBtn = (ofxUIToggle *) gui1->addWidgetRight(new ofxUIToggle("RA SYPHON", true, dim, dim));
    
    gui1->addSpacer(length-xInit, 2);
    gui1->addWidgetDown(new ofxUILabel("FPS ALLE APPS", OFX_UI_FONT_MEDIUM));
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
    
//    for (int i = 0; i < reader->chapters.size(); i++) {
//        listBtn.push_back(BtnList());
//        
//        if(reader->chapters[i].inOrder && reader->chapters[i].complete) {
//            //activeChapBtn = (ofxUILabelButton *) gui2->addLabelButton(reader->chapters[i].name, false, length-xInit);
//            listBtn[listBtn.size()-1].btn = (ofxUILabelButton *) gui2->addLabelButton(reader->chapters[i].name, false, length-xInit);
//        } else {
//            //gui2->addLabelButton("Err. " + reader->chapters[i].name, false, length-xInit);
//            listBtn[listBtn.size()-1].btn = (ofxUILabelButton *) gui2->addLabelButton(reader->chapters[i].name, false, length-xInit);
//        }        
//    }

    for (int i = 0; i < reader->chapters.size(); i++) {
        listBtn.push_back(BtnList());
        
        if(reader->chapters[i].inOrder && reader->chapters[i].complete) {
            listBtn[listBtn.size()-1].isVideo = true;
            listBtn[listBtn.size()-1].label = reader->chapters[i].name;
            listBtn[listBtn.size()-1].btn = (ofxUILabelButton *) gui2->addLabelButton(reader->chapters[i].name, false, length-xInit);
        } else {
            listBtn[listBtn.size()-1].isVideo = true;
            listBtn[listBtn.size()-1].label = reader->chapters[i].name;
            listBtn[listBtn.size()-1].btn = (ofxUILabelButton *) gui2->addLabelButton("Err." + reader->chapters[i].name, false, length-xInit);
        }
        
        if (miniApp->appComesAfter(reader->chapters[i].name) != "") {
            listBtn.push_back(BtnList());
            listBtn[listBtn.size()-1].isVideo = false;
            listBtn[listBtn.size()-1].label = miniApp->appComesAfter(reader->chapters[i].name);
            listBtn[listBtn.size()-1].btn = (ofxUILabelButton *) gui2->addLabelButton(miniApp->appComesAfter(reader->chapters[i].name), false, length-xInit);
        }
    }
    
    gui2->addSpacer(length-xInit, 2);
    prevBtn = (ofxUIImageButton *) gui2->addWidgetDown(new ofxUIImageButton(dim*2, dim*2, false, "GUI/prev.png", "PREV BUTTON"));
    pauseBtn = (ofxUIImageButton *) gui2->addWidgetRight(new ofxUIImageButton(dim*2, dim*2, false, "GUI/pause.png", "PAUSE BUTTON"));
    playBtn = (ofxUIImageButton *) gui2->addWidgetRight(new ofxUIImageButton(dim*2, dim*2, false, "GUI/play.png", "PLAY BUTTON"));
    prevBtn = (ofxUIImageButton *) gui2->addWidgetRight(new ofxUIImageButton(dim*2, dim*2, false, "GUI/next.png", "NEXT BUTTON"));
    
    gui2->addLabelToggle("PLAY ALL", true);
    
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
	gui3->addWidgetDown(new ofxUILabel("TOTALE PROGRESSIE", OFX_UI_FONT_MEDIUM));
	totalPercent = (ofxUISlider *) gui3->addSlider("PROCENT", 0.0, 100.0, 0.0, length-xInit, dim);
    
    gui3->addSpacer(length-xInit, 2);
    gui3->addWidgetDown(new ofxUILabel("INTERACTIVE OBJECTS", OFX_UI_FONT_MEDIUM));
    gui3->addLabelButton("SHOW SIMULATION", false, length-xInit);
    
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