//
//  customPlayer.cpp
//  eCinemaSetup
//
//  Created by Mick van Olst on 07-11-12.
//
//

#include "customPlayer.h"

//--------------------------------------------------------------
customPlayer::customPlayer(handleChapters *_rea)
{
    reader = _rea;
    
    ofSetVerticalSync(true);
    ofEnableAlphaBlending();
}

//--------------------------------------------------------------
void customPlayer::setup(string ID)
{
    appName = ID;
    ofAddListener(reader->addAllVideosEvent, this, &customPlayer::addAllVideos);
    
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
    if (reader->chapters.size() != players.size()) {
        players.resize(reader->chapters.size());
    }
    printf("adding player: %s\n",videoDir.c_str());
    players.push_back(Players());
    players[players.size()-1].vid.loadMovie(videoDir);
    players[players.size()-1].vid.stop();
}

//--------------------------------------------------------------
void customPlayer::addAllVideos(int & i)
{
    printf("adding all videos to customPlayer, first deleting what was there\n");
    removeAllPlayers();
    printf("still alive\n");
    printf("player size: %li\n",players.size());
    for (int i = 0; i < reader->chapters.size(); i++) {
        if(reader->chapters[i].complete && reader->chapters[i].inOrder){
            if (appName == "left") {
                addPlayer(reader->chapters[i].left.file);
            } else if(appName == "middle") {
                addPlayer(reader->chapters[i].middle.file);
            } else if(appName == "right") {
                addPlayer(reader->chapters[i].right.file);
            }
        }
        
    }
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
    //for (int i = 0; i < players.size(); i++) {
    printf("about to remove all players\n");
    players.erase(players.begin(), players.end());
    players.resize(0);
    printf("removed all players\n");
    //}
}

//--------------------------------------------------------------
void customPlayer::syphonOut()
{
//    if (clientID == "left" || clientID == "right") {
//        // thesse guys should be having syphon on be default. Can be toggled from the GUI
//    }
}