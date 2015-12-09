

#include "stdafx.h"
#include <conio.h>
#include <iostream>
#include "FSM.h"
#include "State.h"

//Global Variables
int d; //Distance 
int life, op;
char selEvent[50],lastState[50];
unsigned int eventID;

FSM cFSM;
State cState[6];

/******************
STATE ACTIONS
******************/
// State: Activity: On Event: shortAttack
void sActivity_SA(char *args)
{
	std::cout << "Activity: Short Attacking" << std::endl;
}
// State: Activity: On Event: LongAttack
void sActivity_LA(char *args)
{
	std::cout << "Activity: Long Attacking" << std::endl;
}
// State: Activity: On Event: Escape
void sActivity_ES(char *args)
{
	std::cout << "Activity: Escaping" << std::endl;
}

void init()
{
	life = 100;
	// Idle State
	cState[0].setName("Idle");
	cState[0].addTransition("GoToActivity",&cState[1]);

	// Activity state
	cState[1].setName("Activity");
	cState[1].addAction(3, 0, "Action: ShortAttack", "ShortAttack", *sActivity_SA );
	cState[1].addAction(3, 0, "Action: LongAttack", "LongAttack", sActivity_LA);
	cState[1].addAction(3, 0, "Action: Escape", "Escape", sActivity_ES);
	cState[1].addTransition("ShortAttack", &cState[1]);
	cState[1].addTransition("LongAttack", &cState[1]);
	cState[1].addTransition("Escape", &cState[0]);

	// Add the states to the FSM controller
	cFSM.addStates(cState);
	cFSM.outEvent("GoToActivity", "");
}

bool checkLife(){
	if (life <= 25) return false;
	else return true;
}

void readInput()
{
	std::cin >> d;
	
	if (d > 5) d = 2;
	else d = 1;

	if (!checkLife()) d = 3;
	

}

void progres()
{
	switch (d)
	{
	case 1:
		strcpy_s(selEvent, "ShortAttack");
		break;
	case 2:

		strcpy_s(selEvent, "LongAttack");
		break;
	case 3:
		strcpy_s(selEvent, "Escape");
		break;
	default:
		strcpy_s(selEvent, "Idle");
		break;
	}
	if (cFSM.outEvent(selEvent, ""))
	{
		life -= 20;
		std::cout << "Event: "<< selEvent<< " LIFE: "<<life<<std::endl;
	}
	
}

void print(std::string line){
	std::cout << line.data() << std::endl;
}


int _tmain(int argc, _TCHAR* argv[])
{
	print("******* Finit State Machine *******");
	init();
	std::cout<<"Current State: "<<cFSM.getCurrentStateName() <<std::endl;
	print("Enter the distance");
	do {
		readInput();
		progres();
		std::cout << "Current State: " << cFSM.getCurrentStateName() << std::endl;

	} while (op != 5);

	_getch();
	return 0;
}


