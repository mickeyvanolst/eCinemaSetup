//
//  01_TestMini.cpp
//  eCinemaSetup
//
//  Created by Mick van Olst on 13-11-12.
//
//

#include "01_TestMini.h"

//--------------------------------------------------------------
testMini::testMini(mainMini *_mai){
    main = _mai;
}

//--------------------------------------------------------------
void testMini::setup(){
    initTime = ofGetElapsedTimeMillis();
    
    // add a "randomly" placed ball
    Ball* ball = new Ball(ofRandom(0, main->client->getMWidth()), ofRandom(0, main->client->getMHeight()), main->client->getMWidth(), main->client->getMHeight());
    balls.push_back(ball);
}

//--------------------------------------------------------------
void testMini::update(){
    durTime = ofGetElapsedTimeMillis() - initTime;
    
    for (int i = 0; i < balls.size(); i++) {
        balls[i]->calc();
    }
    
    // just for now to show the end of an interactive event can be triggered by time
    // this should be the last thing you do in an update!!!!!!!
    if (durTime > 10000) {
        endOfMini();
    }
}

//--------------------------------------------------------------
void testMini::draw(){
    ofBackground(200, 200, 20);
// move and draw all the balls
    for (int i = 0; i < balls.size(); i++) {
        balls[i]->draw();
    }
}

//--------------------------------------------------------------
void testMini::endOfMini(){
    main->done();
}