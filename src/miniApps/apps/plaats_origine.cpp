//
//  plaats_origine.cpp
//  eCinemaSetup
//
//  Created by Mick van Olst on 01-12-12.
//
//

#include "plaats_origine.h"

//--------------------------------------------------------------
plaats_origine::plaats_origine(mainMini *_mai){
    main = _mai;
}

//--------------------------------------------------------------
void plaats_origine::setup(){
    initTime = ofGetElapsedTimeMillis();

    /*
    ofSetLogLevel(OF_LOG_VERBOSE);
	ofSetVerticalSync(true);
    
    files.listDir("app_content/plaats_origine/art/");
    files.sort();
    
	for(int i = 0; i < files.numFiles(); ++i) {
		ofImage* img = new ofImage();
		artImg.push_back(img);
		loader.loadFromDisk(img, files.getPath(i));
	}
    
    files.listDir("app_content/plaats_origine/world/");
    files.sort();
    
	for(int i = 0; i < files.numFiles(); ++i) {
		ofImage* img = new ofImage();
		worldImg.push_back(img);
		loader.loadFromDisk(img, files.getPath(i));
	}
    
    loader.startThread(false, false);
     */
    
    ofBackground(255, 225, 255);
    
    ofQTKitDecodeMode decodeMode = OF_QTKIT_DECODE_PIXELS_AND_TEXTURE;
    
    tBg = 0;
    bgMov.setPixelFormat(OF_PIXELS_BGRA);
    
    string bgString = "";
    if (main->appName.compare("left") == 0) {
        bgString = "L_BG.mp4";
    } else if (main->appName.compare("middle") == 0) {
        bgString = "M_BG.mp4";
    } else if (main->appName.compare("right") == 0) {
        bgString = "R_BG.mp4";
    }
    
    bgMov.loadMovie("app_content/plaats_origine/" + bgString, decodeMode);
    curBg = 0; // false
    bgMov.setPaused(true);
    
    if (main->appName == "middle") {
        tArt = 0;
        artMov.setPixelFormat(OF_PIXELS_RGBA);
        artMov.loadMovie("app_content/plaats_origine/kunst.mov", decodeMode);
        artMov.setPaused(true);
    }
    
}

//--------------------------------------------------------------
void plaats_origine::update(){
    durTime = ofGetElapsedTimeMillis() - initTime;
    Tweener.update();
    
    
//    bgMov.update();
//    Tweener.addTween(tBg, main->totalTv2pos, 5);
//    
//    float numRotVal = 0;
//    while (numRotVal + 360 < main->totalTv2pos) {
//        numRotVal += 360;
//    }
//    
//    tBg = tBg - numRotVal;    
//    tBg = ofMap(tBg, 0, 360, 0, 1);
//
//    bgMov.setPosition(tBg);
    
    bgMov.update();
    //printf("tv2pos: %f\n", main->tv2pos);
    if (main->tv2pos >= 0 && main->tv2pos < 90 && curBg != 1) {
        tweenTo = 60;
        printf("vliegveld\n");
        curBg = 1;
    } else if(main->tv2pos > 90 && main->tv2pos < 180 && curBg != 2) {
        tweenTo = 160;
        printf("rome\n");
        curBg = 2;
    } else if(main->tv2pos > 180 && main->tv2pos < 270 && curBg != 3){
        tweenTo = 270;
        printf("voetbal\n");
        curBg = 3;
    } else if(main->tv2pos > 270 && curBg != 4){
        tweenTo = 370;
        printf("straat\n");
        curBg = 4;
    }
    if (tBg != tweenTo) {
        Tweener.addTween(tBg, tweenTo, 5);
    }
    
    if (bgMov.getPosition() != tweenTo) {
        float numRotMVal = 0;
        while (numRotMVal + 360 < main->totalTv2pos) {
            numRotMVal += 360;
        }
        
        //tArt = tArt - numRotMVal;
        
        tBg = ofMap(tBg, 0, 360, 0, 1);
        bgMov.setPosition(tBg); // value between 0-1
        //printf("curBgFr: %i\n",bgMov.getCurrentFrame());
    }
    
    
    
    if (main->appName == "middle") {
        artMov.update();
        //printf("tv1pos: %f\n", main->tv1pos);
        if (main->tv1pos > 0 && main->tv1pos < 120 && curArt != 1) {
            Tweener.addTween(tArt, 120, 5);
            printf("mona\n");
            // mona - 38
            curArt = 1;
        } else if(main->tv1pos > 120 && main->tv1pos < 240 && curArt != 2) {
            Tweener.addTween(tArt, 240, 5);
            printf("mondriaan\n");
            // mondriaan - 68
            curArt = 2;
        } else if(main->tv1pos > 240 && curArt != 3){
            Tweener.addTween(tArt, 0, 5);
            printf("denker\n");
            // denker - 0
            curArt = 3;
        }
        
        if (tArt != artMov.getPosition()) {
            float numRotMVal = 0;
            while (numRotMVal + 360 < main->totalTv1pos) {
                numRotMVal += 360;
            }
            
            //tArt = tArt - numRotMVal;
            
            tArt = ofMap(tArt, 0, 360, 0, 1);
            artMov.setPosition(tArt); // value between 0-1
        }
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
    /*
    for (int i = 0; i < worldImg.size(); i++) {
        int xPos = 0;
        
        for (int f = 0; f < i; f++) {
            int tempWidth;
            scaleByHeight(worldImg.at(f)->getWidth(), worldImg.at(f)->getHeight(), &tempWidth, main->client->getMHeight());
            xPos += tempWidth;
        }
        
        int newWidth;
        scaleByHeight(worldImg.at(0)->getWidth(), worldImg.at(0)->getHeight(), &newWidth, main->client->getMHeight());
        if (isImageInViewport(xPos+(main->totalTv1pos*2), newWidth)) {
            worldImg.at(i)->draw(xPos+(main->totalTv1pos*2),0,newWidth,main->client->getMHeight());
        }
    }
     */
    
    
    
    if (bgMov.isLoaded()) {
        ofSetColor(255, 255, 255);
        bgMov.draw(main->client->getXoffset(), main->client->getYoffset());
    }
    
    if (artMov.isLoaded() && main->appName == "middle") {
        artMov.draw(main->client->getLWidth(), main->client->getYoffset());
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
void plaats_origine::endOfMini(){
    main->done();
}