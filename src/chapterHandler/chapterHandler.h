//
//  chapterHandler.h
//  eCinemaSetup
//
//  Created by Mick van Olst on 5-11-12.
//  Copyright (c) 2012 Mick van Olst. All rights reserved.
//
#pragma once

#include "ofxXmlSettings.h"

#ifndef chapterHandler
#define chapterHandler

class handleChapters {
    
public:
    handleChapters();
    
    void setup(string ID);
    void update();
    void readDir();
    void writeXML();
    void checkFiles();
    
    string          clientID;
    
    // read dir stuff
    ofDirectory     dir;
    ofFile          file;
        
    // okay might seem weird, but we need two vector objects for storing
    // all this info before we write it to an XML, just wanna be safe
    struct FilmInfo {
        string      file;
        string      name;
        int         duration;
        int         numFrames;
        int         filesize;
        int         width;
        int         height;
        bool        sameSettings;
    };
    vector<FilmInfo>    filmInfo;
    
    struct Chapters {
        string      name;
        FilmInfo    left;
        FilmInfo    middle;
        FilmInfo    right;
        bool        complete;
        bool        inOrder;
    };
    vector<Chapters>    chapters;
    
    // need to load movies to get some actual data on them
    ofVideoPlayer*  tempMov;
    
    // xml stuff
    ofxXmlSettings XML;
    string xmlStructure;
    
    string totalXmlString;
    
    int lastTagNumber;
    int lastChapNumber;
    
    ofEvent<int>        buildGUIEvent;
    ofEvent<int>        addAllVideosEvent;
    
    
private:

};

#endif
