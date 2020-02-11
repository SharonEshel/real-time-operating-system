/********************************************************************/
/* A Small Real Time System for the Real-Time laboratory                 */
/* built by: A.Teitelbaum on an idea of H.G.Mendelbaum                  */
/* Jerusalem College of Technology, 5759-64 (1999)                            */
/* SchedAlgo64.CPP, short-term scheduler algorithms                */
/****************************************************************/
//Sharon Eshel: 206795528 , Eden Attias:1196606

#include "smarts77.h"

int roundRobin( )
// Round Robin short-term algorithm 
{
	int count;
	int nextTask = (SMARTS.getCurrentTask()+1)%SMARTS.getTotalTasks( );
	for (count=0;
	     SMARTS.getStatus(nextTask)!=READY && count<SMARTS.getTotalTasks( );
	     count++)
	     nextTask=++nextTask%SMARTS.getTotalTasks( );
	if (count == SMARTS.getTotalTasks( ))	// no found READY task to run
		nextTask = SMARTS.getTotalTasks( );
	return nextTask;
}

int EDF()
{
	int minPriority = MAXINT;
	int nextTask= SMARTS.getTotalTasks();
for(int i=0; i<SMARTS.getTotalTasks();i++)
	if (SMARTS.getStatus(i) == READY && SMARTS.getCurrentPriority(i) < minPriority )
	{
		minPriority = SMARTS.getCurrentPriority(i);
		nextTask = i;
	}
return nextTask;
		
}


int RMS()
{
	int minPriority = MAXINT;
	int nextTask= SMARTS.getTotalTasks();
for(int i=0; i<SMARTS.getTotalTasks();i++)
	if (SMARTS.getStatus(i) == READY && SMARTS.getCurrentPriority(i) < minPriority )
	{
		minPriority = SMARTS.getCurrentPriority(i);
		nextTask = i;
	}
return nextTask;
		
}
