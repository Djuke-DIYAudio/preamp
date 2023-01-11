// Event handler code obtained from
// http://www.codeproject.com/Articles/34675/C-Programming-for-Embedded-System

#include "EventHandler.h"
#include <stddef.h>

#define MAX_EVENTS	64
#define MAX_TASKS	64

static tTaskRecord mpTaskList[MAX_TASKS];
static unsigned char nrTasks = 0;
static unsigned char EventList[MAX_EVENTS];
static unsigned char lastEvent = 0;
static unsigned char firstEvent = 0;


void RegisterEvent(unsigned char EventID, tEventHandler EventHandlerFunc)
{
	tTaskRecord *pNewTask;

	/* Create a new task record */
	pNewTask = &mpTaskList[nrTasks];

	/* Assign the event handler function to the task */
	pNewTask->EventID = EventID;
	pNewTask->EventHandler = EventHandlerFunc;
	nrTasks++;
}

void TriggerEvent(unsigned char EventID)
{
	EventList[lastEvent] = EventID;
	if (lastEvent < MAX_EVENTS-1) lastEvent++;
	else lastEvent = 0;
}

unsigned char GetFirstEvent()
{
	if (lastEvent != firstEvent) return EventList[firstEvent];
	else return 0;
}

void ClearFirstEvent()
{
	EventList[firstEvent] = 0;
	if (firstEvent == lastEvent) return;
	if (firstEvent < MAX_EVENTS-1) firstEvent++;
	else firstEvent = 0;
	
}

void EventManager(void)
{
    unsigned char EventID, i;
    tTaskRecord *pActiveTask;

    /* No return */
    while(1)
    {
        /* Read application event */
        EventID = GetFirstEvent();

        /* Process any event handlers */
        if (EventID) {
            for(i=0;i<nrTasks;i++) {
                pActiveTask = &mpTaskList[i];
                if(pActiveTask->EventHandler != NULL && pActiveTask->EventID == EventID)
                {
                    /* Call the task's event handler function */
                    (pActiveTask->EventHandler)();

                }
                    }
            /* All handlers checked, clear the event */
            ClearFirstEvent();
        }
    }
}


