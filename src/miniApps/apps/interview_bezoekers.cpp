//
//  interview_bezoekers.cpp
//  eCinemaSetup
//
//  Created by Mick van Olst on 16-12-12.
//
//

#include "interview_bezoekers.h"

//--------------------------------------------------------------
interview_bezoekers::interview_bezoekers(){
    
}

//--------------------------------------------------------------
void interview_bezoekers::init(mainMini *_mai, ofxOscSender *_osc){
    main = _mai;
    oscOut = _osc;
}

//--------------------------------------------------------------
void interview_bezoekers::setup(){
    initTime = ofGetElapsedTimeMillis();
    
    ofClear(255, 255, 255);
    ofBackground(0, 0, 0);
    
    files.allowExt("mov");
    files.allowExt("mp4");
    files.listDir("app_content/interview_bezoekers/clips");
    files.sort();
    
    for (int i = 0; i < 5; i++) {//files.numFiles()
        smartClip clip;
        clip.vid.loadMovie(files.getPath(i));
        clip.vid.setLoopState(OF_LOOP_NONE);
        clip.vid.setFrame(1);

        clip.viewport.x = 0;
        clip.viewport.y = 0;
        clip.viewport.width = main->client->getLWidth()/2;
        clip.viewport.height = main->client->getLHeight();
        
        clip.onScreen = false; // figure this out later
        clip.inFrame = 0; // in and out will be changed if needed by loadXML
        clip.outFrame = clip.vid.getTotalNumFrames();
        
        clip.filename = files.getName(i);
        
        videos.push_back(clip);
    }
    
    loadXML("app_content/interview_bezoekers/inOutPoints.xml", true);
    
    for (int i = 0; i < 5; i++) {//files.numFiles()
        videos[i].vid.setFrame(videos[i].inFrame);
    }
    
    curVid = 0;
    rotateVideos = 90; // degrees
    
    videos[curVid].vid.setFrame(videos[curVid].inFrame);
    videos[curVid].vid.play();
}

//--------------------------------------------------------------
void interview_bezoekers::update(){
    durTime = ofGetElapsedTimeMillis() - initTime;
    
    for (int i = 0; i < videos.size(); i++) {
        videos[i].vid.update();
    }
    
    //videos[curVid].vid.getIsMovieDone()
    if (videos[curVid].outFrame <= videos[curVid].vid.getCurrentFrame()) {
        videos[curVid].vid.setPaused(true);
        curVid++;
        
        if (curVid < videos.size()) {
            //videos[curVid].vid.setFrame(videos[curVid].inFrame);
            if (main->appName == "left") {
                if (curVid == 0 || curVid == 1) {
                    videos[curVid].vid.play();
                }
            } else if (main->appName == "middle" && curVid == 2) {
                videos[curVid].vid.play();
            } else if (main->appName == "right") {
                if (curVid == 3 || curVid == 4) {
                    videos[curVid].vid.play();
                }
            }
                
            
        } else {
            //endOfMini();
        }
    }
}

//--------------------------------------------------------------
void interview_bezoekers::draw(){
    ofBackground(0, 0, 0);
    drawViewPorts(videos.size());
}

