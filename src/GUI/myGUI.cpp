//
//  myGUI.cpp
//  eCinemaSetup
//
//  Created by Mick van Olst on 08-11-12.
//
//

#include "myGUI.h"

//--------------------------------------------------------------
myGUI::myGUI(mpeClientTCP *_cli, handleChapters *_rea)
{
    //parent = _par;
    client = _cli;
    reader = _rea;
}

//--------------------------------------------------------------
void myGUI::setup(string appID)
{
    appName = appID;
    
    // yay we can send events from class to class regardless of hiarchie!
    ofAddListener(reader->buildGUIEvent, this, &myGUI::buildGUI);
}

//--------------------------------------------------------------
void myGUI::guiEvent(ofxUIEventArgs &e)
{
	string name = e.widget->getName();
	int kind = e.widget->getKind();
	cout << "got event from: " << name << endl;
    
	if(name == "SCAN FOLDER")
	{
		ofxUIButton *button = (ofxUIButton *) e.widget;
        // trigger reader readDir function
        //reader.readDir();
        client->broadcast("readDir,1");
	}
    
    if(name == "SHOOT")
	{
		ofxUIButton *button = (ofxUIButton *) e.widget;
        // trigger reader readDir function
        //reader.readDir();
        client->broadcast("shoot,1");
	}
    
    // custom event listeners for our chapter buttons
    for (int i = 0; i < reader->chapters.size(); i++) {
        if(reader->chapters[i].inOrder && reader->chapters[i].complete) {
            
            if(name == reader->chapters[i].name)
            {
                ofxUIButton *button = (ofxUIButton *) e.widget;
                // trigger a video
                if(appName == "left") {
                    //player.addChapter(reader.chapters[i].left.file, true);
                
                    client->broadcast("addChapter," + ofToString(reader->chapters[i].left.file));
                }
                if(appName == "middle") {
                    //player.addChapter(reader.chapters[i].middle.file, true);
                    client->broadcast("addChapter," + ofToString(reader->chapters[i].middle.file));
                }
                if(appName == "right") {
                    //player.addChapter(reader.chapters[i].right.file, true);
                    client->broadcast("addChapter," + ofToString(reader->chapters[i].right.file));
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
}

//--------------------------------------------------------------
void myGUI::buildGUI(int & i){
    printf("build GUI\n");
    // setup GUI
    setGUI1();
    setGUI2();
    setGUI3();
    
    gui1->setDrawBack(false);
    gui2->setDrawBack(false);
    gui3->setDrawBack(false);
}

void myGUI::setGUI1()
{
	red = 233; blue = 52; green = 27;
	
	float dim = 16;
	float xInit = OFX_UI_GLOBAL_WIDGET_SPACING;
    float length = 255-xInit;
	hideGUI = false;
    
	gui1 = new ofxUICanvas(0, 0, length+xInit, ofGetHeight());
    
    // app name
	//gui1->addWidgetDown(new ofxUILabel(appName, OFX_UI_FONT_LARGE));
    
    gui1->addSpacer(length-xInit, 2);
	gui1->addWidgetDown(new ofxUILabel("CONNECTIE ALLE APPS", OFX_UI_FONT_MEDIUM));
	gui1->addButton("LA", false, dim, dim);
	gui1->addButton("MA", false, dim, dim);
	gui1->addButton("RA", false, dim, dim);
	gui1->addButton("01", false, dim, dim);
	gui1->addButton("02", false, dim, dim);
    
    gui1->addSpacer(length-xInit, 2);
	gui1->addWidgetDown(new ofxUILabel("SYPHON OUTPUT", OFX_UI_FONT_MEDIUM));
	gui1->addButton("LA", false, dim, dim);
	gui1->addButton("RA", false, dim, dim);
    
    gui1->addSpacer(length-xInit, 2);
    gui1->addWidgetDown(new ofxUILabel("FPS ALLE APPS", OFX_UI_FONT_MEDIUM));
    gui1->addSlider("LA", 0.0, 100.0, red, length-xInit, dim);
    gui1->addSlider("MA", 0.0, 100.0, red, length-xInit, dim);
    gui1->addSlider("RA", 0.0, 100.0, red, length-xInit, dim);
    gui1->addSlider("01", 0.0, 100.0, red, length-xInit, dim);
    gui1->addSlider("02", 0.0, 100.0, red, length-xInit, dim);
    
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
    gui2->addSlider("CURRENT", 0.0, 10.0, red, length-xInit, dim);
    gui2->addWidgetDown(new ofxUILabel("TOTAL: ", OFX_UI_FONT_SMALL));
    
    gui2->addSpacer(length-xInit, 2);
    gui2->addWidgetDown(new ofxUILabel("CHAPTER LIST", OFX_UI_FONT_MEDIUM));
    
    for (int i = 0; i < reader->chapters.size(); i++) {
        if(reader->chapters[i].inOrder && reader->chapters[i].complete) {
            gui2->addLabelButton(reader->chapters[i].name, false, length-xInit);
        } else {
            gui2->addLabelButton("Err. " + reader->chapters[i].name, false, length-xInit);
        }
    }
    
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
	gui3->addSlider("MIN", 0.0, 60.0, red, length-xInit, dim);
    
    gui3->addSpacer(length-xInit, 2);
    gui3->addWidgetDown(new ofxUILabel("INTERACTIVE OBJECTS", OFX_UI_FONT_MEDIUM));
    gui3->addLabelButton("SHOW SIMULATION", false, length-xInit);
    
    gui3->addSpacer(length-xInit, 2);
    gui3->addLabelButton("SCAN FOLDER", false, length-xInit);
    
    gui3->addSpacer(length-xInit, 2);
    gui3->addLabelButton("SHOOT", false, length-xInit);
    
    gui3->addSpacer(length-xInit, 2);
    gui3->addWidgetDown(new ofxUILabel("INTERVIEW SYNCER", OFX_UI_FONT_MEDIUM));
    gui3->addLabelButton("OPEN INTERVIEW FILES", false, length-xInit);
    gui3->addLabelButton("SYNCHRONISE!", false, length-xInit);
    
    gui3->addSpacer(length-xInit, 2);
    gui3->addWidgetDown(new ofxUILabel("MESSAGE OUTPUT", OFX_UI_FONT_MEDIUM));
    
	ofAddListener(gui3->newGUIEvent,this,&myGUI::guiEvent);
}