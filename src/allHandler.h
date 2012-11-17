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
    allHandler(mpeClientTCP *_cli, handleChapters *_rea, miniHandler *_min);
    //customPlayer *_cPl, 
    
    void setup();
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
    
    mpeClientTCP *client;
    handleChapters *reader;
    //customPlayer *player;
    miniHandler *miniApp;
    
    int activeID;
    
    struct TotProg {
        string  name;
        string  type;
        int     chapterID;
        bool    active = false;
    };
    vector<TotProg> list;
    
private:
};

#endif
