//
//  plaats_origine.h
//  eCinemaSetup
//
//  Created by Mick van Olst on 01-12-12.
//
//

#ifndef eCinemaSetup_plaats_origine_h
#define eCinemaSetup_plaats_origine_h

#include "mainMini.h"
#include "plaats_origine.h"

class plaats_origine {
public:
    plaats_origine(mainMini *_mai);
    void setup();
    void update();
    void draw();
    void endOfMini();
    
    string afterChapter;
    
    mainMini *main;
private:
    int initTime;
    int durTime;
};

#endif

