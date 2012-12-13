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
void plaats_origine::init(mainMini *_mai){
    main = _mai;
}

//--------------------------------------------------------------
void plaats_origine::setup(){
    initTime = ofGetElapsedTimeMillis();
    
    loadXML("app_content/plaats_origine/cueponts.xml", true);
    
    ofQTKitDecodeMode decodeMode = OF_QTKIT_DECODE_PIXELS_AND_TEXTURE;
    
    bgMov.loadMovie("app_content/plaats_origine/" + main->appName + ".mov", decodeMode);
    bgMov.setPixelFormat(OF_PIXELS_RGB);
    tBg = 0;
    curBg = 0;
    Tweener.addTween(tBg, bgCuePoints[curBg].frame, 5);
    
    if (main->appName == "middle") {
        artMov.loadMovie("app_content/plaats_origine/kunst.mov", decodeMode);
        artMov.setPixelFormat(OF_PIXELS_BGRA);
        tArt = 0;
        curArt = 0;
        Tweener.addTween(tArt, artCuePoints[curArt].frame, 5);
    }
}

//--------------------------------------------------------------
void plaats_origine::update(){
    durTime = ofGetElapsedTimeMillis() - initTime;
    Tweener.update();
    
    for (int i = 0; i < bgCuePoints.size(); i++) {
        if (curBg != i && main->tv1pos >= i*(360/bgCuePoints.size()) && main->tv1pos < (i+1)*(360/bgCuePoints.size())) {
            
            if (bgCuePoints[i].frame - tBg > bgMov.getTotalNumFrames()/2) {
                output = "go down";
                tBg += bgMov.getTotalNumFrames();
                Tweener.addTween(tBg,bgCuePoints[i].frame, 5);
            } else if(bgCuePoints[i].frame +(bgMov.getTotalNumFrames()-tBg) < bgMov.getTotalNumFrames()/2) {
                output = "go up";
                Tweener.addTween(tBg,bgCuePoints[i].frame + bgMov.getTotalNumFrames(), 5);
            } else {
                output = "normal";
                Tweener.addTween(tBg,bgCuePoints[i].frame, 5);
            }
            curBg = i;
        }
    }
    
    if (tBg != bgMov.getCurrentFrame()) {
        if (tBg > bgMov.getTotalNumFrames()) {
            tBg = tBg - sortaModulo(bgMov.getTotalNumFrames()-1, tBg);
            // take off -1 of totalframes in case cuepoint is zero
        } 
        bgMov.setFrame(int(tBg));
    }
    bgMov.update();
    
    
    if (main->appName == "middle") {
        for (int i = 0; i < artCuePoints.size(); i++) {
            if (curArt != i && main->tv2pos >= i*(360/artCuePoints.size()) && main->tv2pos < (i+1)*(360/artCuePoints.size())) {
                
                if (artCuePoints[i].frame - tArt > artMov.getTotalNumFrames()/2) {
                    output = "go down";
                    tArt += artMov.getTotalNumFrames();
                    Tweener.addTween(tArt,artCuePoints[i].frame, 1, &ofxTransitions::easeInSine);
                } else if(artCuePoints[i].frame +(artMov.getTotalNumFrames()-tArt) < artMov.getTotalNumFrames()/2) {
                    output = "go up";
                    Tweener.addTween(tArt,artCuePoints[i].frame + artMov.getTotalNumFrames(), 1, &ofxTransitions::easeInSine);
                } else {
                    output = "normal";
                    Tweener.addTween(tArt,artCuePoints[i].frame, 1, &ofxTransitions::easeInSine);
                }
                curArt = i;
            }
        }
        
        if (tArt != artMov.getCurrentFrame()) {
            if (tArt > artMov.getTotalNumFrames()) {
                tArt = tArt - sortaModulo(artMov.getTotalNumFrames()-1, tArt);
                // take off -1 of totalframes in case cuepoint is zero
            }
            artMov.setFrame(int(tArt));
        }
        artMov.update();
    }
    
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
        artMov.draw(main->client->getXoffset(),0);
        
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
void plaats_origine::scaleByHeight(int oW, int oH, int *nW, int nH){
    //original width / original height x new height = new width
    float newVal = (float(oW) / float(oH)) * float(nH);
    (*nW) = int(newVal);
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
float plaats_origine::sortaModulo(float timesFit, float totalVal){
    float returnVal = 0.0;
    while (returnVal + timesFit < totalVal) {
        returnVal += timesFit;
    }
    return returnVal;
}

//--------------------------------------------------------------
void plaats_origine::loadXML(string file, bool printResult){
    if (!XML.loadFile(file)) {
        printf("failed loading cuepoints.xml\n");
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
void plaats_origine::endOfMini(){
    main->done();
}