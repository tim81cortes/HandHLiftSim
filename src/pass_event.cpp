//
//  passEvent.cpp
//  
//
//  Created by Bingo Yu on 15/11/19.
//
//

#include "pass_event.h"
#include "splitter.h"

#include <fstream>
#include <string>
#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <vector>
#include <time.h>
#include "timer.h"

using namespace std;

/*
 */
PassEvent::PassEvent(){
    eventTime = time(NULL);
    passengerID = "0000";
    currFloor = 0;
    outFloor = 0;
    waitTime = -1;
    outTime = -1;
    nextQueuedUser = NULL;
}


PassEvent::PassEvent(int eTime, string passID, int currFl, int outFl){
    eventTime = eTime;
    passengerID = passID;
    currFloor = currFl;
    outFloor = outFl;
    outTime = -1;
    waitTime = -1;
    nextQueuedUser = NULL;
}


PassEvent::~PassEvent(){
    
}

void PassEvent::setOutTime(int timeOut){
    outTime = timeOut;
}


PassEvent* PassEvent::readInEvent(ifstream* inputFile){
   PassEvent* singlePassEvent = new PassEvent();
    
    string line;
    string tmp_time;
    int eTime;
    string tmp_ID;
    int tmp_currFl;
    int tmp_outFl;
    
    //string file = fileName;
    //ifstream out;
   // out.open(file.c_str(), ios::in);
    
    if(!inputFile->eof()){
        
        Timer* timer = new Timer();
        Splitter* splitter;
        getline(*inputFile, line);
		vector<string> sub_str = splitter->split(line, ", ");
		tmp_time = sub_str.at(0);
		tmp_ID = sub_str.at(1);
		tmp_currFl = stoi(sub_str.at(2));
		tmp_outFl = stoi(sub_str.at(3));
		eTime = timer->string2sec(tmp_time);
		this->eventTime = eTime;
		this->passengerID = tmp_ID;
		this->currFloor = tmp_currFl;
		this->outFloor = tmp_outFl;
		this->nextQueuedUser = NULL;
		return this;

    }
    else
    {
    	inputFile->close();
    	return NULL;
    }
}

void PassEvent::printPass(PassEvent* passEvent){
    cout<<passEvent->passengerID<<endl;
}

void PassEvent::printPassJourney()
	{
        string timeStringI;
        Timer* timer = new Timer();
        timeStringI = timer->sec2string(eventTime);
        
        if(outTime>0){            
            string timeStringO = timer->sec2string(outTime);
            cout << "Arrived: " << timeStringI << " Exited: " << timeStringO << " Start Floor: " << currFloor << " End Floor: " << outFloor << endl;
            this->outPassFile();
        }
	}

void PassEvent::clearOutFile(){
    ofstream ofile;               // define the output file
    ofile.open("outPassEvent.csv", ios::binary);     // open the output file
    ofile << "Start Time" <<", " << "Start Floor" << ", " << "End Floor" << ", " << "End Time" << endl; //Add a header to the output file
    ofile.close();                // close the file
}

void PassEvent::outPassFile(){
    ofstream ofile;               // define the output file
    ofile.open("outPassEvent.csv", ios::app);     // open the output file
    Timer* timer = new Timer();
    ofile << timer->sec2string(eventTime) <<", " << currFloor << ", " << outFloor << ", " << timer->sec2string(outTime) << endl;
    ofile.close();                // close the file
}


void PassEvent::printQ(){
	printPassJourney();
	PassEvent* cursor = new PassEvent();
	cursor = this;
	while (cursor->nextQueuedUser)
	{
		cursor->printPassJourney();
		cursor = cursor->nextQueuedUser;
	}

		return;

}


int PassEvent::getEventTime(){
	return eventTime;
}

int PassEvent::getStartFloor(){
	return currFloor;
}
int PassEvent::getEndFloor(){
	return this->outFloor;
}
PassEvent* PassEvent::getPassFromQ(PassEvent* qHead){
	PassEvent* cursor = new PassEvent();
	if(qHead)
	{
		cursor = qHead;
	}
	if(cursor->nextQueuedUser != NULL)
	{
		qHead = cursor->nextQueuedUser;
	}
	else
	{
		qHead = NULL;
	}
	currFloor = cursor->currFloor;
	eventTime = cursor->eventTime;
	outFloor = cursor->outFloor;
	outTime = cursor->outTime;
	nextQueuedUser = NULL;
	return qHead;
}

PassEvent* PassEvent::releaseOnePassenger(PassEvent* qHead){
	PassEvent* cursor = new PassEvent();
	//PassReleaseWBTest "giving the cursor the q Head" << endl;
	if (qHead)
	{
		cursor = qHead;
		//PassReleaseWBTest "Cursor pass = " << cursor->nextQueuedUser;
		cursor->printPassJourney();
	}
	//PassReleaseWBTest "Checking if there's a next queued user" << endl;
	if(cursor->nextQueuedUser != NULL)
	{
		//PassReleaseWBTest "There is so giving qHead the second in line." << endl;
		qHead = cursor->nextQueuedUser;
	}
	else{
		//PassReleaseWBTest "There isn't so giving qHead NULL as there is noone left in the Q" << endl;
		qHead = NULL;
	}
	return qHead;
}
void PassEvent::makeNextQdPassNULL(){
	nextQueuedUser = NULL;
}

bool PassEvent::wantsToGoUp(){
	if(currFloor < outFloor)
	{
		return true;
	}
	else
	{
		return false;
	}
}
void PassEvent::addPassEndTime(int nextPassEventTime, int liftTimeRemaining){
	outTime = nextPassEventTime - round(liftTimeRemaining / 1000);
}


void PassEvent::joinQueueAtTail(PassEvent* head, PassEvent* pass)
	{
		//cout << " JoingQAtTail called" << endl;
		PassEvent* cursor;
		PassEvent* toBePlaced = pass;


		int traversals = 0;


		cursor = head;

		while (cursor->nextQueuedUser != NULL)
		{
			//cout << "This is the "<< traversals << "traversal" << endl;
			cursor = cursor->nextQueuedUser;
			traversals ++;
		}
		//cout << "Traversals finished" << endl;
		toBePlaced -> nextQueuedUser = NULL;
		cursor->nextQueuedUser = toBePlaced;
		return;






	}

void PassEvent::startQueue(PassEvent* qHead, PassEvent* pass)
{
	nextQueuedUser = NULL;
	qHead = this;
	return;
}





