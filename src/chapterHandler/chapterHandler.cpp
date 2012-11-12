//
//  chapterHandler.cpp
//  eCinemaSetup
//
//  Created by Mick van Olst on 5-11-12.
//  Copyright (c) 2012 Mick van Olst. All rights reserved.
//
#include "chapterHandler.h"

//--------------------------------------------------------------
handleChapters::handleChapters(){
    // start of with the <movies> tag in our XML
    lastTagNumber	 = XML.addTag("movies");
	xmlStructure	 = "<movies>\n";
    lastTagNumber    = 0;
    lastChapNumber   = 0;
}

//--------------------------------------------------------------
void handleChapters::setup(string ID){
    clientID = ID;
    totalXmlString = "";
}

//--------------------------------------------------------------
void handleChapters::update(){
    
}

//--------------------------------------------------------------
void handleChapters::readDir(){
    
    printf("reading directory sir\n");
    
    // load content
    dir.listDir("content/");
	dir.sort(); // in linux the file system doesn't return file lists ordered in alphabetical order
    
	//allocate the vector to have as many strings as files
	if( dir.size() ){
		chapters.assign(dir.size(), Chapters());
	}
    
	// you can now iterate through the files and load them into the Chapters vector
	for(int i = 0; i < (int)dir.size(); i++){
        
        file.open(dir.getPath(i));
        if(file.isDirectory()){
            dir.sort(); // do this again to alfabetize
            
            chapters[i].complete = false; // don't worry kids, we're just not certain yet
            chapters[i].inOrder  = false; // let me get back to that
            
            chapters[i].name = dir.getName(i);
            printf("%s\n",chapters[i].name.c_str());
            // getting filename should be a folder
            
            
            // write a new chapter to the XMl object
            if( XML.pushTag("movies", lastTagNumber) ){
                
                dir.listDir(dir.getPath(i)+"/");
                dir.sort();
                
                int lastChapNumber = XML.addTag("chapter");
                // writing chapter folder name to XML object
                XML.setValue("chapter:name", chapters[i].name, lastChapNumber);
                
                //printf("tagNum: %i\n",lastChapNumber);
                //printf("lastTagNumber: %i\n",lastTagNumber);
                
                // iterate through directory
                for(int f = 0; f < (int)dir.size(); f++){
                    file.open(dir.getPath(f));
                    
                    if(file.isFile()){
                        file.open(dir.getPath(f));
                        
                        // check if movie is named L, M or R then add to filmInfo object thats inside Chapters
                        // furthermore we check for several aspects of the film files, some info is only gained
                        // when actually loading the video, so thats where the tempMov kicks in. This is a bit
                        // processor heavy though, so do't do it all the time!
                        string baseName = file.getBaseName();
                        string firstChar = "";
                        firstChar += baseName[0];
                        
                        if(firstChar == "L"){ // left screen duh
                            chapters[i].left.file = dir.getPath(f);
                            chapters[i].left.name = dir.getName(0);
                            chapters[i].left.filesize = file.getSize();
                            
                            tempMov = new ofVideoPlayer();
                            tempMov -> loadMovie(chapters[i].left.file);
                            chapters[i].left.width    = tempMov -> getWidth();
                            chapters[i].left.height   = tempMov -> getHeight();
                            chapters[i].left.duration = tempMov -> getDuration();
                            chapters[i].left.numFrames = tempMov -> getTotalNumFrames();
                            delete tempMov;
                            
                            chapters[i].left.sameSettings = false; // figure this out later
                            
                            if( XML.pushTag("chapter", lastChapNumber) ){
                                int tagNum = XML.addTag("left");
                                // writing film info of this thing to XML
                                XML.setValue("left:file", chapters[i].left.file, tagNum);
                                XML.setValue("left:name", chapters[i].left.name, tagNum);
                                XML.setValue("left:duration", chapters[i].left.duration, tagNum);
                                XML.setValue("left:frames", chapters[i].left.numFrames, tagNum);
                                XML.setValue("left:filesize", chapters[i].left.filesize, tagNum);
                                XML.setValue("left:width", chapters[i].left.width, tagNum);
                                XML.setValue("left:height", chapters[i].left.height, tagNum);
                                XML.popTag();
                            }
                        } else if(firstChar == "M"){ // middle screen duh
                            chapters[i].middle.file = dir.getPath(f);
                            chapters[i].middle.name = dir.getName(0);
                            chapters[i].middle.filesize = file.getSize();
                            
                            tempMov = new ofVideoPlayer();
                            tempMov -> loadMovie(chapters[i].left.file);
                            chapters[i].middle.width    = tempMov -> getWidth();
                            chapters[i].middle.height   = tempMov -> getHeight();
                            chapters[i].middle.duration = tempMov -> getDuration();
                            chapters[i].middle.numFrames = tempMov -> getTotalNumFrames();
                            delete tempMov;
                            
                            chapters[i].middle.sameSettings = false; // figure this out later
                            
                            if( XML.pushTag("chapter", lastChapNumber) ){
                                int tagNum = XML.addTag("middle");
                                // writing film info of this thing to XML
                                XML.setValue("middle:file", chapters[i].middle.file, tagNum);
                                XML.setValue("middle:name", chapters[i].middle.name, tagNum);
                                XML.setValue("middle:duration", chapters[i].middle.duration, tagNum);
                                XML.setValue("middle:frames", chapters[i].left.numFrames, tagNum);
                                XML.setValue("middle:filesize", chapters[i].middle.filesize, tagNum);
                                XML.setValue("middle:width", chapters[i].middle.width, tagNum);
                                XML.setValue("middle:height", chapters[i].middle.height, tagNum);
                                XML.popTag();
                            }
                        } else if(firstChar == "R"){ // right screen duh
                            chapters[i].right.file = dir.getPath(f);
                            chapters[i].right.name = dir.getName(0);
                            chapters[i].right.filesize = file.getSize();
                            
                            tempMov = new ofVideoPlayer();
                            tempMov -> loadMovie(chapters[i].left.file);
                            chapters[i].right.width    = tempMov -> getWidth();
                            chapters[i].right.height   = tempMov -> getHeight();
                            chapters[i].right.duration = tempMov -> getDuration();
                            chapters[i].right.numFrames = tempMov -> getTotalNumFrames();
                            delete tempMov;
                            
                            chapters[i].right.sameSettings = false; // figure this out later
                            
                            if( XML.pushTag("chapter", lastChapNumber) ){
                                int tagNum = XML.addTag("right");
                                // writing film info of this thing to XML
                                XML.setValue("right:file", chapters[i].right.file, tagNum);
                                XML.setValue("right:name", chapters[i].right.name, tagNum);
                                XML.setValue("right:duration", chapters[i].right.duration, tagNum);
                                XML.setValue("right:frames", chapters[i].left.numFrames, tagNum);
                                XML.setValue("right:filesize", chapters[i].right.filesize, tagNum);
                                XML.setValue("right:width", chapters[i].right.width, tagNum);
                                XML.setValue("right:height", chapters[i].right.height, tagNum);
                                XML.popTag();
                            }
                        }
                    }
                }
                dir.listDir("content/");
                dir.sort();
                // pop out of chapter
                XML.popTag();
            }
            
        } else {
            printf("no directory senior! - %s\n", dir.getPath(i).c_str());
        }
    }
    checkFiles(); // checking to see if all files are in order
    writeXML(); // write it to an XML file, makes it easy to check whats wrong
    
    XML.copyXmlToString(totalXmlString);
}

