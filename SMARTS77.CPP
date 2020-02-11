/****************************************************************/
/* A Small Real Time System for the Real-Time laboratory        */
/* built by: A.Teitelbaum on an idea of H.G.Mendelbaum          */
/* Jerusalem College of Technology, 5759-64 (1999)              */
/* update  Tishrey   5777                                       */
/* SMARTS77.CPP, SMARTS class body                              */
/****************************************************************/
//Sharon Eshel: 206795528 , Eden Attias:1196606

#include "smarts77.h"

/**********    Function     **********/
unsigned getTimerClocks()
// Gets the remaining clocks of the timer register
{
	unsigned clocks;
	/* latch counter #0 */
	outportb(0x43, 0x00);
	/* read counter #0 low byte */
	clocks = inportb(0x40);
	/* read counter #0 high byte */
	clocks += inportb(0x40) << 8;
	return clocks;
}
////////////////////////////////////////////////////
/**********    class body:  Parallelism     **********/
Parallelism::Parallelism()
{
	currentTask = 0;
	sleepTasks = 0;
	activeTasks = 0;
	totalTasks = 0;
	deadlock = false;
	error = false;
	contextSwitchFlag = true;
	endOfTimeSlice = true;
}

void Parallelism::externalFunctions(void interrupt(*timerInterruptHandler)(...),
	void far *scheduler, void far *userTaskEnd,
	int far(*algorithm)())
	// Sets the external functions
{
	this->timerInterruptHandler = timerInterruptHandler;
	this->scheduler = scheduler;
	this->userTaskEnd = userTaskEnd;
	this->algorithm = algorithm;
	contextSched.declare(scheduler, userTaskEnd, 'S', 0, 0); // prepare the stack of the scheduler task
	for (int i = MaxStack - 1; i >= (MaxStack - 14); i--)
		schedCopy[i] = contextSched.stack[i];
}

int Parallelism::declareTask(void far *code, char name, int priority, int cycle)
// Insert a new task entry in SMARTS context array [ ]
{
	if (totalTasks < MaxTask - 1)
	{
		context[totalTasks++].declare(code, userTaskEnd, name, priority, cycle-1);
		++activeTasks;
		return true;
	}
	else
		return false;
}

void Parallelism::runTheTasks()
// Start running all tasks declared in SMARTS.		
{
	int flag;
	context[totalTasks].status = READY;	//for task 'runTheTasks' (IDLE task)
	context[totalTasks].priority = MAXINT;
	context[totalTasks].currentPriority = MAXINT;

	currentTask = totalTasks;

	asm	cli;		// forbids interrupts (clear interrupts) while changing the interrupt vect
		// saves the original BIOS userInt in our variable 'userIntAddress' to be restored at the end
	userIntAddress = getvect(userInt);	// BIOS userInt 0x60  (unused by PC)
				// puts the normal BIOS timerInt into the unused userInt address
	setvect(userInt, getvect(timerInt));	// timerInt 0x08 

	// sets our SMARTS external function 'timerInterruptHandler' as the new PC hard interrupt time handler
	setvect(timerInt, timerInterruptHandler);
	asm	sti;	// allows back interrupts (set interrupts)

		// waits for end of runTheTasks (end of all the tasks)
	while (true)
	{
		flag = false;
		if (deadlock)
		{
			textcolor(RED);
			cprintf("\n\n\rExit : deadlock");
			break;
		}
		if (activeTasks == 0)
		{
			for (int i = 0;i <totalTasks;i++)
			{
				if (context[i].cycle > 0)//there are tasks we need to execute
					flag = true;
			}
			if (!flag)
			{
				cprintf("\n\n\rExit : finish");
				break;
			}
		}
		if (error)
		{
			textcolor(RED);
			cprintf("\n\n\rExit : error");
			break;
		}

	}

	// restore the original BIOS 'interrupt vector' at the end before returning to regular DOS
	asm	cli;	// no interrupts
	setvect(timerInt, getvect(userInt));	// restore original BIOS time handler
	setvect(userInt, userIntAddress);	// restore original BIOS userInt
	asm	sti;	// yes interrupts
}

