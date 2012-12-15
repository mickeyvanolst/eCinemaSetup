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
void mainMini::init(mpeClientTCP *_cli, bool *_bMidi, float *_tv1rotVal, float *_tv2rotVal, float *_tv1rotTotVal, float *_tv2rotTotVal){
    client = _cli;
    bMidi = _bMidi;
    
    tv1pos          = _tv1rotVal;
    tv2pos          = _tv2rotVal;
    totalTv1pos     = _tv1rotTotVal;
    totalTv2pos     = _tv2rotTotVal;
    totalTv1prevPos = *totalTv1pos;
    totalTv2prevPos = *totalTv2pos;
    
}

//--------------------------------------------------------------
void mainMini::setup(string id){
    appName         = id;
}

//--------------------------------------------------------------
void mainMini::reset(){
    
}

//--------------------------------------------------------------
void mainMini::update(){
    
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
