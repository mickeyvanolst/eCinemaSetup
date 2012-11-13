//
//  01_TestMini.h
//  eCinemaSetup
//
//  Created by Mick van Olst on 13-11-12.
//
//

#ifndef eCinemaSetup__1_TestMini_h
#define eCinemaSetup__1_TestMini_h

#include "mainMini.h"
#include "Ball.h"

class testMini {
public:
    testMini(mainMini *_mai);
    void setup();
    void update();
    void draw();
    void endOfMini();
    
    string afterChapter;
    int testX, testY;
    
    mainMini *main;
private:
    vector<Ball*> balls;
};

#endif
