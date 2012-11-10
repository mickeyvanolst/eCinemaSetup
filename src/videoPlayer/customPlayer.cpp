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
    
    activeVid = 1;
    // I guess this should be the first movie by default
    // maybe change this to 0 to make it easier for array use?
    
    isPlaying = false;
    // some general boolean to let customplayer know something should be playing
    
    // set all values we've got on default mode
    totalProgress = 0.0;
    //appLaCon, appMaCon, appRaCon, appObj1Con, appObj2Con,
    syphonLaOn, syphonRaOn = false;
    
    chapCurPercent = 0.0;
    chapCurTime = 0.0;
    chapTotalTime = 0.0;
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
    players.push_back(Players());
    printf("adding movie: %s at players[%li]\n",videoDir.c_str(), players.size()-1);
    players[players.size()-1].vid.loadMovie(videoDir);
    players[players.size()-1].vid.stop();
}

//--------------------------------------------------------------
void customPlayer::addAllVideos(int & i)
{
    printf("adding all videos to customPlayer, first deleting what was there\n");
    removeAllPlayers();
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
    printf("new player size: %li\n",players.size());
}

//--------------------------------------------------------------
void customPlayer::draw(int x, int y, int w, int h)
{
    if(isPlaying) {
        players[activeVid-1].vid.update();
        
        chapCurPercent = players[activeVid-1].vid.getCurrentFrame() / players[activeVid-1].vid.getTotalNumFrames() * 100;
        chapCurTime = players[activeVid-1].vid.getDuration() / 100 * chapCurPercent;
        chapTotalTime = players[activeVid-1].vid.getDuration();
        
        players[activeVid-1].vid.draw(x,y,w,h);
    }
    // this is still a bit sketchy, not sure if I should update all video's in order to
    // keep commands nice and swift, or to only update the one thats playing for CPU's sake
    // option 2 for now..
}

//--------------------------------------------------------------
void customPlayer::startPlayer(int whichVid)
{
    pausePlayer();
    isPlaying = true;
    activeVid = whichVid;
    printf("customplayer: play vid nr: %i\n",activeVid);
    players[activeVid-1].vid.play();
    // pause the one that might be playing and play the new one
}

//--------------------------------------------------------------
void customPlayer::pausePlayer()
{
    if (isPlaying) {
        players[activeVid-1].vid.stop();
        isPlaying = false;
    }
}

//--------------------------------------------------------------
void customPlayer::removeAllPlayers()
{
    printf("about to remove all players\n");
    printf("total num players: %li\n",players.size());
    players.erase(players.begin(), players.end());
    players.clear();
    players.resize(0);
    printf("removed all players!\n");
}

//--------------------------------------------------------------
void customPlayer::syphonOut()
{

}