#include "testApp.h"

#define RECONNECT_TIME 400

testApp::testApp()
{
    handler     = new allHandler(&client, &reader);
    gui         = new myGUI(&client, handler);
}

//--------------------------------------------------------------
void testApp::setup(){
	// initialize app
	ofSetFrameRate(30);
    ofEnableSmoothing();
    ofEnableAlphaBlending();
	ofSetBackgroundAuto(false);
    ofBackground(50, 50, 50);

	client.setup("mpe_settings.xml", this);
    
    string appNameList[5] = {"left","middle","right","obj 01", "obj 02"};
    appName = appNameList[client.getID()];
    
    handler->setup(appName);
    
    // GUI give appname and start ofListener for triggering buildGUI
    gui->setup(appName);
    
	// start client
    client.start();
    
    // set the random seed (MPE thing)
	ofSeedRandom(1);
    
    // read out the directory and check if all files are correct
    reader.setup(appName);
    reader.readDir();
    
    handler->createList(); // setting up our total list of apps and movies
    
    if (appName == "left" || appName == "right") {
        syphonServer.setName(appName);
        syphonOut = true;
    } else {
        syphonOut = false;
    }
    
    firstFrameEvent = true;
    fpsCounter = 0;
    outputString = "";
    nextCounter = 0;
    
    appFbo.allocate(client.getLWidth(), client.getLHeight(),GL_RGB);
}

//--------------------------------------------------------------
void testApp::update(){
    
}

//--------------------------------------------------------------
void testApp::draw(){
    
}

