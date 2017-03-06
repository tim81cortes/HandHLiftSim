/*
 * lift.cpp
 *
 *  Created on: 6 Nov 2015
 *      Author: T_Aylott
 */

#include "lift.h"
#include <cmath>
#include <cstdlib>
#include <iostream>
#include "unistd.h"
#include <array>


using namespace std;

Lift::Lift() {
	//This constructor is the one that will be used initially to test the initial working of the system.
	//All values and settings will be set here.
	upCallQHead = NULL;
	downCallQHead = NULL;
	time = 0;
	currentFloorCall = 0;
	newBoarder=false;
	dataLogInterval = 100; //Logged this many milliseconds
	boardingTime = 1000; // Milliseconds
	distBetweenFloors = 10000; // Floor separation in Centimetres assumed to be constant for now
	slowDownDist = distBetweenFloors * 0.2;
	mostRecentFloor = 0; // Should be updated when it reaches a floor but not when it leaves
	distFromLowestFloor = mostRecentFloor*distBetweenFloors; // Where the floor is in cm from the ground floor lift position
	doorOpeningTime = 2000; // Milliseconds
	maxSpeed = 1; // meters per second
	safeShortTermVelocity = 0.05;
	acceleration = 0.00030; // millimetres per millisecond squared
	velocity = 0; // meters per second
	targetFloor = 1;
	goingUp = true;
	if (targetFloor < mostRecentFloor)
	{
		goingUp = false;
	}
	doorsOpen = false;
	moving = false;
	//lastAtFloor = -1;
	full = false; // might not be used in first iteration
}

Lift::Lift(int strtFloor, int brdngT, int flDst, int opnT, int spd) {
	// This constructor will be used to pass values later on.
	boardingTime = brdngT;
	distBetweenFloors = flDst;
	doorOpeningTime = opnT;
	maxSpeed = spd;
	doorsOpen = false;
	goingUp = true;
	moving = false;
	mostRecentFloor = 0;
	targetFloor = strtFloor;
	full = false;
}

