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
    
    // video stuff
    vid.loadMovie("F1.MOV");
    vid.stop();
    playVid = false;
    
    // set the random seed
	ofSeedRandom(1);
    
	// start client
    client.start();
    
    // setup OSC
    //receiver.setup(9000);
}

//--------------------------------------------------------------
void testApp::update(){
    
    vid.idleMovie();
    
    // OSC is not needed at this time
    /*
     while(receiver.hasWaitingMessages()){
     // get the next message
     ofxOscMessage m;
     receiver.getNextMessage(&m);
     if(m.getAddress() == "/1/fader5"){///accxyz
     //printf("%f\n", m.getArgAsFloat(0));
     accPos = m.getArgAsFloat(0);
     client.broadcast("acc," + ofToString(accPos));
     }
     printf("%s\n",m.getAddress().c_str());
     }
     */
    
    
}

//--------------------------------------------------------------
void testApp::draw(){
    
}

//--------------------------------------------------------------
void testApp::frameEvent() {
    // clear the screen
    ofBackground(255, 255, 255);
    
    ofSetColor(255, 255, 255);
    vid.draw(0,0);
    
    // read any incoming messages
    if (client.messageAvailable()) {
        vector<string> msg = client.getDataMessage();
        vector<string> splitMsg = ofSplitString(msg[0], ",");
        
        if(splitMsg[0].compare("togglePlay") == 0) {
            bool playVidCmd = ofToBool(splitMsg[1]);
            
            if (playVidCmd) {
                vid.play();
            } else {
                vid.stop();
            }
        }
    }
    
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    switch (key) {
        case ' ':
            playVid = !playVid;
            client.broadcast("togglePlay," + ofToString(playVid));
            break;
        case 's':
            reader.writeXML();
            break;
        case 'r':
            reader.readDir();
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
