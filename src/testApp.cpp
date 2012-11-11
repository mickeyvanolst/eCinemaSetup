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
    
    syphonServer.setName(appName);
    
    firstFrameEvent = true;
    fpsCounter = 0;
    
    syphonOut = false;

}

//--------------------------------------------------------------
void testApp::update(){
    
}

//--------------------------------------------------------------
void testApp::draw(){
    
}

//--------------------------------------------------------------
void testApp::frameEvent() {
    // add all stuff here that should be communicated right after contact
    
    if (firstFrameEvent) {
//        printf("sending xml string: %s\n",reader.totalXmlString.c_str());
//        client.broadcast("XML," + reader.totalXmlString);
        
        if (appName == "left") {
            client.broadcast("syphonLaOn,1");
        } else if(appName == "right") {
            client.broadcast("syphonRaOn,1");
        }
        
        firstFrameEvent = false;
    }    
    
    // clear the screen
    ofBackground(50, 50, 50);
    ofSetColor(255, 255, 255);
    
    if (syphonOut) {
        // Clear with alpha, so we can capture via syphon and composite elsewhere should we want.
        glClearColor(0.0, 0.0, 0.0, 0.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
    
    // handle video playing stuff
    player->draw(client.getXoffset(),client.getYoffset(), client.getLWidth(), client.getLHeight());
    
    if (syphonOut) {
        syphonServer.publishScreen();
    }
    
    // sending FPS value over the network to all cients every 200 milisec
    if ((ofGetElapsedTimeMillis()- fpsCounter ) > 200) {
        client.broadcast("FRAMERATE," + ofToString(client.getID()) + "," +  ofToString(ofGetFrameRate()) );
        fpsCounter = ofGetElapsedTimeMillis();
    }
    
    // read any incoming messages
    if (client.messageAvailable()) {
        vector<string> msg = client.getDataMessage();
        vector<string> splitMsg = ofSplitString(msg[0], ",");
        
        printf("MESSAGE: %s\n",splitMsg[0].c_str());
        
        // commands that should be done on every app should be received here
        if (splitMsg[0].compare("readDir") == 0) {
            reader.readDir();
        }
        
        // checking if the XML we've written is the same everywhere, otherwise it means not all directories are the same
        if (splitMsg[0].compare("XML") == 0) {
            printf("XML: %s\n",splitMsg[1].c_str());
        }
        
        // just to fire of a red colored ofBackground to check if we're still happy
        if (splitMsg[0].compare("shoot") == 0) {
            ofBackground(255, 0, 0);
        }
        
        if (splitMsg[0].compare("play") == 0) {
            player->playPlayer();
        }
        
        if (splitMsg[0].compare("pause") == 0) {
            player->pausePlayer();
        }
        
        if (splitMsg[0].compare("prev") == 0) {
            player->prevPlayer();
        }
        
        if (splitMsg[0].compare("next") == 0) {
            player->nextPlayer();
        }
        
        if (splitMsg[0].compare("playChapter") == 0) {
            printf("play chapter: %s\n",splitMsg[1].c_str());
            printf("chapter id nr: %s\n",splitMsg[2].c_str());
            player->startPlayer(ofToInt(splitMsg[2]));
        }
        
        if (splitMsg[0].compare("syphonLaOn") == 0) {
            if (appName == "left") {
                syphonOut = true;
            }
            gui->syphonLaBtn->setState(true);
        }
        
        if (splitMsg[0].compare("syphonLaOff") == 0) {
            if (appName == "left") {
                syphonOut = false;
            }
            gui->syphonLaBtn->setState(false);
        }
        
        if (splitMsg[0].compare("syphonRaOn") == 0) {
            if (appName == "right") {
                syphonOut = true;
            }
            gui->syphonRaBtn->setState(true);
        }
        
        if (splitMsg[0].compare("syphonRaOff") == 0) {
            if (appName == "right") {
                syphonOut = false;
            }
            gui->syphonRaBtn->setState(false);
        }
        
        if (splitMsg[0].compare("FRAMERATE") == 0) {
            printf("CLIENT ID: %s FPS: %s\n",splitMsg[1].c_str(), splitMsg[2].c_str());
            
            // setting value to the right fps slider
            switch (ofToInt(splitMsg[1])) {
                case 0:
                    gui->fpsLaSlider->setValue(ofToInt(splitMsg[2]));
                    break;
                case 1:
                    gui->fpsMaSlider->setValue(ofToInt(splitMsg[2]));
                    break;
                case 2:
                    gui->fpsRaSlider->setValue(ofToInt(splitMsg[2]));
                    break;
                case 3:
                    gui->fps01Slider->setValue(ofToInt(splitMsg[2]));
                    break;
                case 4:
                    gui->fps02Slider->setValue(ofToInt(splitMsg[2]));
                    break;
                    
                default:
                    break;
            }
            
        }
    }
    // sending values from player to gui, should not have to be done this way...
    gui->chapCurPercent->setValue(player->chapCurPercent);
    gui->chapTotalTime->setLabel("TOTAL: " + ofToString(player->chapTotalTime) + " SEC");
    gui->totalPercent->setValue(player->totalProgress);
    
    gui->playBtn->setValue(player->isPlaying);
    gui->pauseBtn->setValue(!player->isPlaying);
    
    // this for loop sets the buttons true or false each time,
    //still have to put this somehwere it doesn't happen every frame
    for (int i = 0; i < gui->chapBtn.size(); i++) {
        if(player->activeVid == i+1){
            gui->chapBtn[i].btn->setValue(true);
            //printf("button %i true!!\n",i);
        } else {
            gui->chapBtn[i].btn->setValue(false);
            //printf("button %i not true\n",i);
        }
    }
    
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    switch (key) {
        case 's':
            //reader.writeXML();
            break;
        case 'r':
            //reader.readDir();
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