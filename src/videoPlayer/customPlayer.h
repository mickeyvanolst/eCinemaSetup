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

class customPlayer : ofBaseApp {
    
public:
    customPlayer();
    
    void setup(string ID);
    void update();
    void draw();
    // video functions
    void addPlayer(string videoDir);
    void removeAllPlayers();
    void pausePlayer();
    void playPlayer();
    
    void syphonOut();
    
    struct Players {
        ofxQTKitVideoPlayer vid;
    };
    vector<Players>      players;
    
    string              clientID;
    int                 activeVid;
    
private:
    
};

#endif
