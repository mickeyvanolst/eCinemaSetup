//
//  allHandler.cpp
//  eCinemaSetup
//
//  Created by Mick van Olst on 17-11-12.
//
//

#include "allHandler.h"

//--------------------------------------------------------------
allHandler::allHandler(){
    //player      = new customPlayer(reader);
    //miniApp     = new miniHandler(client);

    activeID = -1; // negative so at least we know something's wrong
    bPlayAll = true;
}


//--------------------------------------------------------------
void allHandler::init(mpeClientTCP *_cli, handleChapters *_rea, ofxOscSender * _osc, float *_tv1rotVal, float *_tv2rotVal, float *_tv1rotTotVal, float *_tv2rotTotVal){
    client  = _cli;
    reader  = _rea;
    oscOut = _osc;
    player.init(_rea);
    miniApp.init(_cli, _osc, &bOsc, _tv1rotVal, _tv2rotVal, _tv1rotTotVal, _tv2rotTotVal);
}


//--------------------------------------------------------------
void allHandler::setup(string appName){
    
    appName = appName;
    
    // only send OSC (for audio sync) from left app
    if (appName == "left") {
        bOsc = true;
    } else {
        bOsc = false;
    }

    // customp player, also needs to know who he is
    player.setup(appName);
    
    // our miniApp handler, this should handle all interactive chapters
    miniApp.setup(appName);
    
    // adding listeners (still have to add this)
    ofAddListener(miniApp.doneEvent, this, &allHandler::done);
    ofAddListener(player.doneEvent, this, &allHandler::done);
}

//--------------------------------------------------------------
void allHandler::createList(){
    // fill up our list with all apps and video names and types
    int numChap = reader->chapters.size();
    int curChap = 0;
    int numApps = 0;
    for (int curChap = 0; curChap < numChap; curChap++) {
        list.push_back(TotProg());
        
        list[curChap+numApps].name = reader->chapters[curChap].name;
        list[curChap+numApps].type = "mov";
        list[curChap+numApps].chapterID = curChap;
        
        // checks after every movie if there should be an app next
        if (miniApp.appComesAfter(reader->chapters[curChap].name) != "") {
            numApps++;
            list.push_back(TotProg());
            list[curChap+numApps].name = miniApp.appComesAfter(reader->chapters[curChap].name);
            list[curChap+numApps].type = "app";
        }
    }
    
    for (int i = 0; i < list.size(); i++) {
        printf("[%i] %s - %s\n", i,list[i].name.c_str(), list[i].type.c_str());
    }
    
    
    // init first item, otherwise some classes won't be happy..
    start(list[0].name);
    pause();


    
    
    int myInt;
    ofNotifyEvent(buildGUIEvent,myInt,this);
}

//--------------------------------------------------------------
void allHandler::update(){
    ofxOscMessage m;
    if (bOsc) {
        m.setAddress("chapter");
        m.addIntArg(activeID);    
    }
    if (list[activeID].type.compare("app") == 0) {
        // this is an app
        miniApp.update();
    } else if(list[activeID].type.compare("mov") == 0) {
        // this is a movie
        player.update();
    }
    if (bOsc) {
        oscOut->sendMessage(m);
    }
}

//--------------------------------------------------------------
void allHandler::draw(){
    if (list[activeID].type.compare("app") == 0) {
        // this is an app
        miniApp.draw();
    } else if(list[activeID].type.compare("mov") == 0) {
        // this is a movie
        player.draw(client->getXoffset(),client->getYoffset());
    }
}

//--------------------------------------------------------------
void allHandler::setActive(int listID){
    // iterating through the list to set only the right one to active = true and the rest to active = false
    for (int i = 0; i < list.size(); i++) {
        if (i == listID) {
            list[i].active = true;
            activeID = i;
        } else {
            list[i].active = false;
        }
    }
}

//--------------------------------------------------------------
void allHandler::start(string name){
    // checking if the app/video you wanna start exists and set it as active
    bool notFound = true;
    for (int i = 0; i < list.size(); i++) {
        if (name.compare(list[i].name) == 0) {
            // stop video/app that is currently playing
            
            if (activeID != -1) {
                if (list[activeID].type.compare("app") == 0) {
                    // this is an app
                    int myInt; // bleh, ofListener stuff..
                    miniApp.stopMini(myInt);
                } else if(list[activeID].type.compare("mov") == 0) {
                    // this is a movie
                    player.stopPlayer();
                }
            }
            
            // set the new activeID
            setActive(i);
            notFound = false;
        }
    }
    if (notFound) {
        printf("allHandler-start: the requested app/movie has not been found\n");
        return false; // does this work right?
    }
    
    // actually starting something --
    if (list[activeID].type.compare("app") == 0) {
        // this is an app
        miniApp.startMini(list[activeID].name);
    } else if(list[activeID].type.compare("mov") == 0) {
        // this is a movie
        printf("film name: %s\n", list[activeID].name.c_str());
        player.startPlayer(list[activeID].chapterID);
    }
    printf("starting: %s - id:%i type: %s\n",list[activeID].name.c_str(), activeID, list[activeID].type.c_str());
}

//--------------------------------------------------------------
void allHandler::startNext(){
    if (activeID < list.size()-1) {
        start(list[activeID+1].name);
        //printf("startNext: %s\n",list[activeID+1].name.c_str());
    }
}

//--------------------------------------------------------------
void allHandler::startPrev(){
    if (activeID > 0) {
        start(list[activeID-1].name);
    }
}

//--------------------------------------------------------------
void allHandler::pause(){
    if (list[activeID].type.compare("app") == 0) {
        // this is an app
        miniApp.pauseMini();
    } else if(list[activeID].type.compare("mov") == 0) {
        // this is a movie
        player.pausePlayer();
    }
}

//--------------------------------------------------------------
void allHandler::resume(){
    if (list[activeID].type.compare("app") == 0) {
        // this is an app
        miniApp.playMini();
    } else if(list[activeID].type.compare("mov") == 0) {
        // this is a movie
        player.playPlayer();
    }
}

//--------------------------------------------------------------
void allHandler::stop(){
    
}

//--------------------------------------------------------------
void allHandler::done(int & i){
    printf("I AM DONE!\n");
    if (bPlayAll) {
        client->broadcast("next,1");
        //startNext();
    }
}