Lift::~Lift() {

}
void Lift::addToQ(int upDwn, callQNode* call){

}
liftEvent* Lift::serviceCallQs(int timeTillNextPassEvent){
    // 
    
	liftEvent* gotToFloor = new liftEvent;
	TreeSortNode* currCall;
	int currentFloorCallUp = 0;
	int currentFloorCallDown = 0;
	int callTmp;
	gotToFloor->liftEvent = false;
	if (newBoarder)
	{
		// If there is a new boarder, make sure their call is picked up
		if (goingUp)
		{
			targetFloor = currCall->getNextValidCall(mostRecentFloor,1,upCallQHead);
		}
		else
		{
			targetFloor = currCall->getNextValidCall(mostRecentFloor,-1,downCallQHead);
		}
		newBoarder = false;
	}
	// Set the direction
	if (targetFloor > mostRecentFloor)
	{
		goingUp = true;
	}
	else
	{
		//currently biased towards going down as = floor means going down.
		// However this shouldn't matter.
		goingUp = false;

	}


	time = 0;
	//lftVerbose "Time till next passenger event " << timeTillNextPassEvent<< endl;
	while (gotToFloor->liftEvent == false && timeTillNextPassEvent - time > 0)
	{

		//lftVerbose "Looping while there is no liftEvent and while there is still time remaining."<< endl;
		if(goingUp)
		{
			// Going up	cout <<"going up" << endl;
			if(upCallQHead)
			{

				//lftVerbose "There is an up job queue" << endl;

				//currentFloorCall = currCall->getNextCall(mostRecentFloor, 1, upCallQHead);

				if(time % 500 == 0)
				{
					//gUpWBT "going up"<< endl;
					currentFloorCallUp = currCall->getNextValidCall(mostRecentFloor,1,upCallQHead);

					//lftVerbose "Current up call is " << currentFloorCallUp;
					targetFloor = currentFloorCallUp;
				}

				//lftVerbose "The next job found in the Q is " << currentFloorCall <<endl;
				if(distToTargetFloor() > 2 || gotToFloor->liftEvent == true){

					//lftVerbose "Lift call " << currentFloorCallUp << "Lift Height: " << distFromLowestFloor <<   endl;
					goToFloor(currentFloorCallUp);

				}
				else
				{
					//cout << "!!The most recent floor " << mostRecentFloor<< " The current call is " << currentFloorCallUp << "Lift Height: " << distFromLowestFloor <<   endl;
					//lftVerbose "Moving on to the next floor" << endl;
					gotToFloor->liftEvent = true;
					updateMostRecentFloor();

					currentFloorCallUp = currCall->getNextValidCall(mostRecentFloor,1,upCallQHead);
					//lftVerbose "This was returned from getNextCall " << currentFloorCallUp << endl;
					if (currentFloorCallUp > -1)
					{
						targetFloor = currentFloorCallUp;
					}

					gotToFloor->currentFloor = mostRecentFloor;
					//lftVerbose "!!!!! Now!!!! The most recent floor: " << mostRecentFloor << " Currentfloorcall: " << currentFloorCallUp << " TargetFloor: "<< targetFloor <<" Lift Height "<< distFromLowestFloor <<   endl;
				}

			}
			else if(upCallQHead == NULL || currentFloorCallUp < 0)
			{
				upCallQHead = NULL;  //deleting but not freeing allocated heap memory
				//lftVerbose << "Setting going up to false"  << endl;
				goingUp = false;


				// Delete up call Q

				//printDownFloorQ();
				targetFloor = 0;
			}
		}
		else
		{
			//going down			cout << "going down" << endl;
			if(downCallQHead)
			{
				//cout<< "There is an down job queue" << endl;

				if(time % 500 == 0)
				{
					//gUpWBT "going down"<< endl;
					currentFloorCallDown = currCall->getNextValidCall(mostRecentFloor, -1, downCallQHead);
					//lftVerbose "Current up call is " << currentFloorCallDown;
					targetFloor = currentFloorCallDown;
				}

				//cout << "Target floor before lift moves down is " <<targetFloor << endl;
				if(distToTargetFloor() < -2 || gotToFloor->liftEvent == true){
					//lftVerbose "Lift call " << currentFloorCallDown << "Lift Height: " << distFromLowestFloor <<   endl;
					goToFloor(currentFloorCallDown);
				}
				else
				{
					//lftVerbose "!!The most recent floor " << mostRecentFloor<< " The current call is " << currentFloorCallDown << "Lift Height: " << distFromLowestFloor <<   endl;
					//lftVerbose "Moving on to the next floor" << endl;
					gotToFloor->liftEvent = true;
					updateMostRecentFloor();
					currentFloorCallDown = currCall->getNextValidCall(mostRecentFloor, -1, downCallQHead);
					//lftVerbose "This was returned from getNextCall " << currentFloorCallDown << " Most recent floor is: "<< mostRecentFloor <<endl;
					if (currentFloorCallDown > 0)
					{
						targetFloor = currentFloorCallDown;
					}

					gotToFloor->currentFloor = mostRecentFloor;

					//lftVerbose "!!!!! Now!!!! The most recent floor: " << mostRecentFloor << " currentFloorCallDown: " << currentFloorCallDown << " TargetFloor: "<< targetFloor <<" Lift Height "<< distFromLowestFloor <<   endl;
				}
			}
			else if(downCallQHead == NULL || currentFloorCallDown < 0)
			{
					downCallQHead = NULL;
					gotToFloor->liftEvent = false;
					goingUp = true;
					//printUpFloorQ();
					targetFloor = 5;
			}
		}
	}
	//gotToFloor->currentFloor = mostRecentFloor; most recent floor not yet working as an accurate flag
	gotToFloor->timeRemaining = timeTillNextPassEvent - time;
	gotToFloor->goingUp = goingUp;

	return gotToFloor;
}

void Lift::handleQcalls(setOfLiftCalls* allCalls) {
	//Assuming a 6 floor building (0-5) the in lift array should contain 6 buttons
	//The onFloor array should contain 10 buttons up will be 0-4 and down will be 5-9
	//Calculate the down buttons floor by subtracting from 10 i.e. button 5 is down for fl 5
	// Button 6 is down for floor 4 7 for 3
	//Update: The above system has now been simplified to 3 different arrays 2 floor arrays and a lift one.
	//They now all have the same number of floors.
	for (int i = 0; i < 6; i++) {
		//HandleLiftCallsWBTest "This is the " << i << "th iteration over floors" << endl;
		if (allCalls->inLiftCalls[i] == 1) {
			//lftVerbose "A call is being handled in the lift for floor " << i << endl;
			if (goingUp) {//&& i > mostRecentFloor
				TreeSortNode* newCall = new TreeSortNode(i);
				//lftVerbose "NewCall lift up call is ";
				//newCall->printNode();
				//lftVerbose "Survived the printing function"<< endl;
				if (upCallQHead)
				{
					newCall->insertNode(upCallQHead, 1);
					//HandleLiftCallsWBTest "A call from in the lift should be added to the up queue." << endl;
				}
				else{
					upCallQHead = newCall->joinAtHead(upCallQHead);
					//HandleLiftCallsWBTest "A call from in the lift should be added to the up queue at the head." << endl;
				}
				newBoarder = true;
			}

			else if (goingUp == false) {
				TreeSortNode* newCall = new TreeSortNode(i);
				//HandleLiftCallsWBTest "NewCall lift down call is ";
				if (downCallQHead)
				{
					newCall->insertNode(downCallQHead, -1);
					//HandleLiftCallsWBTest "A call from in the lift will be added to the down queue." << endl;
				}
				else
				{
					downCallQHead = newCall->joinAtHead(downCallQHead);
					//HandleLiftCallsWBTest "A call from in the lift will be added to the down queue at the head." << endl;
				}
				newBoarder = true;

			}
		}
	}
	for (int i = 0; i < 6; i++) {  //
		if (allCalls->onFloorUpCalls[i] == 1)  //
				{
			//lftVerbose "An up call is being handled from floor " << i << endl;
			TreeSortNode* newCall = new TreeSortNode(i);
			//lftVerbose "NewCall floor up call is ";
			//newCall->printNode();
			//lftVerbose "Survived the printing function"<< endl;
			if (upCallQHead)
			{
				//HandleFloorCallsWBTest "A call from in a floor will be added to the up queue." << endl;
				newCall->insertNode(upCallQHead, 1);
			}
			else
			{
				//HandleFloorCallsWBTest "A call from in a floor will be added to the up queue at the head." << endl;
				upCallQHead = newCall->joinAtHead(upCallQHead);
			}
		}
		if (allCalls->onFloorDownCalls[i] == 1)  //
				{
			TreeSortNode* newCall = new TreeSortNode(i);
			//lftVerbose "NewCall floor down call is ";
			//newCall->printNode();
			if (downCallQHead)
			{
				//HandleFloorCallsWBTest "A call from in a floor should be added to the down queue." << endl;
				newCall->insertNode(downCallQHead, -1);
			}
			else
			{
				//HandleFloorCallsWBTest "A call from in a floor should be added to the down queue at the head." << endl;
				downCallQHead = newCall->joinAtHead(downCallQHead);
			}


		}
	}
}

