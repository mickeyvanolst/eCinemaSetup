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
}

//--------------------------------------------------------------
void plaats_origine::update(){
    durTime = ofGetElapsedTimeMillis() - initTime;

    
    
    // just for now to show the end of an interactive event can be triggered by time
    // this should be the last thing you do in an update!!!!!!!
//    if (durTime > 10000) {
//        endOfMini();
//    }
}

//--------------------------------------------------------------
void plaats_origine::draw(){
    ofBackground(100, 200, 20);
        
    for (int i = 0; i < worldImg.size(); i++) {
        int xPos = 0;
        
        for (int f = 0; f < i; f++) {
            int tempWidth;
            scaleByHeight(worldImg.at(f)->getWidth(), worldImg.at(f)->getHeight(), &tempWidth, main->client->getMHeight());
            xPos += tempWidth;
        }
        
        int newWidth;
        scaleByHeight(worldImg.at(0)->getWidth(), worldImg.at(0)->getHeight(), &newWidth, main->client->getMHeight());
        if (isImageInViewport(xPos, newWidth)) {
            worldImg.at(i)->draw(xPos+(main->totalTv1pos*2),0,newWidth,main->client->getMHeight());
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
    if ((x+w) > 0 && x < main->client->getLWidth() ) {
        return true;
    } else {
        return false;
    }
}

//--------------------------------------------------------------
void plaats_origine::endOfMini(){
    main->done();
}