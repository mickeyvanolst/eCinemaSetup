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
	ofSetBackgroundAuto(false); // otherwise it will flicker during every framedrop
    ofBackground(50, 50, 50);
    
	client.setup("mpe_settings.xml", this);
    
    string appNameList[5] = {"left","middle","right","TV_1", "TV_2"};
    appName = appNameList[client.getID()];
    
    
    handler->setup(appName);
    gui->setup(appName);
    receiver.setup(8000); // OSC
    
    client.start();
    
    // set the random seed if needed (MPE thing)
	//ofSeedRandom(1);
    
    // read out the directory and check if all files are correct
    reader.setup(appName);
    reader.readDir();
    
    handler->createList(); // setting up our total list of apps and movies
    
    // syphon is ON by default at the left and right app
    if (appName == "left" || appName == "right") {
        syphonServer.setName(appName);
        syphonOut = true;
    } else {
        syphonOut = false;
    }
    
    drawScreen = true;
    fpsCounter = 0;
    nextCounter = 0;
    outputString = "";
    
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
    
    // checking if all parts of the XML file we created of our directory are right,
    // sending parts out that haven't been checked yet
    for (int i = 0; i < reader.partXML.size(); i++) {
        if (reader.partXML[i].checked == false) {
            client.broadcast("checkXML," + appName + "," + ofToString(i) + "," + reader.partXML[i].part);
            i = 1000; // suppose no one would like to see things done this way but it works
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
    
    // start capturing in the FBO if there is a miniApp playing
    if (syphonOut && handler->list[handler->activeID].type.compare("app") == 0) {
        ofClear(255,255,255, 0);
        appFbo.begin();
    }
    
    // we can turn off drawing the screen at the left and right app because it gets send to syphon anyway
    if (!drawScreen && handler->list[handler->activeID].type.compare("app") == 0) {
        handler->draw();
    } else if(drawScreen) {
        handler->draw();
    }
    
    // wraps up drawing into the FBO if it's an app, and also checks if it needs to be drawn to the screen
    if (syphonOut && handler->list[handler->activeID].type.compare("app") == 0) {
        appFbo.end();
        ofSetColor(255, 255, 255);
        if (drawScreen) {
            appFbo.draw(0,0);
        } else {
            ofBackground(50, 50, 50);
        }
    }
    
    // send out a syphon feed, should only be possible for left and right app, in case it's a video the video's
    // texture get's send, if it's a miniApp, we send the FBO
    if (syphonOut) {
        if (handler->list[handler->activeID].type.compare("mov") == 0) {
            syphonServer.publishTexture(&handler->player->players[handler->player->activeVid].vid.getTextureReference());
        } else if(handler->list[handler->activeID].type.compare("app") == 0) {
            syphonServer.publishTexture(&appFbo.getTextureReference());
        }
    }
    
    // in case you don't trust the GUI and want to see for yourself if the running right app has syphon turned on
    if (appName == "right") {
        ofSetColor(255,255,255);
        ofDrawBitmapString("app: " + appName + "\nSyphon: " + ofToString(syphonOut), 20,20);
    }
    
    // check for waiting OSC messages, this is just for testing with the iPad and touchOSC,
    // will be replaced by real objects
	while(receiver.hasWaitingMessages()){
		// get the next message
		ofxOscMessage m;
		receiver.getNextMessage(&m);
        
		// check for mouse moved message
		if(m.getAddress() == "/1/encoder1"){
            client.broadcast("tv1rotOSC," + ofToString(m.getArgAsFloat(0)));
		}
		// check for mouse button message
		else if(m.getAddress() == "/1/encoder2"){
            client.broadcast("tv2rotOSC," + ofToString(m.getArgAsFloat(0)));
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
        
        // check if there's a , in there should make it so that it also accepts one value, but i'm too lazy for that..
        if (msg[0].find(",") > 0) {
            vector<string> splitMsg = ofSplitString(msg[0], ",");
            
            // will be captured further on, annoying to keep seeing these messages..
            if (splitMsg[0].compare("FRAMERATE") == 0) {
                // do nothing
            } else {
                printf("MESSAGE: %s\n",splitMsg[0].c_str());
            }
            
            // checking if the XML we've written is the same everywhere, otherwise it means not all directories are the same
            if (splitMsg[0].compare("checkXML") == 0) {
<<<<<<< HEAD
                if (ofToInt(splitMsg[2]) < reader.partXML.size()) {
                    if (splitMsg[3].compare(reader.partXML[ofToInt(splitMsg[2])].part) != 0) {
                        // the (ridiculous) long space after the sentence send to the outputframe is because otherwise it won't show...
                        outputString = "In app " + splitMsg[1] + " zijn niet alle bestanden correct!                              .";
                        gui->outputFrame->setTextString(outputString);
                    } else {
                        //printf("all files in ORDER!\n");
                    }
=======
                if (splitMsg[3].compare(reader.partXML[ofToInt(splitMsg[2])].part) != 0) {
                    // the (ridiculous) long empty space after the sentence send to the outputframe is because otherwise it won't show...
                    outputString = "In app " + splitMsg[1] + " zijn niet alle bestanden correct!                              .";
                    gui->outputFrame->setTextString(outputString);
                } else {
                    //printf("all files in ORDER!\n");
>>>>>>> chapterHandler-noInfo
                }
                reader.partXML[ofToInt(splitMsg[2])].checked = true;
            }
            
            // =========== GUI COLUMN 1 ===========
            
            // ----------- DRAW SCREEN ------------
            
            // turn drawing on the screen on or off, second value is the false/true
            if (splitMsg[0].compare("drawLa") == 0) {
                if (appName == "left" && ofToInt(splitMsg[1]) == 1) {
                    drawScreen = true;
                    printf("drawLA: TRUE\n");
                } else if(appName == "left" && ofToInt(splitMsg[1]) == 0) {
                    drawScreen = false;
                    printf("drawLA: FALSE\n");
                }
                gui->drawLaBtn->setValue(ofToInt(splitMsg[1]));
            }
            
            // turn drawing on the screen on or off, second value is the false/true
            if (splitMsg[0].compare("drawRa") == 0) {
                if (appName == "right" && ofToInt(splitMsg[1]) == 1) {
                    drawScreen = true;
                    printf("drawRA: TRUE\n");
                } else if(appName == "right" && ofToInt(splitMsg[1]) == 0) {
                    drawScreen = false;
                    printf("drawRA: FALSE\n");
                }
                gui->drawRaBtn->setValue(ofToInt(splitMsg[1]));
            }
            
            // ------------ FULLSCREEN ------------
            
            // toggle fullscreen of the main screen
            if (splitMsg[0].compare("middleFs") == 0) {
                if (appName == "middle" && ofToInt(splitMsg[1]) == 1) {
                    ofToggleFullscreen();
                    printf("middleFs: TRUE\n");
                } else if(appName == "middle" && ofToInt(splitMsg[1]) == 0) {
                    ofToggleFullscreen();
                    printf("middleFs: FALSE\n");
                }
                gui->middleFsBtn->setValue(ofToInt(splitMsg[1]));
            }
            
            // toggle fullscreen of the TV 1 screen
            if (splitMsg[0].compare("tv1Fs") == 0) {
                if (appName == "TV_1" && ofToInt(splitMsg[1]) == 1) {
                    ofToggleFullscreen();
                    printf("tv1Fs: TRUE\n");
                } else if(appName == "TV_1" && ofToInt(splitMsg[1]) == 0) {
                    ofToggleFullscreen();
                    printf("tv1Fs: FALSE\n");
                }
                gui->tv1FsBtn->setValue(ofToInt(splitMsg[1]));
            }
            
            // toggle fullscreen of the TV 2 screen
            if (splitMsg[0].compare("tv2Fs") == 0) {
                if (appName == "TV_2" && ofToInt(splitMsg[1]) == 1) {
                    ofToggleFullscreen();
                    printf("tv2Fs: TRUE\n");
                } else if(appName == "TV_2" && ofToInt(splitMsg[1]) == 0) {
                    ofToggleFullscreen();
                    printf("tv2Fs: FALSE\n");
                }
                gui->tv2FsBtn->setValue(ofToInt(splitMsg[1]));
            }
            
            // ------------ SYPHON OUT ------------
            
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
            
            // ------------ FPS ALL APPS ------------
            
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
            
            // ------------ SHOOT ------------
            
            // just to fire of a red colored ofBackground to check for latency
            if (splitMsg[0].compare("shoot") == 0) {
                ofBackground(255, 0, 0);
            }
            
            // =========== GUI COLUMN 2 ===========
            
            // ----------- CHAPTER LIST -----------
            
            // play a certain item from the allHandler list
            if (splitMsg[0].compare("handlerStart") == 0) {
                printf("play: %s - %s\n",splitMsg[1].c_str(),splitMsg[2].c_str());
                handler->start(splitMsg[1]);
            }
            
            // -------- PLAY/PAUSE/PREV/NEXT ---------
            
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
            
            // ----------- PLAY ALL -----------
            
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
            
            // =========== GUI COLUMN 3 ===========
            
            // ------- INTERACTIVE OBJECTS --------
            
            // set rotate (OSC) val of the TV 1 screen
            if (splitMsg[0].compare("tv1rotOSC") == 0) {
                float incoming = ofToFloat(splitMsg[1]);
                if (incoming == 1.0) {
                    if (gui->tv1rotVal > 359) {
                        gui->tv1rotVal = 0;
                        //gui->tv1rotTotVal += 360;
                    } else {
                        gui->tv1rotVal += 3;
                    }
                    gui->tv1rotTotVal += 3;
                } else if(incoming == 0) {
                    if (gui->tv1rotVal < 1) {
                        gui->tv1rotVal = 359;
                        //gui->tv1rotTotVal -= 360;
                    } else {
                        gui->tv1rotVal -= 3;
                    }
                    gui->tv1rotTotVal -= 3;
                } else {
                    gui -> tv1rotVal = incoming;
                }
                gui->tv1rot->setValue(gui->tv1rotVal);
                handler->miniApp->main->tv1pos = gui->tv1rotVal;
                handler->miniApp->main->totalTv1pos = gui->tv1rotTotVal;
            }
            
            // set rotate (OSC) val of the TV 2 screen
            if (splitMsg[0].compare("tv2rotOSC") == 0) {
                float incoming = ofToFloat(splitMsg[1]);
                
                if (incoming == 1.0) {
                    if (gui->tv2rotVal > 359) {
                        gui->tv2rotVal = 0;
                        //gui->tv2rotTotVal += 360;
                    } else {
                        gui->tv2rotVal += 3;
                    }
                    gui->tv2rotTotVal += 3;
                } else if(incoming == 0) {
                    if (gui->tv2rotVal < 1) {
                        gui->tv2rotVal = 359;
                        //gui->tv2rotTotVal -= 360;
                    } else {
                        gui->tv2rotVal -= 3;
                    }
                    gui->tv2rotTotVal -= 3;
                } else {
                    gui -> tv2rotVal = incoming;
                }

                
                gui->tv2rot->setValue(gui->tv2rotVal);
                handler->miniApp->main->tv2pos = gui->tv2rotVal;
                handler->miniApp->main->totalTv2pos = gui->tv2rotTotVal;
            }
            
            // set rotate val of the TV 1 screen
            if (splitMsg[0].compare("tv1rot") == 0) {
                float incoming = ofToFloat(splitMsg[1]);
                incoming = ofMap(incoming, 0.0, 1.0, 0.0, 360.0);
                
                if (gui->tv1rotVal > 270 && incoming < 90) {
                    if (incoming < gui->tv1rotVal) {
                        gui->tv1rotTotVal += 360;
                    }
                }
                
                if (gui->tv1rotVal < 90 && incoming > 270) {
                    if (incoming > gui->tv1rotVal) {
                        gui->tv1rotTotVal -= 360;
                    }
                }
                
                gui->tv1rotTotVal = gui->tv1rotTotVal - (gui->tv1rotVal - incoming);
                gui -> tv1rotVal = incoming;
                gui->tv1rot->setValue(gui->tv1rotVal);
                handler->miniApp->main->tv1pos = gui->tv1rotVal;
                handler->miniApp->main->totalTv1pos = gui->tv1rotTotVal;
            }
            
            // set rotate val of the TV 2 screen
            if (splitMsg[0].compare("tv2rot") == 0) {
                float incoming = ofToFloat(splitMsg[1]);
                incoming = ofMap(incoming, 0.0, 1.0, 0.0, 360.0);
                
                if (gui->tv2rotVal > 270 && incoming < 90) {
                    if (incoming < gui->tv2rotVal) {
                        gui->tv2rotTotVal += 360;
                    }
                }
                
                if (gui->tv2rotVal < 90 && incoming > 270) {
                    if (incoming > gui->tv2rotVal) {
                        gui->tv2rotTotVal -= 360;
                    }
                }
                
                gui->tv2rotTotVal = gui->tv2rotTotVal - (gui->tv2rotVal - incoming);
                gui->tv2rotVal = incoming;
                gui->tv2rot->setValue(gui->tv2rotVal);
                handler->miniApp->main->tv2pos = gui->tv2rotVal;
                handler->miniApp->main->totalTv2pos = gui->tv2rotTotVal;
            }
            
            // ------- SCAN FOLDER --------
            
            // read directory of chapterHandler
            if (splitMsg[0].compare("readDir") == 0) {
                reader.readDir();
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
    
    gui->keyPressed(key); // so we can toggle visibility by pressing 'h'
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