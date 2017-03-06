//
//  main.cpp
//  
//
//  Created by Bingo Yu on 15/11/19.
//
//

#include "pass_event.h"

#include <stdio.h>
#include <fstream>
#include <string>
#include "lift.h"
#include <time.h>
#include <iostream>
#include <fstream>
#include <vector>



struct liftContents{
	PassEvent* riders[10];
};
struct Q{
	PassEvent* Head;
};
struct Floor{
	Q upQ;
	Q downQ;
};
struct building{
	 vector <Floor> floors;
};

using namespace std;

PassEvent* addPassengerToCorrectQ(PassEvent* crrntPssngr, PassEvent* qHead){
	crrntPssngr->printPassJourney();
	crrntPssngr->makeNextQdPassNULL();
	if(!qHead)
	{
		//// "There is no Q to Join." << endl;
		qHead = crrntPssngr;
	}
	else
	{
		//// "There is a Q to join" << endl;
		crrntPssngr->joinQueueAtTail(qHead, crrntPssngr);
	}
	return qHead;
}

int main(int argc, const char * argv[]) {
    // Initialise some variables
	int floorCount = 6; // 5 numbered loors and a ground floor
    PassEvent* passEvent = new PassEvent();
    ifstream* inFile = new ifstream();
    PassEvent* qToJoin;
    Lift* lift = new Lift();
    setOfLiftCalls* allCalls = new setOfLiftCalls;
    // Check if a file has been specified
    if (argc!=2)
    {
    	return 2;
    }
    inFile->open(argv[1]);
    // Check if the file wil open
    if(inFile->fail())
    {
    	//// "couldn't open file" << endl;
    	return 1;
    }
    //Floor* floors = new Floor[floorCount]; //was working before converted to vector
    building* bldng = new building;
    vector <Floor> flrs;
    // Initialise floor vector to required number of floors
    for (int i = 0; i < floorCount; i++)
    {
    	Floor tmpFl;
    	tmpFl.upQ.Head = NULL;
    	tmpFl.downQ.Head = NULL;
    	bldng->floors.push_back(tmpFl);

    }
    liftContents lift1;
    // Initialise contents of lift to NULL
    for(int i = 0; i < 10; i++)
    {
    	lift1.riders[i]= NULL;
    }
    // Necessary to set the start time of the simulation
    int eventGroupTime[2] = {32400,32400};
    int timeDifference;
    liftEvent* liftAtFloor;
    PassEvent* cursor = new PassEvent();
    // Clear .csv file
    cursor->clearOutFile();
    int strtFl = 0;
    // This loop runs until the end of events in file
    while(!inFile->eof()){
    	PassEvent* currentPass = new PassEvent();
        // Read in one event from file
    	currentPass->readInEvent(inFile);
        // Remember last passengers event time
    	eventGroupTime[0] = eventGroupTime[1];
        // Get next passengers event time
    	eventGroupTime[1] = currentPass -> getEventTime();
    	//eventTimesWBT << eventGroupTime[0] << " " << eventGroupTime[1] <<endl;
    	if (eventGroupTime[1] == eventGroupTime[0])
    	{
			
            //currentPass->printPassJourney();
            
            
			strtFl = currentPass->getStartFloor();

			//QingPsngrsWBT "Wants up: " << currentPass->wantsToGoUp() << endl;
			//QingPsngrsWBT "We know the start floor is ... " << strtFl << endl;
			if(currentPass->wantsToGoUp())
			{
				bldng->floors[strtFl].upQ.Head = addPassengerToCorrectQ(currentPass, bldng->floors[strtFl].upQ.Head);
				//QingPsngrsWBT "The up floor on " << strtFl<< " has been added to" << endl;
			}
			else
			{
				bldng->floors[strtFl].downQ.Head = addPassengerToCorrectQ(currentPass, bldng->floors[strtFl].downQ.Head);
				//QingPsngrsWBT "The down floor on " << strtFl<< " has been added to" << endl;
			}
    	}
    	else{
    		// check floors to see if there is a queue waiting.
    		for (int i = 0; i < floorCount; i++)
    		{
    			//Calls2QsWBT "check floors to see if there is a queue waiting." << endl;
				//currentPass->printPassJourney();
    	    	if(bldng->floors[i].upQ.Head != NULL)
    	    	{
    	    		allCalls->onFloorUpCalls[i] = 1;
    	    		//Calls2QsWBT "There is an up Q on floor " << i << endl;
    	    	}
    	    	else
    	    	{
    	    		allCalls->onFloorUpCalls[i] = 0;
    	    	}
    	    	if(bldng->floors[i].downQ.Head != NULL)
    	    	{
    	    		allCalls->onFloorDownCalls[i] = 1;
    	    		//Calls2QsWBT "There is a down Q on floor " << i << endl;
    	    	}
    	    	else
    	    	{
    	    		allCalls->onFloorDownCalls[i] = 0;
    	    	}
    		}
    		// Calculate time difference between passengers
    		timeDifference =  (eventGroupTime[1] - eventGroupTime[0]) * 1000;
    	    lift->handleQcalls(allCalls);
    	    bool carryOn = true;
            // Loop while there is still time remaining until next passenger
    	    while (carryOn){
    	    	PassEvent* cursor = new PassEvent();
                // Lift should service the job queues until a lift even occurs or the time runs out
    	    	liftAtFloor = lift->serviceCallQs(timeDifference);
    	    	timeDifference = liftAtFloor->timeRemaining;
    	    	cout << liftAtFloor->timeRemaining << " is the time remaining." << endl;
    	    	if (liftAtFloor->liftEvent)
    	    	{
    	    		cout <<"There is a lift event at floor: " << liftAtFloor->currentFloor << "!!!!!" << endl;
    	    	}
                // Does the following only if there is a lift event between the bottom floor and the top.
    	    	if (liftAtFloor -> liftEvent && liftAtFloor-> currentFloor >= 0 && liftAtFloor -> currentFloor < floorCount)
    	    	{
    	    		for (int i = 0; i < 10; i++){ //add constant of lift capacity.
    	    			// "Finding lift leavers." << endl;
    	    			//iterate over current lift passengers to find leavers
    	    			if(lift1.riders[i] != NULL) // if there is a rider
    	    			{
    	    				if(lift1.riders[i]->getEndFloor() == liftAtFloor -> currentFloor) //and they wand to leave
							{

    	    					//DeboardWBTest "Passenger "<< i <<"is leaving at" << liftAtFloor ->currentFloor << "Desired floor is " <<lift1.riders[i]->getEndFloor() << endl;
    	    					allCalls->inLiftCalls[lift1.riders[i]->getEndFloor()] = 0;
    	    					lift1.riders[i]->addPassEndTime(eventGroupTime[1], liftAtFloor->timeRemaining);
								lift1.riders[i]->printPassJourney();  //This is the main thing that needs to be output.
								lift1.riders[i] = NULL;
									//TODO delete rider using destructor.
							}
    	    				else{
    	    					//DeboardWBTest  "Passenger "<< i <<" not Leaving" << "Desired floor is" <<lift1.riders[i]->getEndFloor() << endl;
    	    				}
    	    			}
    	    			else
    	    			{
	    					//DeboardWBTest  "No passenger "<< i <<" in lift." << endl;
    	    			}
    	    		}
    	    		for(int i = 0; i < 10; i++) // TODO Add constant of number of lift capacity
    	    		{
    	    			//Finding lift spaces and boarding passengers
    	    			PassEvent* cursor = new PassEvent();
    	    			//SpaceBoardWBTest "Finding lift spaces and boarding passengers"<< endl;
    	    			int endFloorTest;
    	    				if (liftAtFloor -> goingUp)
    	    				{
    	    					if(lift1.riders[i] == NULL)
    	    					{
    	    						//SpaceBoardWBTest "Found a space at lift space: "<< i << endl;
									if(bldng->floors[liftAtFloor->currentFloor].upQ.Head != NULL){
										//SpaceBoardWBTest " There is an up Q on currentFloor which is"<< liftAtFloor->currentFloor << endl;
										//Freeing the user from the queue to the cursor
										bldng->floors[liftAtFloor->currentFloor].upQ.Head = cursor->getPassFromQ(bldng->floors[liftAtFloor->currentFloor].upQ.Head);
										cursor->makeNextQdPassNULL();
										// add the freed usr to the lift.
										lift1.riders[i] = cursor;

										allCalls->inLiftCalls[lift1.riders[i]->getEndFloor()] = 1;
										endFloorTest = lift1.riders[i]->getEndFloor();
										//SpaceBoardWBTest "Handling call" << endFloorTest << endl;
										lift1.riders[i]->printPassJourney();
										//SpaceBoardWBTest "This passenger is boarding and want's to go to " << endFloorTest << endl;
										lift->handleQcalls(allCalls);
									}
									else{
										// " There is no Q on currentFloor which is"<< liftAtFloor->currentFloor << endl;
									}
    	    					}
    	    				}
    	    				else
    	    				{
    	    					if(lift1.riders[i] == NULL)
    	    					{
    	    						//SpaceBoardWBTest "Found a space at lift space: "<< i << endl;
									//TODO Update with same features as when going up.
									if (bldng->floors[liftAtFloor->currentFloor].downQ.Head != NULL)
									{
										//SpaceBoardWBTest " There is a DOWN Q on currentFloor which is"<< liftAtFloor->currentFloor << endl;
										//SpaceBoardWBTest "Cursor's end floor " << cursor->getEndFloor() << endl;
										bldng->floors[liftAtFloor->currentFloor].downQ.Head = cursor->getPassFromQ(bldng->floors[liftAtFloor->currentFloor].downQ.Head);
										//SpaceBoardWBTest "Cursor's end floor now " << cursor->getEndFloor() << endl;
										cursor->makeNextQdPassNULL();
										lift1.riders[i] = cursor;
										endFloorTest = lift1.riders[i] ->getEndFloor();
										allCalls->inLiftCalls[lift1.riders[i]->getEndFloor()] = 1;
										//SpaceBoardWBTest "Handling call" << endFloorTest << endl;
										lift1.riders[i]->printPassJourney();

										lift->handleQcalls(allCalls);
									}
									}
    	    				}
    	    		}

    	    				//gUpWBT cout << "Down Q:" << endl;
    	    				//gUpWBT lift->printDownFloorQ();
    	    				//gUpWBT cout << endl;
    	    				//gUpWBT cout << "Up Q:" << endl;
    	    				//gUpWBT lift->printUpFloorQ();
                    // If there is a lift event with no passengers, run the time down until there are passengers
    	    				int timeTemp = timeDifference;
    	    				int timeDelay = 3000; //replace with constant
    	    				while (timeTemp > 0 && timeDifference-timeTemp < timeDelay){
    	    					timeTemp = timeTemp -1;
    	    				}
    	    				timeDifference = timeTemp;

    	    		}

    	    		if(timeDifference == 0)
    	    		{
    	    			// Finally Q user who was first in new time slot
    	    			currentPass->printPassJourney();
						strtFl = currentPass->getStartFloor();
						if(currentPass->wantsToGoUp())
						{
							//The up q on a floor  has been added to with pass from new time block
							bldng->floors[strtFl].upQ.Head = addPassengerToCorrectQ(currentPass, bldng->floors[strtFl].upQ.Head);
						}
						else
						{
							//The down q on a floor  has been added to with pass from new time block
							bldng->floors[strtFl].downQ.Head = addPassengerToCorrectQ(currentPass, bldng->floors[strtFl].downQ.Head);
						}
    	    			carryOn = false;
    	    		}
    	    	}


    	    	}
    	    }


    return 0;
}