void Parallelism::callScheduler()
// Return the control to the scheduler, this sets ON the software interrupt ProgInt flag
{

	setProgInt();
	asm int timerInt;
}

void Parallelism::restoreSchedStack()
// Restore the scheduler stack
{
	for (int i = MaxStack - 1; i >= (MaxStack - 14); i--)
		contextSched.stack[i] = schedCopy[i];
}

int Parallelism::getCurrentTask()
{
	return currentTask;
}

void Parallelism::setCurrentTask(int taskNum)
// Sets the next task to be run
{
	if (taskNum <= totalTasks)
		currentTask = taskNum;
}
void Parallelism::setInheritancePriority(int taskNum, int myPriority) {
	context[taskNum].currentPriority = myPriority;
}

int Parallelism::getTotalTasks()
// Gets total tasks declared
{
	return totalTasks;
}

int Parallelism::getDeadlock()
{
	return deadlock;
}

void Parallelism::setDeadlock()
{
	deadlock = true;
}

int Parallelism::contextSwitchOn()
// flag which enables context switch
{
	if (endOfTimeSlice) //is current time slice finished ?
	{
		endOfTimeSlice = false;
		context[getCurrentTask()].contextSwitchFlag = true;
		//contextSwitchFlag = true;
		callScheduler();	// return control to the scheduler
		return 1;
	}
	context[getCurrentTask()].contextSwitchFlag = true;
	//contextSwitchFlag = true;
	return 0;
}

void Parallelism::contextSwitchOff()
// Disable context switch
{
	context[getCurrentTask()].contextSwitchFlag = false;
	//contextSwitchFlag = false;
}

int Parallelism::getContextSwitch()
{
	return contextSwitchFlag;
}
int Parallelism::getTaskContextSwitch()
{
	return context[getCurrentTask()].getContextSwitch();
}

void Parallelism::setProgInt()
// flag indicates to the extern function 'timerInterruptHandler' 
// that this is an internal SMARTS software interrupt call, 
// and the original BIOS function has no to be called.
{
	progInt = true;
}

void Parallelism::resetProgInt()
{
	progInt = false;
}

int Parallelism::getProgInt()
{
	return progInt;
}

void Parallelism::setEndOfTimeSlice()
// flag indicates that when 'context switch' will be enabled, 
// it must also return the control to the scheduler.
{
	endOfTimeSlice = true;
}

char Parallelism::getName(int taskNum)	// returns name found or ' ' if not
{
	return (taskNum <= totalTasks) ? context[taskNum].name : ' ';
}

char Parallelism::getCurrentName()
{
	return context[currentTask].name;
}

taskStatus Parallelism::getStatus(int taskNum)
// returns status or undefined if not found
{
	return (taskNum <= totalTasks) ? context[taskNum].status : UNDEFINED;
}

taskStatus Parallelism::getCurrentStatus()
{
	return context[currentTask].status;
}

void Parallelism::resume(int taskNum)
{
	if (taskNum < totalTasks)
		context[taskNum].status = READY;
}

void Parallelism::resume(char taskName)
{
	for (int i = 0;i < totalTasks;++i)
		if (context[i].name == taskName)
			context[i].status = READY;
}


void Parallelism::setCurrentNotActive()
{
	context[currentTask].status = NOT_ACTIVE;
	--activeTasks;
}
void Parallelism::suspended()
{
	context[currentTask].status = SUSPENDED;
	callScheduler();
}

void Parallelism::incrPriority(int taskNum)
{
	if (taskNum < totalTasks)
		context[taskNum].incrPriority();
}
void Parallelism::setOriginalPriority(int taskNum)
{
	if (taskNum < totalTasks)
		context[taskNum].setOriginalPriority();
}

void Parallelism::setCurrentOriginalPriority()
{
	context[currentTask].setOriginalPriority();
}

Event *Parallelism::getExpectedEvent(int taskNum)
// returns *Event  or  NULL  if not found
{
	return (taskNum <= totalTasks) ? context[taskNum].expectedEvent : NULL;
}

Event *Parallelism::getCurrentExpectedEvent()
{
	return context[currentTask].expectedEvent;
}

