#include "testApp.h"

#define RECONNECT_TIME 400

testApp::testApp()
{
    tv1rotVal    = 0;
    tv2rotVal    = 0;
    tv1rotTotVal = 0;
    tv2rotTotVal = 0;
    
    hiddenCursor = false;
    
    handler.init(&client, &reader, &oscOut, &tv1rotVal, &tv2rotVal, &tv1rotTotVal, &tv2rotTotVal);
    gui.init(&client, &handler, &tv1rotVal, &tv2rotVal, &tv1rotTotVal, &tv2rotTotVal, &oscOutIp, &oscOutPort);
}

//--------------------------------------------------------------
void testApp::setup(){
	// initialize app
	ofSetFrameRate(30);
    ofEnableSmoothing();
    //ofEnableAlphaBlending();
    ofSetVerticalSync(true);
	ofSetBackgroundAuto(false); // otherwise it will flicker during every framedrop
    ofBackground(50, 50, 50);
    
	client.setup("mpe_settings.xml", this);
    
    string appNameList[3] = {"left","middle","right"}; // ,"TV_1", "TV_2"
    appName = appNameList[client.getID()];
    
    if (XML.loadFile("osc_settings.xml")) {
        XML.pushTag("settings");
        oscOutIp            = XML.getValue("out:ip", "");
        oscOutPort          = XML.getValue("out:port", 0);
        oscInPort           = XML.getValue("in:port", 0);
        XML.popTag();
    } else {
        cout << "loading osc_settings.xml failed, needed for OSC\n";
    }
    
    handler.setup(appName);
    gui.setup(appName);
    receiver.setup(oscInPort); // OSC
    
    oscOut.setup(oscOutIp, oscOutPort);
        
    client.start();
    
    // read out the directory and check if all files are correct
    reader.setup(appName);
    reader.readDir();
    
    handler.createList(); // setting up our total list of apps and movies
    
    // syphon is ON by default at the left and right app
    //if (appName == "left" || appName == "right") {
        syphonServer.setName(appName);
        syphonOut = true;
    //} else {
    //    syphonOut = false;
    //}
    
    drawScreen = true;
    ignoreOSCrot = false;
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
    
    //cout << "TV1: " << tv1rotVal << "\n";
    
    // clear the screen
    ofBackground(50, 50, 50);
    ofSetColor(255, 255, 255);
    
    handler.update();
    
    // some clearing needed for syphon output
    if (syphonOut) {
        // Clear with alpha, so we can capture via syphon and composite elsewhere should we want.
        glClearColor(0.0, 0.0, 0.0, 0.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
    
    // start capturing in the FBO if there is a miniApp playing
    if (syphonOut && handler.list[handler.activeID].type.compare("app") == 0) {
        ofClear(255,255,255, 0);
        appFbo.begin();
        ofBackground(0, 0, 0);
        ofSetColor(0, 0, 0);
        ofRect(0, 0, client.getLWidth(), client.getLHeight());
        ofSetColor(255, 255, 255);
        ofPushMatrix();
        ofTranslate(-client.getXoffset(), client.getYoffset());
    }
    
    handler.draw();
    // wraps up drawing into the FBO if it's an app, and also checks if it needs to be drawn to the screen
    if (syphonOut && handler.list[handler.activeID].type.compare("app") == 0) {
        ofPopMatrix();
        appFbo.end();
        ofSetColor(255, 255, 255);
        if (drawScreen) {
            appFbo.draw(client.getXoffset(),0,ofGetWidth(),ofGetHeight()); // ajust this later
        } else {
            ofBackground(50, 50, 50);
        }
    }
    
    // send out a syphon feed, should only be possible for left and right app, in case it's a video the video's
    // texture get's send, if it's a miniApp, we send the FBO
    if (syphonOut) {
        if (handler.list[handler.activeID].type.compare("mov") == 0) {
            syphonServer.publishTexture(&handler.player.players[handler.player.activeVid].getTextureReference());
        } else if(handler.list[handler.activeID].type.compare("app") == 0) {
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
    int val1 = 0;
    int val2 = 0;
    if (ignoreOSCrot == false) {
        while(receiver.hasWaitingMessages()){
            // get the next message
            ofxOscMessage m;
            receiver.getNextMessage(&m);
            
            // check encoder 1
            if(m.getAddress() == "/1/encoder1"){
                if (m.getArgAsFloat(0) == 0) {
                    val1 -= 1;
                } else if(m.getArgAsFloat(0) == 1) {
                    val1 += 1;
                }
            }
            // check encoder 2
            if(m.getAddress() == "/1/encoder2"){
                if (m.getArgAsFloat(0) == 0) {
                    val2 -= 1;
                } else if(m.getArgAsFloat(0) == 1) {
                    val2 += 1;
                }
            }
        }
    } else {
        // ignoring the OSC values from the rotary encoders
    }
	
    string sendVals = "tvRotOSC," + ofToString(val1) + "," + ofToString(val2);
    if (val1 == 0 && val2 == 0) {
        // do nothing
    } else {
        client.broadcast(sendVals);
        //cout << sendVals << "\n";
    }
    
    handleMessages(); // handle all messages from the MPE client
    
    // sending FPS value over the network to all cients every 200 milisec
    if ((ofGetElapsedTimeMillis() - fpsCounter ) > 200) {
        client.broadcast("FRAMERATE," + ofToString(client.getID()) + "," +  ofToString(ofGetFrameRate()) );
        fpsCounter = ofGetElapsedTimeMillis();
    }
    
    // sending values from player to gui, later change this to variable binding
    gui.chapCurPercent->setValue(handler.player.chapCurPercent);
    gui.chapTotalTime->setLabel("TOTAL: " + ofToString(handler.player.chapTotalTime) + " SEC");
    gui.totalPercent->setValue(handler.player.totalProgress);
    gui.playBtn->setValue(handler.player.isPlaying);
    gui.pauseBtn->setValue(!handler.player.isPlaying);
    
    // this for loop sets the buttons true or false each time,
    for (int i = 0; i < handler.list.size(); i++) {
        gui.listBtn[i].btn->setValue(handler.list[i].active);
    }
}

//--------------------------------------------------------------
void testApp::handleMessages(){
    // read any incoming messages
    if (client.messageAvailable()) {
        vector<string> msg = client.getDataMessage();
        
        // check if there's a , in there should make it so that it also accepts one value, but i'm too lazy for that..
        //printf("msg size: %li ",msg.size());
        if (msg.empty()) {
            printf("it's empty\n");
        } else {
            //for (int b = 0; b < msg.size(); b++) {
            //    printf("msg: %s", msg[b].c_str());
            //}
            if (msg[0].find(",") > 0) {
                vector<string> splitMsg = ofSplitString(msg[0], ",");
                
                // will be captured further on, annoying to keep seeing these messages..
                if (splitMsg[0].compare("FRAMERATE") == 0) {
                    // do nothing
                } else {
                    //printf("MESSAGE: %s\n",splitMsg[0].c_str());
                }
                
                // checking if the XML we've written is the same everywhere, otherwise it means not all directories are the same
                if (splitMsg[0].compare("checkXML") == 0 && splitMsg.size() == 4) {
                    if (ofToInt(splitMsg[2]) < reader.partXML.size()) {
                        if (splitMsg[3].compare(reader.partXML[ofToInt(splitMsg[2])].part) != 0) {
                            // the (ridiculous) long empty space after the sentence send to the outputframe is because otherwise it won't show...
                            outputString = "In app " + splitMsg[1] + " zijn niet alle bestanden correct!                              .";
                            gui.outputFrame->setTextString(outputString);
                        } else {
                            //printf("all files in ORDER!\n");
                        }
                    }
                    reader.partXML[ofToInt(splitMsg[2])].checked = true;
                }
                
                // =========== GUI COLUMN 1 ===========
                
                // ----------- DRAW SCREEN ------------
                
                // turn drawing on the screen on or off, second value is the false/true
                if (splitMsg[0].compare("drawLa") == 0  && splitMsg.size() == 2) {
                    if (appName == "left" && ofToInt(splitMsg[1]) == 1) {
                        drawScreen = true;
                        printf("drawLA: TRUE\n");
                    } else if(appName == "left" && ofToInt(splitMsg[1]) == 0) {
                        drawScreen = false;
                        printf("drawLA: FALSE\n");
                    }
                    gui.drawLaBtn->setValue(ofToInt(splitMsg[1]));
                }
                
                // turn drawing on the screen on or off, second value is the false/true
                if (splitMsg[0].compare("drawRa") == 0 && splitMsg.size() == 2) {
                    if (appName == "right" && ofToInt(splitMsg[1]) == 1) {
                        drawScreen = true;
                        printf("drawRA: TRUE\n");
                    } else if(appName == "right" && ofToInt(splitMsg[1]) == 0) {
                        drawScreen = false;
                        printf("drawRA: FALSE\n");
                    }
                    gui.drawRaBtn->setValue(ofToInt(splitMsg[1]));
                }
                
                // ------------ FULLSCREEN ------------
                
                // toggle fullscreen of the main screen
                if (splitMsg[0].compare("middleFs") == 0 && splitMsg.size() == 2) {
                    if (appName == "middle" && ofToInt(splitMsg[1]) == 1) {
                        ofToggleFullscreen();
                        printf("middleFs: TRUE\n");
                    } else if(appName == "middle" && ofToInt(splitMsg[1]) == 0) {
                        ofToggleFullscreen();
                        printf("middleFs: FALSE\n");
                    }
                    gui.middleFsBtn->setValue(ofToInt(splitMsg[1]));
                }
                
//                // toggle fullscreen of the TV 1 screen
//                if (splitMsg[0].compare("tv1Fs") == 0 && splitMsg.size() == 2) {
//                    if (appName == "TV_1" && ofToInt(splitMsg[1]) == 1) {
//                        ofToggleFullscreen();
//                        printf("tv1Fs: TRUE\n");
//                    } else if(appName == "TV_1" && ofToInt(splitMsg[1]) == 0) {
//                        ofToggleFullscreen();
//                        printf("tv1Fs: FALSE\n");
//                    }
//                    gui.tv1FsBtn->setValue(ofToInt(splitMsg[1]));
//                }
//                
//                // toggle fullscreen of the TV 2 screen
//                if (splitMsg[0].compare("tv2Fs") == 0 && splitMsg.size() == 2) {
//                    if (appName == "TV_2" && ofToInt(splitMsg[1]) == 1) {
//                        ofToggleFullscreen();
//                        printf("tv2Fs: TRUE\n");
//                    } else if(appName == "TV_2" && ofToInt(splitMsg[1]) == 0) {
//                        ofToggleFullscreen();
//                        printf("tv2Fs: FALSE\n");
//                    }
//                    gui.tv2FsBtn->setValue(ofToInt(splitMsg[1]));
//                }
                
                // ------------ SYPHON OUT ------------
                
                // turn syphon on or off, second value is the false/true
                if (splitMsg[0].compare("syphonLa") == 0 && splitMsg.size() == 2) {
                    if (appName == "left" && ofToInt(splitMsg[1]) == 1) {
                        syphonOut = true;
                        printf("syphonLA: TRUE\n");
                    } else if(appName == "left" && ofToInt(splitMsg[1]) == 0) {
                        syphonOut = false;
                        printf("syphonLA: FALSE\n");
                    }
                    gui.syphonLaBtn->setValue(ofToInt(splitMsg[1]));
                }
                
                // turn syphon on or off, second value is the false/true
                if (splitMsg[0].compare("syphonMa") == 0 && splitMsg.size() == 2) {
                    if (appName == "middle" && ofToInt(splitMsg[1]) == 1) {
                        syphonOut = true;
                        printf("syphonMA: TRUE\n");
                    } else if(appName == "middle" && ofToInt(splitMsg[1]) == 0) {
                        syphonOut = false;
                        printf("syphonMA: FALSE\n");
                    }
                    gui.syphonMaBtn->setValue(ofToInt(splitMsg[1]));
                }
                
                // turn syphon on or off, second value is the false/true
                if (splitMsg[0].compare("syphonRa") == 0 && splitMsg.size() == 2) {
                    if (appName == "right" && ofToInt(splitMsg[1]) == 1) {
                        syphonOut = true;
                        printf("syphonRA: TRUE\n");
                    } else if(appName == "right" && ofToInt(splitMsg[1]) == 0) {
                        syphonOut = false;
                        printf("syphonRA: FALSE\n");
                    }
                    gui.syphonRaBtn->setValue(ofToInt(splitMsg[1]));
                }
                
                // ------------ OSC ------------
                
                // turn osc on or off, second value is the false/true
                if (splitMsg[0].compare("oscOut") == 0 && splitMsg.size() == 2) {
                    if (appName == "left" && ofToInt(splitMsg[1]) == 1) {
                        handler.bOsc = true;
                        printf("oscOut: TRUE\n");
                    } else if(appName == "left" && ofToInt(splitMsg[1]) == 0) {
                        handler.bOsc = false;
                        printf("oscOut: FALSE\n");
                    }
                    gui.oscOutBtn->setValue(ofToInt(splitMsg[1]));
                }
                
                if (splitMsg[0].compare("ignoreOscIn") == 0 && splitMsg.size() == 2) {
                    if (appName == "left" && ofToInt(splitMsg[1]) == 1) {
                        ignoreOSCrot = true;
                        printf("ignoreOSCrot: TRUE\n");
                    } else if(appName == "left" && ofToInt(splitMsg[1]) == 0) {
                        ignoreOSCrot = false;
                        printf("ignoreOSCrot: FALSE\n");
                    }
                    gui.ignoreOscBtn->setValue(ofToInt(splitMsg[1]));
                }
                
                // ------------ FPS ALL APPS ------------
                
                // setting framerate for all apps
                if (splitMsg[0].compare("FRAMERATE") == 0 && splitMsg.size() == 3) {
                    //printf("CLIENT ID: %s FPS: %s\n",splitMsg[1].c_str(), splitMsg[2].c_str());
                    
                    // setting value to the right fps slider
                    switch (ofToInt(splitMsg[1])) {
                        case 0:
                            gui.fpsLaSlider->setValue(ofToInt(splitMsg[2]));
                            break;
                        case 1:
                            gui.fpsMaSlider->setValue(ofToInt(splitMsg[2]));
                            break;
                        case 2:
                            gui.fpsRaSlider->setValue(ofToInt(splitMsg[2]));
                            break;
                        default:
                            break;
                    }
                }
                
                // ------------ SHOOT ------------
                
                // just to fire of a red colored ofBackground to check for latency
                if (splitMsg[0].compare("shoot") == 0 && splitMsg.size() == 2) {
                    ofBackground(0, 255, 0);
                }
                
                // =========== GUI COLUMN 2 ===========
                
                // ----------- CHAPTER LIST -----------
                
                // play a certain item from the allHandler list
                if (splitMsg[0].compare("handlerStart") == 0 && splitMsg.size() == 3) {
                    printf("play: %s - %s\n",splitMsg[1].c_str(),splitMsg[2].c_str());
                    resetRotation();
                    handler.start(splitMsg[1]);
                }
                
                // -------- PLAY/PAUSE/PREV/NEXT ---------
                
                // play control
                if (splitMsg[0].compare("play") == 0 && splitMsg.size() == 2) {
                    handler.resume();
                }
                
                // pause control
                if (splitMsg[0].compare("pause") == 0 && splitMsg.size() == 2) {
                    handler.pause();
                }
                
                // prev control
                if (splitMsg[0].compare("prev") == 0 && splitMsg.size() == 2) {
                    resetRotation();
                    handler.startPrev();
                }
                
                // next control
                if (splitMsg[0].compare("next") == 0 && splitMsg.size() == 2) {
                    if ((ofGetElapsedTimeMillis() - nextCounter ) > 500) {
                        resetRotation();
                        handler.startNext();
                        nextCounter = ofGetElapsedTimeMillis();
                        appFbo.begin();
                        ofClear(255,255,255, 0);
                        appFbo.end();
                    }
                }
                
                // ----------- PLAY ALL -----------
                
                // turn playAll on or off, second value is the false/true
                if (splitMsg[0].compare("playAll") == 0 && splitMsg.size() == 2) {
                    if (ofToInt(splitMsg[1]) == 1) {
                        handler.bPlayAll = true;
                        printf("playAll: TRUE\n");
                    } else if(ofToInt(splitMsg[1]) == 0) {
                        handler.bPlayAll = false;
                        printf("playAll: FALSE\n");
                    }
                    gui.playAllBtn->setValue(ofToInt(splitMsg[1]));
                }
                
                // =========== GUI COLUMN 3 ===========
                
                // ------- INTERACTIVE OBJECTS --------
                
                // set rotate (OSC) val of TV 1 and 2
                if (splitMsg[0].compare("tvRotOSC") == 0 && splitMsg.size() == 3) {
                    int addNr1;
                    if (splitMsg[1].compare("0") != 0) {
                        handler.miniApp.main.totalTv1prevPos = tv1rotTotVal;
                        float incoming1 = ofToFloat(splitMsg[1]);
                        addNr1 = 5 * incoming1;
                        
                        if (incoming1 > 0) {
                            if (tv1rotVal > 359) {
                                tv1rotVal = 0;
                            } else {
                                tv1rotVal += addNr1;
                            }
                            tv1rotTotVal += addNr1;
                        } else if(incoming1 < 0) {
                            if (tv1rotVal < 1) {
                                tv1rotVal = 359;
                            } else {
                                tv1rotVal += addNr1;
                            }
                            tv1rotTotVal += addNr1;
                        }
                        
                        gui.tv1rot->setValue(tv1rotVal);
                        tv1rotTotVal += addNr1;
                    }
                    
                    int addNr2;
                    if (splitMsg[2].compare("0") != 0) {
                        handler.miniApp.main.totalTv2prevPos = tv2rotTotVal;
                        float incoming2 = ofToFloat(splitMsg[2]);
                        addNr2 = 5 * incoming2;
                        if (incoming2 > 0) {
                            if (tv2rotVal > 359) {
                                tv2rotVal = 0;
                            } else {
                                tv2rotVal += addNr2;
                            }
                            tv2rotTotVal += addNr2;
                        } else if(incoming2 < 0) {  
                            if (tv2rotVal < 1) {
                                tv2rotVal = 359;
                            } else {
                                tv2rotVal += addNr2;
                            }
                            tv2rotTotVal += addNr2;
                        }
                        
                        gui.tv2rot->setValue(tv2rotVal);
                        tv2rotTotVal += addNr2;
                    }
                    
                    // there's a bit of a nasty workaround here, if both values are changing, somehow this goes half speed, so we need to overdo it in order to have the same effect.
                    if (splitMsg[1].compare("0") != 0 && splitMsg[2].compare("0") != 0) {
                        tv1rotTotVal += addNr1;
                        gui.tv1rot->setValue(tv1rotVal);
                        tv2rotTotVal += addNr2;
                        gui.tv2rot->setValue(tv2rotVal);
                    }
                }
                
                // set rotate val of the TV 1 screen
                if (splitMsg[0].compare("tv1rot") == 0 && splitMsg.size() == 2) {
                    handler.miniApp.main.totalTv1prevPos = tv1rotTotVal;
                    
                    float incoming = ofToFloat(splitMsg[1]);
                    incoming = ofMap(incoming, 0.0, 1.0, 0.0, 360.0);
                    
                    if (tv1rotVal > 270 && incoming < 90) {
                        if (incoming < tv1rotVal) {
                            tv1rotTotVal += 360;
                        }
                    }
                    
                    if (tv1rotVal < 90 && incoming > 270) {
                        if (incoming > tv1rotVal) {
                            tv1rotTotVal -= 360;
                        }
                    }
                    
                    tv1rotTotVal = tv1rotTotVal - (tv1rotVal - incoming);
                    tv1rotVal = incoming;
                }
                
                // set rotate val of the TV 2 screen
                if (splitMsg[0].compare("tv2rot") == 0 && splitMsg.size() == 2) {
                    handler.miniApp.main.totalTv2prevPos = tv2rotTotVal;
                    
                    float incoming = ofToFloat(splitMsg[1]);
                    incoming = ofMap(incoming, 0.0, 1.0, 0.0, 360.0);
                    
                    if (tv2rotVal > 270 && incoming < 90) {
                        if (incoming < tv2rotVal) {
                            tv2rotTotVal += 360;
                        }
                    }
                    
                    if (tv2rotVal < 90 && incoming > 270) {
                        if (incoming > tv2rotVal) {
                            tv2rotTotVal -= 360;
                        }
                    }
                    
                    tv2rotTotVal = tv2rotTotVal - (tv2rotVal - incoming);
                    tv2rotVal = incoming;
                }
                
                // ------- SCAN FOLDER --------
                
                // read directory of chapterHandler
                if (splitMsg[0].compare("readDir") == 0 && splitMsg.size() == 2) {
                    reader.readDir();
                }
            }
        }
    }
}

//--------------------------------------------------------------
void testApp::scaleByHeight(int oW, int oH, int *nW, int nH){
    //original width / original height x new height = new width
    float newVal = (float(oW) / float(oH)) * float(nH);
    (*nW) = int(newVal);
}

//--------------------------------------------------------------
void testApp::scaleByWidth(int oW, int oH, int nW, int *nH){
    //original height / original width x new width = new height
    float newVal = (float(oH) / float(oW)) * float(nW);
    (*nH) = int(newVal);
}

//--------------------------------------------------------------
void testApp::resetRotation()
{
    tv1rotVal       = 0;
    tv2rotVal       = 0;
    tv1rotTotVal    = 0;
    tv2rotTotVal    = 0;
    gui.tv1rot->setValue(0);
    gui.tv2rot->setValue(0);
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    switch (key) {
        case 'f':
			ofToggleFullscreen();
			break;
        case 'c':
            if (hiddenCursor) {
                ofShowCursor();
            } else {
                ofHideCursor();
            }
            hiddenCursor = !hiddenCursor;
            break;
        default:
            break;
    }
    
    gui.keyPressed(key); // so we can toggle visibility by pressing 'h'
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
void testApp::exit() {
	// clean up
}