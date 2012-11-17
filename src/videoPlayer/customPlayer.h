//
//  customPlayer.h
//  eCinemaSetup
//
//  Created by Mick van Olst on 07-11-12.
//
//

#ifndef eCinemaSetup_customPlayer_h
#define eCinemaSetup_customPlayer_h

#include "chapterHandler.h"

class customPlayer {
    
public:
    customPlayer(handleChapters *_rea);
    
    void setup(string ID);
    void update();
    void draw(int x, int y);
    
    // video functions
    void addPlayer(string videoDir);
    void addAllVideos(int & i);
    void removeAllPlayers();
    void pausePlayer();
    void stopPlayer();
    void playPlayer();
//    void nextPlayer();
//    void prevPlayer();
    void startPlayer(int whichVid);
    
    struct Players {
        ofVideoPlayer   vid;
    };
    vector<Players>     players;
    
    string              appName;
    int                 activeVid;
    bool                isPlaying;
    bool                isDone;
    
    handleChapters      *reader;
    
    // some general values for easy access
    float               totalProgress;
    bool                syphonLaOn;
    bool                syphonRaOn;

    float               chapCurPercent;
    float               chapCurTime;
    float               chapTotalTime;
    
    float               totPrevMovsFrames;
    float               totMovsFrames;
    
    ofEvent<int>        doneEvent;
    
private:
    
};

#endif
