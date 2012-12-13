//
//  allHandler.h
//  eCinemaSetup
//
//  Created by Mick van Olst on 17-11-12.
//
//

#include "ofMain.h"
#include "mpeClientTCP.h"
#include "chapterHandler.h"
#include "miniHandler.h"
#include "customPlayer.h"

#ifndef eCinemaSetup_allHandler_h
#define eCinemaSetup_allHandler_h

class allHandler {
public:
    allHandler(); 
    
    void init(mpeClientTCP *_cli, handleChapters *_rea);
    void setup(string appName);
    void createList();
    void update();
    void draw();
    void setActive(int listID);
    
    void start(string name);
    void startNext();
    void startPrev();
    void pause();
    void resume();
    void stop();
    void done(int & i);
    
    string appName;
    
    mpeClientTCP *client;
    handleChapters *reader;

    miniHandler miniApp;
    
    // our custom videoPlayer, loads all movies and then decides which
    // to draw and play
    customPlayer player;
    
    int activeID;
    bool bPlayAll;
    
    struct TotProg {
        string  name;
        string  type;
        int     chapterID;
        bool    active = false;
    };
    vector<TotProg> list;
    
    ofEvent<int>        buildGUIEvent;
    
private:
};

#endif
