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
void mainMini::init(mpeClientTCP *_cli, bool *_bOsc, float *_tv1rotVal, float *_tv2rotVal, float *_tv1rotTotVal, float *_tv2rotTotVal){
    client = _cli;
    bOsc = _bOsc;
    
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
float mainMini::sortaModulo(float timesFit, float totalVal){
    float returnVal = 0.0;
    if (timesFit <= totalVal) {
        while (returnVal + timesFit < totalVal) {
            returnVal += timesFit;
        }
        return returnVal;
    } else {
        return returnVal;
    }
}

//--------------------------------------------------------------
void mainMini::scaleByHeight(int oW, int oH, int *nW, int nH){
    //original width / original height x new height = new width
    float newVal = (float(oW) / float(oH)) * float(nH);
    (*nW) = int(newVal);
}

//--------------------------------------------------------------
void mainMini::scaleByWidth(int oW, int oH, int nW, int *nH){
    //original height / original width x new width = new height
    float newVal = (float(oH) / float(oW)) * float(nW);
    (*nH) = int(newVal);
}

//--------------------------------------------------------------
void mainMini::done(){
    int myInt;
    ofNotifyEvent(doneEvent,myInt,this); // trigger listener at miniHandler which will execute stopMini
    printf("notify miniApp done to miniHandler\n");
}
