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
    customPlayer();
    
    void init(handleChapters *_rea);
    void setup(string _name);
    void update();
    void draw(int x, int y);
    
    // video functions
    void addPlayer(string videoDir);
    void addAllVideos(int & i);
    void removeAllPlayers();
    void pausePlayer();
    void stopPlayer();
    void playPlayer();
    void startPlayer(int whichVid);
    
    vector<ofVideoPlayer> players;
    
    int                 activeVid;
    bool                isPlaying;
    bool                isDone;
    string              myAppName;
    
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
