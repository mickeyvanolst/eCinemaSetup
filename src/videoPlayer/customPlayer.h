//
//  customPlayer.h
//  eCinemaSetup
//
//  Created by Mick van Olst on 07-11-12.
//
//

#ifndef eCinemaSetup_customPlayer_h
#define eCinemaSetup_customPlayer_h

#include "ofxQTKitVideoPlayer.h"
#include "chapterHandler.h"

class customPlayer {
    
public:
    customPlayer(handleChapters *_rea);
    
    void setup(string ID);
    void update();
    void draw();
    // video functions
    void addPlayer(string videoDir);
    void addAllVideos(int & i);
    void removeAllPlayers();
    void pausePlayer();

    void playPlayer();
    
    void syphonOut();
    
    struct Players {
        ofxQTKitVideoPlayer vid;
    };
    vector<Players>      players;
    
    string              appName;
    int                 activeVid;
    
    handleChapters *reader;
    
private:
    
};

#endif
