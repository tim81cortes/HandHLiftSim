//
//  timer.cpp
//  
//
//  Created by Bingo Yu on 15/11/19.
//
//

#include "timer.h"
#include "splitter.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <iostream>


using namespace std;

Timer::Timer(){
    hour = 0;
    min = 0;
    sec = 0;
}

Timer::Timer(int h, int m, int s){
    hour = h;
    min = m;
    sec = s;
}

Timer::~Timer(){
    
}

/**
 * Change a time from string format into seconds
 * @param timeString is the time in a string format
 * @return time in seconds
 */
int Timer::string2sec(string timeString){
    int hour, min, sec;
    int seconds;
    Splitter* split = new Splitter();
    vector<string> sub_timeString = split->split(timeString, ":");
    hour = stoi(sub_timeString.at(0));
    min = stoi(sub_timeString.at(1));
    sec = stoi(sub_timeString.at(2));
    
    seconds = hour*60*60+min*60+sec;
    return seconds;
}

/**
 * Change a time from seconds into string format
 * @param sec is the time in a sec format
 * @return time in string format
 */
string Timer::sec2string(int sec){
    
    if(sec<86400){
        char timeString[10];
        char tmpString[3];
        int hour, min, secs;
        
        hour = sec/60/60;
        min = sec/60 - hour*60;
        secs = sec - hour*60*60 - min*60;
        
        
        if(hour<10){
            strcpy(timeString,"0");
            strcat(timeString,to_string(hour).c_str());
        }else{
            strcpy(timeString,to_string(hour).c_str());
        }
        
        strcat(timeString,":");
        
        if(min<10){
            strcat(timeString,"0");
            strcat(timeString,to_string(min).c_str());
        }else{
            strcat(timeString,to_string(min).c_str());
        }
        
        strcat(timeString,":");
        
        
        if(secs<10){
            strcat(timeString,"0");
            strcat(timeString,to_string(secs).c_str());
        }else{
            strcat(timeString,to_string(secs).c_str());
        }
//        cout<<timeString<<endl;
        return string(timeString);
    }else{
        return sec2string(sec-86400);
    }
}

/**
 * Calculate the time into seconds
 * @param timeT is the time to calculate
 * @return time in seconds
 */
//int Timer::timeInSeconds(time_t timeT){
//    string timeString;
//    timeString = time2string(timeT);
//    int hour, min, sec;
//    int seconds;
//    Splitter* split = new Splitter();
//    vector<string> sub_timeString = split->split(timeString, ":");
//    hour = stoi(sub_timeString.at(0));
//    min = stoi(sub_timeString.at(1));
//    sec = stoi(sub_timeString.at(2));
//    
//    seconds = hour*60*60+min*60+sec;
//    return seconds;
//}

/**
 * Compare two time
 * @param timeBefore the first time to compare
 * @param timeBefore the second time to compare
 * @return true if timeAfter is after timeBefore
 */
bool Timer::compareTime(int timeBefore, int timeAfter){

    if(timeBefore < timeAfter){
        return true;
    }else{
        return false;
    }
    
}
