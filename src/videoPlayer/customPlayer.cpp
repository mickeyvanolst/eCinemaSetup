//
//  customPlayer.cpp
//  eCinemaSetup
//
//  Created by Mick van Olst on 07-11-12.
//
//

#include "customPlayer.h"

//--------------------------------------------------------------
customPlayer::customPlayer()
{
    ofSetVerticalSync(true);
    ofEnableAlphaBlending();
}

//--------------------------------------------------------------
void customPlayer::setup(string ID)
{
    clientID = ID;
}

//--------------------------------------------------------------
void customPlayer::update()
{
    for (int i = 0; i < players.size(); i++) {
        players[i].vid.idleMovie();
    }
}

//--------------------------------------------------------------
void customPlayer::addPlayer(string videoDir)
{
    printf("adding player: %s\n",videoDir.c_str());
    players.push_back(Players());
    players[players.size()-1].vid.loadMovie(videoDir);
    players[players.size()-1].vid.stop();
}

//--------------------------------------------------------------
void customPlayer::draw()
{
    for (int i = 0; i < players.size(); i++) {
        players[i].vid.draw(0,0);
    }
}

//--------------------------------------------------------------
void customPlayer::playPlayer()
{
    
}

//--------------------------------------------------------------
void customPlayer::pausePlayer()
{
    
}

//--------------------------------------------------------------
void customPlayer::removeAllPlayers()
{
    for (int i = 0; i < players.size(); i++) {
        
    }
}

//--------------------------------------------------------------
void customPlayer::syphonOut()
{
//    if (clientID == "left" || clientID == "right") {
//        // thesse guys should be having syphon on be default. Can be toggled from the GUI
//    }
}