//--------------------------------------------------------------
void interview_bezoekers::drawViewPorts(int numPorts){
    switch (numPorts) {
        case 1: // [-l-] [-l-] [1l1l1l1] [-l-] [-l-]
            
            break;
        case 2: // [-l-] [-l-] [1l1l2l2] [-l-] [-l-]
            
            break;
        case 3: // [1l1] [-l-] [-l2l2l-] [-l-] [3l3]
            
            break;
        case 4: // [1l1] [2l2] [-l-l-l-] [3l3] [4l4]
            
            break;
        case 5: // [1l1] [2l2] [-l3l3l-] [4l4] [5l5]
            
            if (main->appName == "left" || main->appName == "right") {
                int vidNr;
                int tempHeight = 0;
                if (main->appName == "left") {
                    vidNr = 0;                   
                } else if(main->appName == "right") {
                    vidNr = 3;
                }
                
                main->scaleByWidth(videos[vidNr].vid.getWidth(), videos[vidNr].vid.getHeight(), main->client->getLHeight(), &tempHeight);
                
                videos[vidNr].viewport.x = 0;
                videos[vidNr].viewport.y = 0;
                videos[vidNr].vidpos.x = main->client->getLHeight() / 2 * -1;
                videos[vidNr].vidpos.y = main->client->getLWidth()/2 * -1;//tempHeight/2 * -1;//main->client->getLWidth()/2 * -1;
                videos[vidNr].viewport.width = main->client->getLWidth()/2;
                videos[vidNr].viewport.height = main->client->getLHeight();

                ofPushView();
                    ofViewport(videos[vidNr].viewport);
                    //ofSetupScreen();
                    glScalef(-1, -1, 1);
                    glTranslatef(-main->client->getXoffset(), -main->client->getLHeight(), 0);
                    
                    ofSetColor(255, 255, 255);
                    ofPushMatrix();
                        ofSetRectMode(OF_RECTMODE_CENTER);
                        ofRotate(rotateVideos + 180);
                        ofTranslate(0, 0, 0 );
                        videos[vidNr].vid.draw(videos[vidNr].vidpos.x, videos[vidNr].vidpos.y, main->client->getLHeight(), tempHeight*2);
                        ofSetRectMode(OF_RECTMODE_CORNER);
                    ofPopMatrix();
                ofPopView();
                
                if (main->appName == "left") {
                    vidNr = 1;
                } else if(main->appName == "right") {
                    vidNr = 4;
                }
                tempHeight = 0;
                
                main->scaleByWidth(videos[vidNr].vid.getWidth(), videos[vidNr].vid.getHeight(), main->client->getLHeight(), &tempHeight);
                
                videos[vidNr].viewport.x = main->client->getLWidth()/2;
                videos[vidNr].viewport.y = 0;
                videos[vidNr].vidpos.x = main->client->getLHeight() / 2 * -1;
                videos[vidNr].vidpos.y = main->client->getLWidth()/2 * -1;//tempHeight/2 * -1;//main->client->getLWidth()/2 * -1;
                videos[vidNr].viewport.width = main->client->getLWidth()/2;
                videos[vidNr].viewport.height = main->client->getLHeight();
                
                ofPushView();
                ofViewport(videos[vidNr].viewport);
                    //ofSetupScreen();
                    glScalef(-1, -1, 1);
                    glTranslatef(-main->client->getXoffset(), -main->client->getLHeight(), 0);
                    
                    ofSetColor(255, 255, 255);
                    ofPushMatrix();
                        ofSetRectMode(OF_RECTMODE_CENTER);
                        ofRotate(rotateVideos + 180);
                        ofTranslate(0, 0, 0 );
                        videos[vidNr].vid.draw(videos[vidNr].vidpos.x, videos[vidNr].vidpos.y, main->client->getLHeight(), tempHeight*2);
                        ofSetRectMode(OF_RECTMODE_CORNER);
                    ofPopMatrix();
                ofPopView();
                
            } else if(main->appName == "middle") {
                
                int vidNr = 2;
                int tempHeight = 0;
                
                main->scaleByWidth(videos[vidNr].vid.getWidth(), videos[vidNr].vid.getHeight(), main->client->getLHeight(), &tempHeight);
                
                videos[vidNr].viewport.x = main->client->getLWidth()/4;
                videos[vidNr].viewport.y = 0;
                videos[vidNr].vidpos.x = main->client->getLHeight() / 2 * -1;
                videos[vidNr].vidpos.y = main->client->getLWidth()/2 * -1;//tempHeight/2 * -1;//main->client->getLWidth()/2 * -1;
                videos[vidNr].viewport.width = main->client->getLWidth()/2;
                videos[vidNr].viewport.height = main->client->getLHeight();
                
                ofPushView();
                    ofViewport(videos[vidNr].viewport);
                    //ofSetupScreen();
                    glScalef(-1, -1, 1);
                    glTranslatef(-main->client->getXoffset(), -main->client->getLHeight(), 0);
                    
                    ofSetColor(255, 255, 255);
                    ofPushMatrix();
                        ofSetRectMode(OF_RECTMODE_CENTER);
                        ofRotate(rotateVideos + 180);
                        ofTranslate(0, 0, 0 );
                        videos[vidNr].vid.draw(videos[vidNr].vidpos.x, videos[vidNr].vidpos.y, main->client->getLHeight(), tempHeight*2);
                        ofSetRectMode(OF_RECTMODE_CORNER);
                    ofPopMatrix();
                ofPopView();
            }
            break;
        default:
            break;
    }
}

//--------------------------------------------------------------
void interview_bezoekers::loadXML(string file, bool printResult){
    if (!XML.loadFile(file)) {
        cout << "failed loading " << file << "\n";
    } else {
        XML.pushTag("settings");
            rotateVideos = XML.getValue("rotate", 0);
        XML.popTag();
        
        XML.pushTag("inoutpoints");
        int numVideos = XML.getNumTags("video");
        
        for (int i = 0; i < numVideos; i++) {
            XML.pushTag("video",i);
            for (int v = 0; v < videos.size(); v++) {
                if (videos[v].filename == XML.getValue("filename", "")) {
                    videos[v].inFrame = XML.getValue("inFrame", 0);
                    videos[v].outFrame = XML.getValue("outFrame", 0);
                    
                    if(videos[v].inFrame < 0 || videos[v].inFrame > videos[v].vid.getTotalNumFrames() || videos[v].inFrame >= videos[v].outFrame) {
                        cout << videos[v].filename << "is being fead funky XML!\n";
                        videos[v].inFrame = 0;
                    }
                    if(videos[v].outFrame < 0 || videos[v].outFrame > videos[v].vid.getTotalNumFrames() || videos[v].outFrame <= videos[v].inFrame) {
                        cout << videos[v].filename << "is being fead funky XML!\n";
                        videos[v].outFrame = videos[v].vid.getTotalNumFrames();
                    }
                    
                    if (printResult) {
                        cout << "video: " << videos[v].filename << " in: " << videos[v].inFrame << " out: " << videos[v].outFrame << "\n";
                    }
                }
            }
            XML.popTag();
        }
        XML.popTag();
    }
}

//--------------------------------------------------------------
void interview_bezoekers::exit(){
    for (int i = 0; i < videos.size(); i++) {
        videos[i].vid.close();
        videos.erase(videos.begin(), videos.end());
    }
}

//--------------------------------------------------------------
void interview_bezoekers::endOfMini(){
    main->done();
}