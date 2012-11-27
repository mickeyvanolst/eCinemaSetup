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
    
    activeVid = 0;
    // I guess this should be the first movie by default
    // maybe change this to 0 to make it easier for array use?
    
    isPlaying = false;
    // some general boolean to let customplayer know something should be playing
    
    // set all values we've got on default mode
    totalProgress = 0.0;
    chapCurPercent = 0.0;
    chapCurTime = 0.0;
    chapTotalTime = 0.0;
    totPrevMovsFrames = 0.0;
    totMovsFrames = 0.0;
    isDone = false;
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
    players[activeVid].vid.update();
    //players[activeVid].vid.idleMovie(); - deprecated
    // send a message to allHandler the video is done
    
    
    if (players[activeVid].vid.getIsMovieDone() && !isDone) {
        int myInt;
        ofNotifyEvent(doneEvent,myInt,this);
        printf("customPlayer - video is done, sending notifyEvent\n");
        isDone = true;
        stopPlayer();
    }
}

//--------------------------------------------------------------
void customPlayer::addPlayer(string videoDir)
{
    players.push_back(Players());
    printf("adding movie: %s at players[%li]\n",videoDir.c_str(), players.size()-1);
    players[players.size()-1].vid.loadMovie(videoDir);
    players[players.size()-1].vid.setLoopState(OF_LOOP_NONE);
    stopPlayer();
    
    // calculate total amount of frames of all movies combined
    totMovsFrames = 0.0;
    for (int i = 0; i < players.size(); i++) {
        if(i >= 0) {
            totMovsFrames += float(players[i].vid.getTotalNumFrames());
        } else {
            totMovsFrames = 0.0;
        }
    }
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
void customPlayer::draw(int x, int y)
{
    // calulating stuff to show in the GUI
    float tempCurFrame      = players[activeVid].vid.getCurrentFrame();
    float tempTotFrame      = players[activeVid].vid.getTotalNumFrames();
    float tempPercent       = tempCurFrame / tempTotFrame * 100.0;
    float tempTotPercent    = (tempCurFrame + totPrevMovsFrames) / totMovsFrames * 100.0;
    
    chapTotalTime = int(players[activeVid].vid.getDuration() );
    chapCurPercent = tempPercent;
    totalProgress = tempTotPercent;
    
    // actually drawing the video
    players[activeVid].vid.draw(x,y);
    

    // this is still a bit sketchy, not sure if I should update all video's in order to
    // keep commands nice and swift, or to only update the one thats playing for CPU's sake
    // option 2 for now..
}

//--------------------------------------------------------------
void customPlayer::startPlayer(int whichVid)
{
    stopPlayer();
    isPlaying = true;
    isDone = false;
    activeVid = whichVid;
    printf("customplayer: play vid nr: %i\n",activeVid);
    //players[activeVid].vid.setFrame(1);
    players[activeVid].vid.play();
    // pause the one that might be playing and play the new one
    
    // calculate new value for total amount of frames of previous films
    totPrevMovsFrames = 0.0;
    for (int i = 0; i < activeVid; i++) {
        if(i >= 0) {
            totPrevMovsFrames += float(players[i].vid.getTotalNumFrames());
        } else {
            totPrevMovsFrames = 0.0;
        }
    }
}

//--------------------------------------------------------------
void customPlayer::pausePlayer()
{
    if (isPlaying) {
        players[activeVid].vid.setPaused(true);
        isPlaying = false;
    }
}

//--------------------------------------------------------------
void customPlayer::stopPlayer()
{
    if (isPlaying) {
        players[activeVid].vid.setPaused(true);
        players[activeVid].vid.setPosition(0);
        players[activeVid].vid.stop();
        isPlaying = false;
        printf("customPlayer - stopPlayer\n");
    }
}

//--------------------------------------------------------------
void customPlayer::playPlayer()
{
    if (!isPlaying) {
        players[activeVid].vid.play();
        isPlaying = true;
    }
}

//--------------------------------------------------------------
void customPlayer::removeAllPlayers()
{
    players.erase(players.begin(), players.end());
    players.clear();
    players.resize(0);
    printf("removed all players!\n");
    
    totPrevMovsFrames = 0;
}