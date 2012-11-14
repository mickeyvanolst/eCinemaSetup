//
//  mainMini.cpp
//  eCinemaSetup
//
//  Created by Mick van Olst on 13-11-12.
//
//

#include "mainMini.h"

//--------------------------------------------------------------
mainMini::mainMini(mpeClientTCP *_cli){
    client = _cli;
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
}
