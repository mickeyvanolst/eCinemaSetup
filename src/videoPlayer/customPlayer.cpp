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
    //reader = _rea;
    
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
void customPlayer::init(handleChapters *_rea)
{
    reader = _rea;
}

//--------------------------------------------------------------
void customPlayer::setup(string _name)
{
    myAppName = _name;
    ofAddListener(reader->addAllVideosEvent, this, &customPlayer::addAllVideos);
}

//--------------------------------------------------------------
void customPlayer::update()
{
    players[activeVid].update();
    //players[activeVid].vid.idleMovie(); - deprecated
    // send a message to allHandler the video is done
    
    if (players[activeVid].getIsMovieDone() && !isDone) {
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
    ofVideoPlayer vid;
    printf("adding movie: %s at players[%li]\n",videoDir.c_str(), players.size());
    vid.loadMovie(videoDir);
    vid.setLoopState(OF_LOOP_NONE);
    players.push_back(vid);
    stopPlayer();
    
    // calculate total amount of frames of all movies combined
    totMovsFrames = 0.0;
    for (int i = 0; i < players.size(); i++) {
        if(i >= 0) {
            totMovsFrames += float(players[i].getTotalNumFrames());
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
            if (myAppName == "left") {
                addPlayer(reader->chapters[i].left.file);
            } else if(myAppName == "middle") {
                addPlayer(reader->chapters[i].middle.file);
            } else if(myAppName == "right") {
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
    float tempCurFrame      = players[activeVid].getCurrentFrame();
    float tempTotFrame      = players[activeVid].getTotalNumFrames();
    float tempPercent       = tempCurFrame / tempTotFrame * 100.0;
    float tempTotPercent    = (tempCurFrame + totPrevMovsFrames) / totMovsFrames * 100.0;
    
    chapTotalTime = int(players[activeVid].getDuration() );
    chapCurPercent = tempPercent;
    totalProgress = tempTotPercent;
    
    // actually drawing the video
    players[activeVid].draw(x,y);
    

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
    players[activeVid].play();
    // pause the one that might be playing and play the new one
    
    // calculate new value for total amount of frames of previous films
    totPrevMovsFrames = 0.0;
    for (int i = 0; i < activeVid; i++) {
        if(i >= 0) {
            totPrevMovsFrames += float(players[i].getTotalNumFrames());
        } else {
            totPrevMovsFrames = 0.0;
        }
    }
}

//--------------------------------------------------------------
void customPlayer::pausePlayer()
{
    if (isPlaying) {
        players[activeVid].setPaused(true);
        isPlaying = false;
    }
}

//--------------------------------------------------------------
void customPlayer::stopPlayer()
{
    if (isPlaying) {
        players[activeVid].setPaused(true);
        players[activeVid].setPosition(0);
        players[activeVid].stop();
        isPlaying = false;
        printf("customPlayer - stopPlayer\n");
    }
}

//--------------------------------------------------------------
void customPlayer::playPlayer()
{
    if (!isPlaying) {
        players[activeVid].play();
        isPlaying = true;
    }
}

//--------------------------------------------------------------
void customPlayer::removeAllPlayers()
{
    // deleting the videoplayer instances we just used
    for (int i = 0; i < players.size(); i++) {
        players[i].close();
    }
    players.erase(players.begin(), players.end());
    printf("removed all players!\n");
    
    totPrevMovsFrames = 0;
}