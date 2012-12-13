//
//  mainMini.cpp
//  eCinemaSetup
//
//  Created by Mick van Olst on 13-11-12.
//
//

#include "mainMini.h"

//--------------------------------------------------------------
mainMini::mainMini(){
    
}

//--------------------------------------------------------------
void mainMini::init(mpeClientTCP *_cli){
    client = _cli;
}

//--------------------------------------------------------------
void mainMini::setup(string id){
    appName         = id;
    tv1pos          = 0;
    tv2pos          = 0;
    totalTv1pos     = 0;
    totalTv2pos     = 0;
    totalTv1prevPos = totalTv1pos;
    totalTv2prevPos = totalTv2pos;
}

//--------------------------------------------------------------
void mainMini::reset(){
    
}

//--------------------------------------------------------------
void mainMini::update(){
//    tweener.update();
    // gets updated through miniHandler class
//    tweener.addTween(tv1speed, totalTv1pos - totalTv1prevPos, 1);
//    tweener.addTween(tv2speed, totalTv2pos - totalTv2prevPos, 1);
//    
//    totalTv1prevPos = totalTv1pos;
//    totalTv2prevPos = totalTv2pos;
    
    //printf("tv1speed: %f\n", tv1speed);
}

//--------------------------------------------------------------
void mainMini::draw(){

}

//--------------------------------------------------------------
void mainMini::done(){
    int myInt;
    ofNotifyEvent(doneEvent,myInt,this); // trigger listener at miniHandler which will execute stopMini
    printf("notify miniApp done to miniHandler\n");
}