//--------------------------------------------------------------
void handleChapters::writeXML(){
    XML.saveFile("dir_files.xml");
    printf("writing XML monsieur\n");
}

//--------------------------------------------------------------
void handleChapters::checkFiles(){
    // checking the files
    
    // iterating through all chapters and check if they all contain the L, M and R movie files
    
    for (int i = 0; i < chapters.size(); i++) {
        
        // check separate to see which one is missing
        if(chapters[i].left.name != "" && chapters[i].middle.name != "" && chapters[i].right.name != "")
        {
            //printf("chapter contains all movies!\n");
            chapters[i].complete = true;
            
            // checking if all movies are the same amount of frames
            for (int f = 0; f < 3; f++) {
                if (chapters[i].left.numFrames && chapters[i].middle.numFrames && chapters[i].right.numFrames) {
                    //printf("SAME AMOUNT OF FRAMES!\n");
                    chapters[i].inOrder = true;
                } else {
                    //printf("NO SAME FRAMES..\n");
                    chapters[i].inOrder = false;
                }
            }
            
        } else {
            
            chapters[i].complete = false;
            
            // check separate to see which one is missing
            if(chapters[i].left.name == "")
            {
                printf("chapter: %s, no lefty..\n", chapters[i].name.c_str());
            } else {
                //printf("left!\n");
            }
            if(chapters[i].middle.name == "")
            {
                printf("chapter: %s, no middle..\n", chapters[i].name.c_str());
            } else {
                //printf("middle!\n");
            }
            if(chapters[i].right.name == "")
            {
                printf("chapter: %s, no righty..\n", chapters[i].name.c_str());
            } else {
                //printf("right!\n");
            }
        }
        
    }
    int myInt;
    ofNotifyEvent(buildGUIEvent,myInt,this);
    ofNotifyEvent(addAllVideosEvent,myInt,this);
}


