//
//  timer.hpp
//  
//
//  Created by Bingo Yu on 15/11/19.
//
//

#ifndef timer_hpp
#define timer_hpp

#include <stdio.h>
#include <string>
#include <time.h>

using namespace std;

class Timer{
private:
    int hour;
    int min;
    int sec;
public:
    Timer();
    Timer(int h, int m, int s);
    ~Timer();
    int string2sec(string timeString); // change time from string to seconds format
    string sec2string(int sec);//change time from seconds to string format
//    int timeInSeconds(time_t timeString);// Calculate the time into seconds
    bool compareTime(int timeBefore, int timeAfter);// Compare two time
    
};

#endif /* timer_hpp */
