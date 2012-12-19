//
//  plaats_origine.cpp
//  eCinemaSetup
//
//  Created by Mick van Olst on 01-12-12.
//
//

#include "plaats_origine.h"

//--------------------------------------------------------------
plaats_origine::plaats_origine(){
    
}

//--------------------------------------------------------------
void plaats_origine::init(mainMini *_mai, ofxMidiOut *_midi){
    main = _mai;
    midiOut = _midi;
}

//--------------------------------------------------------------
void plaats_origine::setup(){
    initTime = ofGetElapsedTimeMillis();
    
    ofClear(255, 255, 255);
    ofBackground(0, 0, 0);
    
    loadXML("app_content/plaats_origine/cueponts.xml", true);
    
    ofQTKitDecodeMode decodeMode = OF_QTKIT_DECODE_PIXELS_AND_TEXTURE;
    
    bgMov.loadMovie("app_content/plaats_origine/" + main->appName + ".mov", decodeMode);
    bgMov.setPixelFormat(OF_PIXELS_RGB);
    tBg = 0;
    curBg = 0;
    Tweener.addTween(tBg, bgCuePoints[curBg].frame, 1, &ofxTransitions::linear);
    
    if (main->appName == "middle") {
        artMov.loadMovie("app_content/plaats_origine/kunst.mov", decodeMode);
        artMov.setPixelFormat(OF_PIXELS_BGRA);
        tArt = 0;
        curArt = 0;
        Tweener.addTween(tArt, artCuePoints[curArt].frame, 1, &ofxTransitions::linear);
    }
}

//--------------------------------------------------------------
void plaats_origine::update(){
    durTime = ofGetElapsedTimeMillis() - initTime;
    
    for (int i = 0; i < bgCuePoints.size(); i++) {
        if (curBg != i && *main->tv1pos >= i*(360/bgCuePoints.size()) && *main->tv1pos < (i+1)*(360/bgCuePoints.size())) {
            
            if (bgCuePoints[i].frame - tBg > bgMov.getTotalNumFrames()/2) {
                output = "go down";
                tBg += bgMov.getTotalNumFrames();
                Tweener.addTween(tBg,bgCuePoints[i].frame, 1, &ofxTransitions::linear);
            } else if(bgCuePoints[i].frame +(bgMov.getTotalNumFrames()-tBg) < bgMov.getTotalNumFrames()/2) {
                output = "go up";
                Tweener.addTween(tBg,bgCuePoints[i].frame + bgMov.getTotalNumFrames(), 1, &ofxTransitions::linear);
            } else {
                output = "normal";
                Tweener.addTween(tBg,bgCuePoints[i].frame, 1, &ofxTransitions::linear);
            }
            curBg = i;
        }
    }
    
    if (tBg != bgMov.getCurrentFrame()) {
        if (tBg > bgMov.getTotalNumFrames()) {
            tBg = tBg - main->sortaModulo(bgMov.getTotalNumFrames()-1, tBg);
            // take off -1 of totalframes in case cuepoint is zero
        } 
        //bgMov.setFrame(int(tBg));
        
        float pos = ofMap(int(tBg), 0, bgMov.getTotalNumFrames(), 0, 1);
        bgMov.setPosition(pos);
        cout << "tBg: " << tBg << "\n";
        cout << "curFrame: " << bgMov.getCurrentFrame() << "\n";
    }
    bgMov.update();
    
    
    if (main->appName == "middle") {
        for (int i = 0; i < artCuePoints.size(); i++) {
            if (curArt != i && *main->tv2pos >= i*(360/artCuePoints.size()) && *main->tv2pos < (i+1)*(360/artCuePoints.size())) {
                
                if (artCuePoints[i].frame - tArt > artMov.getTotalNumFrames()/2) {
                    output = "go down";
                    tArt += artMov.getTotalNumFrames();
                    Tweener.addTween(tArt,artCuePoints[i].frame, 1, &ofxTransitions::linear);
                } else if(artCuePoints[i].frame +(artMov.getTotalNumFrames()-tArt) < artMov.getTotalNumFrames()/2) {
                    output = "go up";
                    Tweener.addTween(tArt,artCuePoints[i].frame + artMov.getTotalNumFrames(), 1, &ofxTransitions::linear);
                } else {
                    output = "normal";
                    Tweener.addTween(tArt,artCuePoints[i].frame, 1, &ofxTransitions::linear);
                }
                curArt = i;
            }
        }
        
        if (tArt != artMov.getCurrentFrame()) {
            if (tArt > artMov.getTotalNumFrames()) {
                tArt = tArt - main->sortaModulo(artMov.getTotalNumFrames()-1, tArt);
                // take off -1 of totalframes in case cuepoint is zero
            }
            artMov.setFrame(int(tArt));
        }
        artMov.update();
    }
    
    Tweener.update();
    
    // just for now to show the end of an interactive event can be triggered by time
    // this should be the last thing you do in an update!!!!!!!
//    if (durTime > 10000) {
//        endOfMini();
//    }
}