void Parallelism::setCurrentExpectedEvent(Event *expectedEvent)
{
	context[currentTask].expectedEvent = expectedEvent;
}

void Parallelism::sleep(int t)
// Current task sleeps for 't' milliseconds
{
	if (t < MAXINT)
	{
		context[currentTask].sleepCount = t / 55 + 1;
		context[currentTask].status = SLEEP;
		++sleepTasks;
		callScheduler();		// return control to scheduler
	}
}

void Parallelism::sleepDecr(int taskNum)
{
	if (taskNum < totalTasks)
		context[taskNum].sleepDecr();
}

void Parallelism::getCurrentStack(unsigned &StackSeg, unsigned &StackPtr)
// Load current task stack pointer
{
	StackSeg = context[currentTask].stackSeg;
	StackPtr = context[currentTask].stackPtr;
}

void Parallelism::setCurrentStack(unsigned StackSeg, unsigned StackPtr)
// Save current task stack pointer
{
	context[currentTask].stackSeg = StackSeg;
	context[currentTask].stackPtr = StackPtr;
}

void Parallelism::getSchedStack(unsigned &StackSeg, unsigned &StackPtr)
// Load scheduler  stack pointer
{
	StackSeg = contextSched.stackSeg;
	StackPtr = contextSched.stackPtr;
}

void Parallelism::handleTimers()
// handling of the sleep status mode
{
	for (int i = totalTasks - 1; i >= 0; --i)
	{
		if (getStatus(i) == SLEEP)
		{
			sleepDecr(i);
			if (getStatus(i) == READY)
				--sleepTasks;

		}
	}
	for (int j= totalTasks - 1; j >= 0; --j)
	{
		context[j].currentPeriod--;//currentPeriod 
		if (context[j].currentPeriod <= 0)
			if (context[j].cycle > 0)
				if (context[j].status == NOT_ACTIVE)
					context[j].redeclare();
				else
					SMARTS.error = true;

	}

}

void Parallelism::taskEnd()
// This function is called after the last operation of a task, instead of function return
{
	SMARTS.setCurrentNotActive();
	SMARTS.callScheduler();	// return the control to the scheduler to run a next task
}

int  Parallelism::getCurrentPriority(int taskNum)
{
	if (taskNum < totalTasks)
		return context[taskNum].getCurrentPriority();
	return MAXINT;
}

int  Parallelism::getPriority(int taskNum)
{
	if (taskNum < totalTasks)
		return context[taskNum].getPriority();
	return MAXINT;
}



/**********    class body:  Task     **********/
Task::Task()
{
	stack[MaxStack - 14] = _BP;
	stack[MaxStack - 13] = _DI;
	stack[MaxStack - 12] = _SI;
	stack[MaxStack - 11] = _DS;
	stack[MaxStack - 10] = _ES;
	stack[MaxStack - 9] = _DX;
	stack[MaxStack - 8] = _CX;
	stack[MaxStack - 7] = _BX;
	stack[MaxStack - 6] = _AX;
	stackSeg = FP_SEG(&stack[MaxStack - 14]);
	stackPtr = FP_OFF(&stack[MaxStack - 14]);
	status = NOT_ACTIVE;
	sleepCount = 0;
	currentPriority = priority = 0;
	contextSwitchFlag = true;
}
//-----------------------------------------------------
void Task::declare(void far *code, void far *userTaskEnd, char name, int priority, int cycle)
{
	stack[MaxStack - 5] = FP_OFF(code);
	stack[MaxStack - 4] = FP_SEG(code);
	stack[MaxStack - 3] = _FLAGS;
	stack[MaxStack - 2] = FP_OFF(userTaskEnd);
	stack[MaxStack - 1] = FP_SEG(userTaskEnd);
	this->name = name;
	this->priority = priority;
	this->currentPriority = priority;
	this->cycle = cycle;
	this->currentPeriod = priority;
	this->period = priority;
	status = READY;
	for (int i = MaxStack - 1; i >= (MaxStack - 14); i--)
		oldStack[i] = stack[i];

}

