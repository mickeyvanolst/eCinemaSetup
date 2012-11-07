#include "testApp.h"

#define RECONNECT_TIME 400

//--------------------------------------------------------------
void testApp::setup(){
	// initialize app
	ofSetFrameRate(30);
    ofEnableSmoothing();
    ofEnableAlphaBlending();
	ofSetBackgroundAuto(false);

	client.setup("mpe_settings.xml", this);
    
    string appNameList[5] = {"left","middle","right","obj 01", "obj 02"};
    appName = appNameList[client.getID()];
    
    // set the random seed
	//ofSeedRandom(1);
    
	// start client
    client.start();
    
    // gets triggered if GUI needs to be (re)build after directory reading
    ofAddListener(reader.buildGUIEvent, this, &testApp::buildGUI);
    
    // read out the directory and check if all files are correct
    reader.setup(appName);
    reader.readDir();
    
    // give the readers vector object to an init of the customPlayer, so he has the list of video's
    reader.players.setup(appName);
    
}

//--------------------------------------------------------------
void testApp::update(){

}

//--------------------------------------------------------------
void testApp::draw(){
    
}

//--------------------------------------------------------------
void testApp::frameEvent() {
    
    // clear the screen
    ofBackground(50, 50, 50);
    
    ofSetColor(255, 255, 255);
    
    // read any incoming messages
    if (client.messageAvailable()) {
        vector<string> msg = client.getDataMessage();
        vector<string> splitMsg = ofSplitString(msg[0], ",");
        
        printf("MESSAGE: %s\n",splitMsg[0].c_str());
        
        
        // commands that should be done on every app should be received here
        if (splitMsg[0].compare("readDir") == 0) {
            reader.readDir();
        }
        
    }
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    switch (key) {
        case 's':
            reader.writeXML();
            break;
        case 'r':
            reader.readDir();
            break;
            
        case 'f':
			ofToggleFullscreen();
			break;
            
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
void testApp::keyReleased(int key){
    
    
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
    
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
    
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button) {
    
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
    
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void testApp::guiEvent(ofxUIEventArgs &e)
{
	string name = e.widget->getName();
	int kind = e.widget->getKind();
	cout << "got event from: " << name << endl;
    
	if(name == "SCAN FOLDER")
	{
		ofxUIButton *button = (ofxUIButton *) e.widget;
        // trigger reader readDir function
        //reader.readDir();
        client.broadcast("readDir,1");
	}
    
    // custom event listeners for our chapter buttons
    for (int i = 0; i < reader.chapters.size(); i++) {
        if(reader.chapters[i].inOrder && reader.chapters[i].complete) {
            
            if(name == reader.chapters[i].name)
            {
                ofxUIButton *button = (ofxUIButton *) e.widget;
                // trigger a video
                if(appName == "left") {
                    //player.addChapter(reader.chapters[i].left.file, true);
                    client.broadcast("addChapter," + ofToString(reader.chapters[i].left.file));
                }
                if(appName == "middle") {
                    //player.addChapter(reader.chapters[i].middle.file, true);
                    client.broadcast("addChapter," + ofToString(reader.chapters[i].middle.file));
                }
                if(appName == "right") {
                    //player.addChapter(reader.chapters[i].right.file, true);
                    client.broadcast("addChapter," + ofToString(reader.chapters[i].right.file));
                }
            }
            
        }
    }
    
    
//	if(name == "RED")
//	{
//		ofxUISlider *slider = (ofxUISlider *) e.widget;
//		cout << "RED " << slider->getScaledValue() << endl;
//		red = slider->getScaledValue();
//	}
//	else if(name == "GREEN")
//	{
//		ofxUISlider *slider = (ofxUISlider *) e.widget;
//		cout << "GREEN " << slider->getScaledValue() << endl;
//		green = slider->getScaledValue();
//	}
//	
//	else if(name == "BLUE")
//	{
//		ofxUISlider *slider = (ofxUISlider *) e.widget;
//		cout << "BLUE " << slider->getScaledValue() << endl;
//		blue = slider->getScaledValue();
//	}
//	else if(name == "DRAW GRID")
//	{
//		ofxUIButton *button = (ofxUIButton *) e.widget;
//	}
//	else if(name == "D_GRID")
//	{
//		ofxUIToggle *toggle = (ofxUIToggle *) e.widget;
//	}
//    else if(name == "TEXT INPUT")
//    {
//        ofxUITextInput *textinput = (ofxUITextInput *) e.widget;
//        if(textinput->getTriggerType() == OFX_UI_TEXTINPUT_ON_ENTER)
//        {
//            cout << "ON ENTER: ";
//            //            ofUnregisterKeyEvents((testApp*)this);
//        }
//        else if(textinput->getTriggerType() == OFX_UI_TEXTINPUT_ON_FOCUS)
//        {
//            cout << "ON FOCUS: ";
//        }
//        else if(textinput->getTriggerType() == OFX_UI_TEXTINPUT_ON_UNFOCUS)
//        {
//            cout << "ON BLUR: ";
//            //            ofRegisterKeyEvents(this);
//        }
//        string output = textinput->getTextString();
//        cout << output << endl;
//    }
	
	
	
}

//--------------------------------------------------------------
void testApp::exit()
{
	delete gui1;
	delete gui2;
	delete gui3;
}

//--------------------------------------------------------------
void testApp::buildGUI(int & i){
    printf("build GUI\n");
    // setup GUI
    setGUI1();
    setGUI2();
    setGUI3();
    
    gui1->setDrawBack(false);
    gui2->setDrawBack(false);
    gui3->setDrawBack(false);
}

void testApp::setGUI1()
{
	red = 233; blue = 52; green = 27;
	
	float dim = 16;
	float xInit = OFX_UI_GLOBAL_WIDGET_SPACING;
    float length = 255-xInit;
	hideGUI = false;
    
	gui1 = new ofxUICanvas(0, 0, length+xInit, ofGetHeight());
    
    // app name
	gui1->addWidgetDown(new ofxUILabel(appName, OFX_UI_FONT_LARGE));
    
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
    
	ofAddListener(gui1->newGUIEvent,this,&testApp::guiEvent);
}

void testApp::setGUI2()
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
    
    for (int i = 0; i < reader.chapters.size(); i++) {
        if(reader.chapters[i].inOrder && reader.chapters[i].complete) {
            gui2->addLabelButton(reader.chapters[i].name, false, length-xInit);
        } else {
            gui2->addLabelButton("Err. " + reader.chapters[i].name, false, length-xInit);
        }
    }

	ofAddListener(gui2->newGUIEvent,this,&testApp::guiEvent);
}

void testApp::setGUI3()
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
    gui3->addWidgetDown(new ofxUILabel("INTERVIEW SYNCER", OFX_UI_FONT_MEDIUM));
    gui3->addLabelButton("OPEN INTERVIEW FILES", false, length-xInit);
    gui3->addLabelButton("SYNCHRONISE!", false, length-xInit);
    
    gui3->addSpacer(length-xInit, 2);
    gui3->addWidgetDown(new ofxUILabel("MESSAGE OUTPUT", OFX_UI_FONT_MEDIUM));
    
	ofAddListener(gui3->newGUIEvent,this,&testApp::guiEvent);
}