//
//  passEvent.h
//  
//
//  Created by Bingo Yu on 15/11/19.
//
//

#ifndef passEvent_hpp
#define passEvent_hpp

#include <stdio.h>
#include <string>
#include <vector>
#include <cmath>

using namespace std;

class PassEvent{
private:
    int eventTime; // the start time of the event (call the lift)
    string passengerID; // ID to trackt the passenger
    int currFloor; // current floor the passenger in
    int outFloor; // the floor passenger go out of the lift
    int outTime; // time of the passenger get out of the lift
    int waitTime;
    PassEvent* nextQueuedUser;
public:
    PassEvent();
    PassEvent(int eventTime, string passengerID, int currFloor, int outFloor);
    ~PassEvent();
    void setOutTime(int timeOut); // mutator to write the outTime
    PassEvent* readInEvent(ifstream* inputFile);
    void printPass(PassEvent* passEvent); // print the passenger
    void joinQueueAtTail(PassEvent* head, PassEvent* pass);
    int getEventTime();
    int getStartFloor();
    int getEndFloor();
    PassEvent* releaseOnePassenger(PassEvent* qHead);
    PassEvent* getPassFromQ(PassEvent* qHead);
    void makeNextQdPassNULL();
    bool wantsToGoUp();
    void startQueue(PassEvent* qHead, PassEvent* pass);
    void printPassJourney();
    void clearOutFile();
    void outPassFile();
    void printQ();
    void addPassEndTime(int nextPassEventTime, int liftTimeRemaining);
};
#endif /* passEvent_hpp */