void Lift::printUpFloorQ() {
	if (upCallQHead)
		{
		upCallQHead -> printTree();
		}


}
void Lift::printDownFloorQ() {
	if (downCallQHead)
	{
		downCallQHead->printTree();
	}
}

bool Lift::goToFloor(int trgFl) {

	targetFloor = trgFl;
	//lftVerbose "Start floor: " << mostRecentFloor << " Destination floor: " << trgFl << endl;
	closeDoors();

//	if (targetFloor > mostRecentFloor) {
//		//lftVerbose "Going up" << endl;
//		goingUp = true;
//	}
//	else if (targetFloor == mostRecentFloor) {
//		return true;
//	}
//	else {
//		//lftVerbose "Going down" << endl;
//		goingUp = false;
//	}

	if (abs(floor(distToTargetFloor())) > 0) {
		time++;
		updateVelocity(); // Speed should be calculated based on the direction and acceleration
		distFromLowestFloor += velocity;
		//updateMostRecentFloor();
		if (time % dataLogInterval == 0) {
			printDetails(0); // Pass 1 for headings 0 for just data
			//lftVerbose endl;
		}
		return false;
	}
	else{
	updateMostRecentFloor();
	velocity = 0;
	//could add another while to slow down
	openDoors();
	return true;
	}
}
// I guess this function is not needed.
void Lift::moveLift(double vlct) {
	distFromLowestFloor += velocity;
}

void Lift::updateVelocity() {
	if (abs(distToTargetFloor()) > slowDownDist && abs(velocity) < maxSpeed) {
		if (goingUp) {
			//lftVerbose "speeding up" << endl;
			velocity += acceleration;
		} else {
			//lftVerbose "speeding up" << endl;
			velocity -= acceleration;
		}
	} else if (abs(distToTargetFloor()) <= slowDownDist) {
		if (goingUp) {
			if (velocity > safeShortTermVelocity) {
				//lftVerbose "slowing down GU" << endl;
				velocity -= acceleration;
			} else {
				velocity = safeShortTermVelocity;
				//lftVerbose "Safe Short Term Velocity"<< endl;
			}
		} else {
			if (abs(velocity) > safeShortTermVelocity) {
				//lftVerbose "slowing down GU" << endl;
				velocity += acceleration;
			} else {
				velocity = 0 - safeShortTermVelocity;
				//lftVerbose "Safe Short Term Velocity"<< endl;
			}
		}
	} else {
		//lftVerbose "constant speed" << endl;
	}
}
void Lift::updateMostRecentFloor() {
	if (goingUp) {
		mostRecentFloor = floor((distFromLowestFloor +4) / distBetweenFloors);
	} else {
		mostRecentFloor = round((distFromLowestFloor -4) / distBetweenFloors);
	}
}

int Lift::distToTargetFloor() {
	return (targetFloor * distBetweenFloors) - distFromLowestFloor;
}
void Lift::printDetails(bool headings) {
	//lftVerbose "Most recent floor" << mostRecentFloor<< endl;
//

}
//void addToCallQ

void Lift::updateFloorInfo(int* mstRectFloor, int* lstAtFl, bool* atFl,
		int dstFrmGrndFl) {

}

void Lift::openDoors() {

}
void Lift::closeDoors() {

}