//--------------------------------------------------------------
void testApp::frameEvent() {

    // checkin if all parts of the XML file we created of our directory are right, sending parts out that haven't been checked yet
    for (int i = 0; i < reader.partXML.size(); i++) {
        if (reader.partXML[i].checked == false) {
            client.broadcast("checkXML," + appName + "," + ofToString(i) + "," + reader.partXML[i].part);
            i = 1000; // suppose no one would like to see things done this way, but I justa like bea meself ;-)
        }
    }
    
    // clear the screen
    ofBackground(50, 50, 50);
    ofSetColor(255, 255, 255);
    
    handler->update();
    
    // some clearing needed for syphon output
    if (syphonOut) {
        // Clear with alpha, so we can capture via syphon and composite elsewhere should we want.
        glClearColor(0.0, 0.0, 0.0, 0.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
    
    // handle video/app playing stuff
        
    if (syphonOut && handler->list[handler->activeID].type.compare("app") == 0) {
        ofClear(255,255,255, 0);
        appFbo.begin();
    }
        
    handler->draw();

    if (syphonOut && handler->list[handler->activeID].type.compare("app") == 0) {
        appFbo.end();
        ofSetColor(255, 255, 255);
        appFbo.draw(0,0);
    }
    
    // send out a syphon feed, should only be possible for left and right app
    if (syphonOut) {
        //syphonServer.publishScreen();
        
        if (handler->list[handler->activeID].type.compare("mov") == 0) {
            syphonServer.publishTexture(&handler->player->players[handler->player->activeVid].vid.getTextureReference());
        } else if(handler->list[handler->activeID].type.compare("app") == 0) {
            syphonServer.publishTexture(&appFbo.getTextureReference());
        }
        
    }
    
    handleMessages(); // handle all messages from the MPE client
    
    // sending FPS value over the network to all cients every 200 milisec
    if ((ofGetElapsedTimeMillis() - fpsCounter ) > 200) {
        client.broadcast("FRAMERATE," + ofToString(client.getID()) + "," +  ofToString(ofGetFrameRate()) );
        fpsCounter = ofGetElapsedTimeMillis();
    }
    
    // sending values from player to gui, later change this to variable binding
    gui->chapCurPercent->setValue(handler->player->chapCurPercent);
    gui->chapTotalTime->setLabel("TOTAL: " + ofToString(handler->player->chapTotalTime) + " SEC");
    gui->totalPercent->setValue(handler->player->totalProgress);
    gui->playBtn->setValue(handler->player->isPlaying);
    gui->pauseBtn->setValue(!handler->player->isPlaying);
    //gui->outputFrame->setTextString(outputString);
    
    // this for loop sets the buttons true or false each time,
    for (int i = 0; i < handler->list.size(); i++) {
        gui->listBtn[i].btn->setValue(handler->list[i].active);
    }
}

//--------------------------------------------------------------
void testApp::handleMessages(){
    // read any incoming messages
    if (client.messageAvailable()) {
        vector<string> msg = client.getDataMessage();
        vector<string> splitMsg = ofSplitString(msg[0], ",");
        
        // will be captured further on, annoying to keep seeing these messages..
        if (splitMsg[0].compare("FRAMERATE") == 0) {
            // do nothing
        } else {
            printf("MESSAGE: %s\n",splitMsg[0].c_str());
        }
        
        // read directory of chapterHandler
        if (splitMsg[0].compare("readDir") == 0) {
            reader.readDir();
        }
        
        // checking if the XML we've written is the same everywhere, otherwise it means not all directories are the same
        if (splitMsg[0].compare("checkXML") == 0) {
            if (splitMsg[3].compare(reader.partXML[ofToInt(splitMsg[2])].part) != 0) {
                // the (ridiculous) long space after the sentence send to the outputframe is because otherwise it won't show...
                outputString = "In app " + splitMsg[1] + " zijn niet alle bestanden correct!                              .";
                gui->outputFrame->setTextString(outputString);
            } else {
                //printf("all files in ORDER!\n");
            }
            reader.partXML[ofToInt(splitMsg[2])].checked = true;
        }
        
        // just to fire of a red colored ofBackground to check if we're still happy
        if (splitMsg[0].compare("shoot") == 0) {
            ofBackground(255, 0, 0);
        }
        
        // play control
        if (splitMsg[0].compare("play") == 0) {
            handler->resume();
        }
        
        // pause control
        if (splitMsg[0].compare("pause") == 0) {
            handler->pause();
        }
        
        // prev control
        if (splitMsg[0].compare("prev") == 0) {            
            handler->startPrev();
        }
        
        // next control
        if (splitMsg[0].compare("next") == 0) {
            if ((ofGetElapsedTimeMillis() - nextCounter ) > 100) {
                handler->startNext();
                nextCounter = ofGetElapsedTimeMillis();
            }
                
        }
                
        // play a certain item from the allHandler list
        if (splitMsg[0].compare("handlerStart") == 0) {
            printf("play: %s - %s\n",splitMsg[1].c_str(),splitMsg[2].c_str());
            handler->start(splitMsg[1]); 
        }
        
        // turn playAll on or off, second value is the false/true
        if (splitMsg[0].compare("playAll") == 0) {
            if (ofToInt(splitMsg[1]) == 1) {
                handler->bPlayAll = true;
                printf("playAll: TRUE\n");
            } else if(ofToInt(splitMsg[1]) == 0) {
                handler->bPlayAll = false;
                printf("playAll: FALSE\n");
            }
            gui->playAllBtn->setValue(ofToInt(splitMsg[1]));
        }
        
        // turn syphon on or off, second value is the false/true
        if (splitMsg[0].compare("syphonLa") == 0) {
            if (appName == "left" && ofToInt(splitMsg[1]) == 1) {
                syphonOut = true;
                printf("syphonLA: TRUE\n");
            } else if(appName == "left" && ofToInt(splitMsg[1]) == 0) {
                syphonOut = false;
                printf("syphonLA: FALSE\n");
            }
            gui->syphonLaBtn->setValue(ofToInt(splitMsg[1]));
        }
        
        // turn syphon on or off, second value is the false/true
        if (splitMsg[0].compare("syphonRa") == 0) {
            if (appName == "right" && ofToInt(splitMsg[1]) == 1) {
                syphonOut = true;
                printf("syphonRA: TRUE\n");
            } else if(appName == "right" && ofToInt(splitMsg[1]) == 0) {
                syphonOut = false;
                printf("syphonRA: FALSE\n");
            }
            gui->syphonRaBtn->setValue(ofToInt(splitMsg[1]));
        }
        
        // setting framerate for all apps
        if (splitMsg[0].compare("FRAMERATE") == 0) {
            //printf("CLIENT ID: %s FPS: %s\n",splitMsg[1].c_str(), splitMsg[2].c_str());
            
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
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    switch (key) {
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