//--------------------------------------------------------------
void plaats_origine::draw(){
    ofBackground(100, 200, 20);
    
    bgMov.draw(main->client->getXoffset(), 0);
    
    if (main->appName == "middle") {
        ofEnableAlphaBlending();
        artMov.draw(main->client->getXoffset(),0);
        ofDisableAlphaBlending();
        bool dispTestMeter = true;
        if (dispTestMeter) {
            ofPushMatrix();
            ofTranslate(main->client->getXoffset()+100, 400);
            ofSetColor(255, 0, 0);
            ofRect(0, 0, artMov.getTotalNumFrames(), 5);
            ofSetColor(0, 0, 0);
            for (int i = 0; i < artCuePoints.size(); i++) {
                ofRect(artCuePoints[i].frame, 0, 1, 10);
            }
            ofSetColor(255, 255, 255);
            ofRect(tArt, 0, 1, 5);
            
            ofDrawBitmapString(output, 0, 50);
            ofPopMatrix();
            
            ofPushMatrix();
            ofTranslate(main->client->getXoffset()+100, 600);
            ofSetColor(255, 255, 0);
            ofRect(0, 0, bgMov.getTotalNumFrames(), 5);
            ofSetColor(0, 0, 0);
            for (int i = 0; i < bgCuePoints.size(); i++) {
                ofRect(bgCuePoints[i].frame, 0, 1, 10);
            }
            ofSetColor(255, 0, 255);
            ofRect(tBg, 0, 1, 5);
            ofPopMatrix();
            
        }
    }
}

//--------------------------------------------------------------
bool plaats_origine::isImageInViewport(int x, int w){
    if ((x+w) > 0 && x < main->client->getMWidth() ) {
        return true;
    } else {
        return false;
    }
}

//--------------------------------------------------------------
void plaats_origine::loadXML(string file, bool printResult){
    if (!XML.loadFile(file)) {
        cout << "failed loading " << file << "\n";
    } else {
        XML.pushTag("cuepoints");
        
        // background XML
        XML.pushTag("background");
        int numCuePt = XML.getNumTags("cuepoint");
        
        for (int i = 0; i < numCuePt; i++) {
            XML.pushTag("cuepoint",i);
            bgCuePoints.push_back(Cuepoints());
            bgCuePoints.back().name     = XML.getValue("name","");
            bgCuePoints.back().frame    = XML.getValue("frame", 0);
            XML.popTag();
        }
        XML.popTag();
        if (printResult) {
            for (int b = 0; b < bgCuePoints.size(); b++) {
                printf("cuepoint: %s\n",bgCuePoints[b].name.c_str());
            }
        }
        
        // artwork XML
        XML.pushTag("artwork");
        numCuePt = XML.getNumTags("cuepoint");
        
        for (int i = 0; i < numCuePt; i++) {
            XML.pushTag("cuepoint",i);
            artCuePoints.push_back(Cuepoints());
            artCuePoints.back().name     = XML.getValue("name","");
            artCuePoints.back().frame    = XML.getValue("frame", 0);
            XML.popTag();
        }
        XML.popTag();
        
        if (printResult) {
            for (int b = 0; b < artCuePoints.size(); b++) {
                printf("cuepoint: %s\n",artCuePoints[b].name.c_str());
            }
        }
    }
}

double plaats_origine::floor0( double value ){
    if (value < 0.0)
        return ceil( value );
    else
        return floor( value );
}

//--------------------------------------------------------------
void plaats_origine::exit(){
    bgMov.close();
    artMov.close();
}

//--------------------------------------------------------------
void plaats_origine::endOfMini(){
    main->done();
}