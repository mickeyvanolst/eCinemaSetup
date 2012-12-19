//
//  intentie_interpretatie.cpp
//  eCinemaSetup
//
//  Created by Mick van Olst on 14-12-12.
//
//

#include "intentie_interpretatie.h"
//--------------------------------------------------------------
intentie_interpretatie::intentie_interpretatie(){
    
}

//--------------------------------------------------------------
void intentie_interpretatie::init(mainMini *_mai, ofxMidiOut *_midi){
    main = _mai;
    midiOut = _midi;
}

//--------------------------------------------------------------
void intentie_interpretatie::setup(){
    initTime = ofGetElapsedTimeMillis();
    ofClear(255, 255, 255);
    ofBackground(0, 0, 0);
    
    if (main->appName == "left") {
        Vid1.loadMovie("app_content/intentie_interpretatie/c1.mov");
        Vid2.loadMovie("app_content/intentie_interpretatie/c2.mov");
        
        ofVideoPlayer Vid3;
        Vid3.loadMovie("app_content/intentie_interpretatie/c3.mov");
        
        ofVideoPlayer Vid4;
        Vid4.loadMovie("app_content/intentie_interpretatie/c4.mov");
        
        numFrames[0] = Vid1.getTotalNumFrames();
        numFrames[1] = Vid2.getTotalNumFrames();
        numFrames[2] = Vid3.getTotalNumFrames();
        numFrames[3] = Vid4.getTotalNumFrames();
        
    } else if(main->appName == "right") {
        ofVideoPlayer Vid3;
        Vid3.loadMovie("app_content/intentie_interpretatie/c1.mov");
        ofVideoPlayer Vid4;
        Vid4.loadMovie("app_content/intentie_interpretatie/c2.mov");
        
        Vid1.loadMovie("app_content/intentie_interpretatie/c3.mov");
        Vid2.loadMovie("app_content/intentie_interpretatie/c4.mov");
        
        numFrames[0] = Vid3.getTotalNumFrames();
        numFrames[1] = Vid4.getTotalNumFrames();
        numFrames[2] = Vid1.getTotalNumFrames();
        numFrames[3] = Vid2.getTotalNumFrames();
    }
    
    totalFrames = 0;
    for (int i = 0; i < 4; i++) {
        totalFrames += numFrames[i];
    }
    
    Vid1.setLoopState(OF_LOOP_NORMAL);
    Vid2.setLoopState(OF_LOOP_NORMAL);
    
    curActive = 0;
    clipCurFrame = 0;
    frameRepeat = 30;
    
    prevTv1pos = 1; // just some value to get things started in update
    prevTv2Pos = 1;
    
    cout << "numfr[0] " << numFrames[0] << "\n";
    cout << "numfr[1] " << numFrames[1] << "\n";
    cout << "numfr[2] " << numFrames[2] << "\n";
    cout << "numfr[3] " << numFrames[3] << "\n";
    
    cout << "totalFrames: " << totalFrames << "\n";
    
    // setting viewport rectangle
	viewport1.x = main->client->getXoffset();
	viewport1.y = main->client->getYoffset();
	viewport1.width = main->client->getLWidth()/2;
	viewport1.height = main->client->getLHeight();
    
    viewport2.x = main->client->getXoffset() + (main->client->getLWidth()/2);
	viewport2.y = main->client->getYoffset();
	viewport2.width = main->client->getLWidth()/2;
	viewport2.height = main->client->getLHeight();
}

