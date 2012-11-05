#include "testApp.h"

#define RECONNECT_TIME 400

//--------------------------------------------------------------
void testApp::setup(){
	// initialize app
	ofSetFrameRate(30);
    ofEnableSmoothing();
    ofEnableAlphaBlending();
	ofSetBackgroundAuto(false);
    
	client.setup("settings.xml", this);
    
    // video stuff
    vid.loadMovie("ForGodsSake_3eEDit.mp4");
    vid.stop();
    playVid = false;
    
    // set the random seed
	ofSeedRandom(1);
    
    // add a "randomly" placed ball
    Ball* ball = new Ball(ofRandom(0, client.getMWidth()), ofRandom(0, client.getMHeight()), client.getMWidth(), client.getMHeight());
    balls.push_back(ball);
    
	// start client
    client.start();
    
    // setup OSC
    receiver.setup(9000);
    accPos = 0;
    movPos = 0;
}

//--------------------------------------------------------------
void testApp::update(){
    vid.idleMovie();

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
}

//--------------------------------------------------------------
void testApp::draw(){
    
}

//--------------------------------------------------------------
void testApp::frameEvent() {
    // clear the screen
    ofBackground(255, 255, 255);
    
    ofSetColor(255, 255, 255);
    vid.draw(movPos,0);
    
    // move and draw all the balls
    for (int i = 0; i < balls.size(); i++) {
        balls[i]->calc();
        balls[i]->draw();
    }
    
    // read any incoming messages
    if (client.messageAvailable()) {
        vector<string> msg = client.getDataMessage();
        vector<string> splitMsg = ofSplitString(msg[0], ",");
        
        if(splitMsg[0].compare("xy") == 0) {
            float x = ofToInt(splitMsg[1]);
            float y = ofToInt(splitMsg[2]);
            Ball* ball = new Ball(x, y, client.getMWidth(), client.getMHeight());
            balls.push_back(ball);
        }
        
        if(splitMsg[0].compare("togglePlay") == 0) {
            bool playVidCmd = ofToBool(splitMsg[1]);
            
            if (playVidCmd) {
                vid.play();
            } else {
                vid.stop();
            }
        }
        
        if(splitMsg[0].compare("acc") == 0) {
            float getAcc = ofToFloat(splitMsg[1]);
            movPos = ofMap(getAcc, 0, 1, -200, 200);
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
    // never include a ":" when broadcasting your message
    x += client.getXoffset();
    y += client.getYoffset();
    client.broadcast("xy," + ofToString(x) + "," + ofToString(y));
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
    
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){
    
}
