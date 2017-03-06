/*
 * lift.h
 *
 *  Created on: 6 Nov 2015
 *      Author: T_Aylott
 */

#ifndef CALLSFROMEVENTS_SRC_LIFT_H_
#define CALLSFROMEVENTS_SRC_LIFT_H_

#include "treeSrtNode.h"

struct setOfLiftCalls{
	bool inLiftCalls[6];
		bool onFloorUpCalls[6];
		bool onFloorDownCalls[6];
	};
struct liftEvent{
	int currentFloor;
	long timeRemaining;
	bool goingUp;
	bool liftEvent;
};

//struct LiftContents{
//	PassEvent* riders[10];
//};


class Lift{
	private:

		struct callQNode{
				int call;
				callQNode* next;
			};

		int time;
		int dataLogInterval;
		int targetFloor;
		int mostRecentFloor;
		int currentFloorCall;
		bool newBoarder;
		int boardingTime; // milliseconds
		int doorOpeningTime; // milliseconds
		double maxSpeed; // meters per milliseconds
		double safeShortTermVelocity;
		double velocity;
		double acceleration; // centimetres per millisecond squared [Could be scaled up once working]
		int distBetweenFloors; // Distance between floors meters
		int slowDownDist;
		double distFromLowestFloor;
		bool doorsOpen;
		bool goingUp;
		bool moving;
		bool full;
		TreeSortNode* upCallQHead;
		TreeSortNode* downCallQHead;
		//Could add a flag if the lift has calls or not.

		// Weight setting to be added

	public:
		Lift();
		Lift(int strtFloor, int brdngT, int flDst, int opnT, int spd);
		~Lift();
		bool goToFloor(int flr); // Setting of how to proceed to floor e.g. 1 go directly to that floor
		void openDoors();
		void closeDoors();
		void moveLift(double vlct);
		void updateFloorInfo(int* mstRectFloor, int* lstAtFl, bool* atFl, int dstFrmGrndFl); // could add weight to influence scheduling
		int isAtFloor(bool useSimSnsrs); //Setting to simulate external sensors
		void updateVelocity();
		int distToTargetFloor();
		void updateMostRecentFloor();
		void printDetails(bool headings);
		void handleQcalls(setOfLiftCalls* allCalls);
		void addToCallQ(callQNode* call);
		void printUpFloorQ();
		void printDownFloorQ();
		liftEvent* serviceCallQs(int timeTillNextPassEvent);
		void addToQ(int upDwn, callQNode* node);
};



#endif /* CALLSFROMEVENTS_SRC_LIFT_H_ */
