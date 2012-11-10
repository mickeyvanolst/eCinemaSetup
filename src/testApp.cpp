#include "testApp.h"

#define RECONNECT_TIME 400

testApp::testApp()
{
    player      = new customPlayer(&reader);
    gui         = new myGUI(&client, &reader);
    
}

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
    
    // gui give appname and start ofListner for triggering buildGUI
    gui->setup(appName);
    
    // customp player, also needs to know who he is
    player->setup(appName);
    
	// start client
    client.start();
    
    // read out the directory and check if all files are correct
    reader.setup(appName);
    reader.readDir();
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
        
        // just to fire of a red colored ofBackground to check if we're still happy
        if (splitMsg[0].compare("shoot") == 0) {
            ofBackground(255, 0, 0);
        }
        
        if (splitMsg[0].compare("playChapter") == 0) {
            printf("play chapter: %s\n",splitMsg[1].c_str());
            printf("chapter id nr: %s\n",splitMsg[2].c_str());
            player->startPlayer(ofToInt(splitMsg[2]));
        }
    }
    
    // handle video playing stuff
    player->draw(client.getXoffset(),client.getYoffset(), client.getLWidth(), client.getLHeight());
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

        default:
            break;
    }
    
    gui->keyPressed(key); // so we can toggle visibility
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