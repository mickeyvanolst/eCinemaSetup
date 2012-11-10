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
#include "myGUI.h"

class customPlayer {
    
public:
    customPlayer(handleChapters *_rea);
    
    void setup(string ID);
    void update();
    void draw(int x, int y, int w, int h);
    // video functions
    void addPlayer(string videoDir);
    void addAllVideos(int & i);
    void removeAllPlayers();
    void pausePlayer();
    void startPlayer(int whichVid);
    void syphonOut();
    
    struct Players {
        //ofxQTKitVideoPlayer vid;
        ofVideoPlayer vid;
    };
    vector<Players>      players;
    
    string              appName;
    int                 activeVid;
    bool                isPlaying;
    
    handleChapters      *reader;
    
    
    // some general values for easy access
    float           totalProgress;
    bool            syphonLaOn;
    bool            syphonRaOn;

    float           chapCurPercent;
    float           chapCurTime;
    float           chapTotalTime;
    
private:
    
};

#endif