void Task::redeclare()
{
	for (int i = MaxStack - 1; i >= (MaxStack - 14); i--)
		stack[i] = oldStack[i];
	stackSeg = FP_SEG(&stack[MaxStack - 14]);
	stackPtr = FP_OFF(&stack[MaxStack - 14]);
	status = READY;
	cycle--;
	//currentPriority = priority;
	currentPeriod = period;
	SMARTS.activeTasks++;

}

//----------------------------------------------------
void Task::incrPriority()
{
	--currentPriority;
}
//----------------------------------------------------
void Task::setOriginalPriority()
{
	currentPriority = priority;
}
//----------------------------------------------------
void Task::sleepDecr()
// Decrements the sleep counter and update the task status accordingly
{
	if (status == SLEEP)
	{
		if (sleepCount > 0)
			--sleepCount;
		if (!sleepCount)
			status = READY;
	}
}
//----------------------------------------------------
int Task::getCurrentPriority()
{
	return currentPriority;
}
//----------------------------------------------------

int Task::getPriority()
{
	return priority;
}


int  Task::getContextSwitch()
{
	return contextSwitchFlag;
}




/**********    class body:  PriorityQueue     **********/
PriorityQueue::PriorityQueue()
{
	for (int i = 0; i < 15; i++)// initialization of the queue
		waitingList[i] = MAXINT;
	counterTask = 0;

}


void PriorityQueue::insert(int task) {
	int index;
	if (counterTask == 0) {
		waitingList[0] = task;
		counterTask = 1;
	}
	else {
		for (int i = 0; i < counterTask;i++)
			if (SMARTS.getCurrentPriority(i) <= SMARTS.getCurrentPriority(task) && SMARTS.getCurrentPriority(i + 1) > SMARTS.getCurrentPriority(task))
			{
				for (int j = counterTask; j > i + 1; j--)
					waitingList[j] = waitingList[j - 1];
				index = i;
			}

		waitingList[index] = task;
		counterTask++;
	}

}


int PriorityQueue::remove() {
	counterTask--;
	int task = waitingList[counterTask];
	waitingList[counterTask]=MAXINT;
	return task;

}

int PriorityQueue::isEmpty() {
	return counterTask == 0;
}

/**********    class body:	Mutex     **********/

Mutex::Mutex() {
	s = 1;
	owner = -1;
	level = 0;
}


void Mutex::acquire(){
	SMARTS.contextSwitchOff();
	if (s == 1 || owner == SMARTS.getCurrentTask())
		s = 0;
	else {
		queue.insert(SMARTS.getCurrentTask());
		SMARTS.suspended();
	}
	owner = SMARTS.getCurrentTask();
	level++;
	SMARTS.contextSwitchOn();
}

void Mutex::release() {

	SMARTS.contextSwitchOff();
	if (owner == SMARTS.getCurrentTask())
	{
		if (--level)
			return;
		else {
			owner = -1;
			if (!queue.isEmpty()) {
				SMARTS.resume(queue.remove());
			}
			else
				s = 1;
		}
	}
	SMARTS.contextSwitchOn();

}


/**********    class body:	MutexInheritance     **********/

MutexInheritance::MutexInheritance() {
	s = 1;
	owner = -1;
	level = 0;
}


void MutexInheritance::acquire() {
	SMARTS.contextSwitchOff();
	if (s == 1 || owner == SMARTS.getCurrentTask())
		s = 0;
	else {
		if (SMARTS.getCurrentPriority(owner) > SMARTS.getCurrentPriority(SMARTS.getCurrentTask()))
			SMARTS.setInheritancePriority(owner, SMARTS.getCurrentPriority(SMARTS.getCurrentTask()));


		queue.insert(SMARTS.getCurrentTask());
		SMARTS.suspended();
	}
	owner = SMARTS.getCurrentTask();
	level++;
	SMARTS.contextSwitchOn();
}

void MutexInheritance::release() {

	SMARTS.contextSwitchOff();
	if (owner == SMARTS.getCurrentTask())
	{
		if (--level)
			return;
		else {
			SMARTS.setInheritancePriority(owner,SMARTS.getPriority(owner));
			owner = -1;
			if (!queue.isEmpty()) {
				SMARTS.resume(queue.remove());
			}
			else
				s = 1;
		}
	}
	SMARTS.contextSwitchOn();

}