//--------------------------------------------------------------
void intentie_interpretatie::update(){
    if (main->appName != "middle") {
        durTime = ofGetElapsedTimeMillis() - initTime;
        
        Vid1.update();
        Vid2.update();
        
        // just some dumb-ass way to tell what clip should be active
        if (*main->tv1pos >= 0 && *main->tv1pos < 90) {
            curActive = 0;
        } else if(*main->tv1pos >= 90 && *main->tv1pos < 180) {
            curActive = 1;
        } else if(*main->tv1pos >= 180 && *main->tv1pos < 270) {
            curActive = 2;
        } else if(*main->tv1pos >= 270 && *main->tv1pos < 360) {
            curActive = 3;
        }
        
        clipCurFrame = ofMap(*main->tv1pos - main->sortaModulo(90, *main->tv1pos), 0, 90, 0, numFrames[curActive]);
        
        if (prevTv1pos != *main->tv1pos) {
            if (main->appName == "left") {
                if (curActive == 0) {
                    Vid1.setFrame(clipCurFrame);
                    Vid1.play();
                    Vid2.setPaused(true);
                } else if (curActive == 1) {
                    Vid2.setFrame(clipCurFrame);
                    Vid2.play();
                    Vid1.setPaused(true);
                } else {
                    Vid1.setPaused(true);
                    Vid2.setPaused(true);
                }
            } else if(main->appName == "right") {
                if (curActive == 2) {
                    Vid1.setFrame(clipCurFrame);
                    Vid1.play();
                    Vid2.setPaused(true);
                } else if (curActive == 3) {
                    Vid2.setFrame(clipCurFrame);
                    Vid2.play();
                    Vid1.setPaused(true);
                } else {
                    Vid1.setPaused(true);
                    Vid2.setPaused(true);
                }
            }
        }
        
        frameRepeat += int((*main->totalTv2pos - prevTv2totPos)/3);
        
        // so it never loops more frames than till the end of the current video
        if (frameRepeat > (numFrames[curActive] - clipCurFrame)) {
            frameRepeat = numFrames[curActive] - clipCurFrame;
        } else if (frameRepeat < 2){
            frameRepeat = 2;
        }
        
        if (main->appName == "left") {
            if (curActive == 0 && Vid1.getCurrentFrame() >= frameRepeat + clipCurFrame) {
                Vid1.setFrame(clipCurFrame);
            } else if (curActive == 1 && Vid2.getCurrentFrame() >= frameRepeat + clipCurFrame) {
                Vid2.setFrame(clipCurFrame);
            }
        } else if (main->appName == "right") {
            if (curActive == 2 && Vid1.getCurrentFrame() >= frameRepeat + clipCurFrame) {
                Vid1.setFrame(clipCurFrame);
            } else if (curActive == 3 && Vid2.getCurrentFrame() >= frameRepeat + clipCurFrame) {
                Vid2.setFrame(clipCurFrame);
            }
        }
        
        prevTv1pos = *main->tv1pos;
        prevTv2Pos = *main->tv2pos;
        
        prevTv1totPos = *main->totalTv1pos;
        prevTv2totPos = *main->totalTv2pos;
        
        // just for now to show the end of an interactive event can be triggered by time
        // this should be the last thing you do in an update!!!!!!!
        //    if (durTime > 10000) {
        //        endOfMini();
        //    }
    }
}

//--------------------------------------------------------------
void intentie_interpretatie::draw(){
    if (main->appName != "middle") {
        ofBackground(20, 200, 20);
        int tempWidth = 0;
        main->scaleByHeight(Vid1.getWidth(), Vid1.getHeight(), &tempWidth, main->client->getLHeight());
        ofPushView();
            ofViewport(viewport1);
            ofSetupScreen();
            glScalef(1, -1, 1);
            glTranslatef(0, -main->client->getLHeight(), 0);
        
            if (Vid1.isLoaded()) {
                ofSetColor(255, 255, 255);
                Vid1.draw(0 - ((tempWidth - viewport1.width)/2), 0, tempWidth, main->client->getLHeight());
            }
        ofPopView();
        
        //--

        tempWidth = 0;
        main->scaleByHeight(Vid2.getWidth(), Vid2.getHeight(), &tempWidth, main->client->getLHeight());
        ofPushView();
        ofViewport(viewport2);
        ofSetupScreen();
        glScalef(1, -1, 1);
        glTranslatef(0, -main->client->getLHeight(), 0);
        
        if (Vid2.isLoaded()) {
            ofSetColor(255, 255, 255);
            Vid2.draw(0 - ((tempWidth - viewport2.width)/2), 0, tempWidth, main->client->getLHeight());
        }
        ofPopView();
        
        ofSetColor(255, 0, 0);
        ofRect(0, 0, 150, 150);
    } else { // Middle app
        ofBackground(0, 0, 0);
    }
}

//--------------------------------------------------------------
void intentie_interpretatie::exit(){
    Vid1.close();
    Vid2.close();
}

//--------------------------------------------------------------
void intentie_interpretatie::endOfMini(){
    main->done();
}