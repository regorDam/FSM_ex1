
#include "stdafx.h"
#include "State.h"


/*
Constructor / Destructor
*/
State::State()
{
	mMaxEvents = 6;

	clean();
}

State::~State()
{
}

/*
Clean
*/
void State::clean()
{
	strcpy_s(mName, "");
	mNumTransitions = 0;
	mNumEvents = 0;

	// Clean transitions
	for (int i = 0; i < mMaxEvents; i++)
	{
		strcpy_s(mTransition[i].event, "");
		mTransition[i].cTo = NULL;
	}

	// Clean actions
	for (int i = 0; i < (4 + mMaxEvents); i++)
	{
		mSpecification[i].func = NULL;
		strcpy_s(mSpecification[i].name, "");
		strcpy_s(mSpecification[i].event, "");
		mSpecification[i].type = eAction;
	}
}

/*
Transitions
*/
void State::addTransition(char *event, State *cState)
{
	// Add a transition to the list
	if (mNumTransitions < mMaxEvents)
	{
		strcpy_s(mTransition[mNumTransitions].event, event);
		mTransition[mNumTransitions].cTo = cState;
		++mNumTransitions;
	}
}

bool State::incoming(char *event, char *args)
{
	strcpy_s(mEvent, event);
	strcpy_s(mArgs, args);

	/*
	Loop through all OnEvent functions and process them
	Otherwise continue with standard actions
	*/
	if (mNumEvents > 0)
	{
		for (int i = eOnEvent; i < (eOnEvent + mNumEvents); i++)
		{
			if (strcmp(mSpecification[i].event, event) == 0)
			{
				// Run the OnEvent action
				mFunc = (mFuncPtr)mSpecification[i].func;
				mFunc(args);

				return true;
			}
		}
	}
	else
	{
		// Perform OnEntry and OnDo
		for (int i = 0; i < 2; i++)
		{
			if (mSpecification[i].func != NULL)
			{
				mFunc = (mFuncPtr)mSpecification[i].func;
				mFunc(args);
			}
		}
		return true;
	}

	return false;
}

State* State::outgoing(char *event)
{
	// Find the State to which this event is tied to
	for (int i = 0; i < mNumTransitions; i++)
	{
		if (strcmp(mTransition[i].event, event) == 0)
		{
			// Run the exit action
			if (mSpecification[eOnExit].func != NULL)
			{
				mFunc = (mFuncPtr)mSpecification[eOnExit].func;
				mFunc(mArgs);
			}

			return mTransition[i].cTo;
		}
	}

	return NULL;
}

/*
addAction
*/
void State::addAction(int when, int type, char *name, void *funcPtr)
{
	if ((when == eOnEntry) || (when == eDo) || (when == eOnExit))
	{
		if (type == eAction)
		{
			strcpy_s(mSpecification[when].name, name);
			mSpecification[when].type = type;
			mSpecification[when].func = funcPtr;
		}
	}
}

void State::addAction(int when, int type, char *name, char *event, void *funcPtr)
{
	if ((when == eOnEvent))
	{
		if ((type == eAction) && (mNumEvents < mMaxEvents))
		{
			strcpy_s(mSpecification[when + mNumEvents].name, name);
			strcpy_s(mSpecification[when + mNumEvents].event, event);
			mSpecification[when + mNumEvents].type = type;
			mSpecification[when + mNumEvents].func = funcPtr;

			++mNumEvents;
		}
	}
}

/*
Operator Methods
*/
void State::setName(char *name)
{
	strcpy_s(mName, name);
}

/*
Accessor Methods
*/
char *State::getName()
{
	return mName;
}

int State::getNumEvents()
{
	return mNumEvents;
}

int State::getNumTransitions()
{
	return mNumTransitions;
}