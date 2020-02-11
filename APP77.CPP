/********************************************************************/
/* A Small Real Time System for the Real-Time laboratory            */
/* built by: A.Teitelbaum on an idea of H.G.Mendelbaum              */
/* Jerusalem College of Technology, 5759-64 (1999-03)               */
/* update  Tishrey   5777                                           */
/* APP77.CPP, an application to demonstrate SMARTS77  functioning   */
/********************************************************************/

//Sharon Eshel: 206795528 , Eden Attias:1196606
#include "smarts77.h"

// user Functions
Mutex outputMutex;

MutexInheritance m3, m4;
//Mutex m3, m4;
FILE* myOutput = fopen("test.txt", "w");
Event evAtoC, evAtoB, evDtoF, evDtoE;



void a( )
{
	long i;
	int j;
	outputMutex.acquire();
	//SMARTS.contextSwitchOff();
	cout << "\n *************   A Start    *********************";
	fprintf(myOutput, "\n *************   A Start    *********************");
	//SMARTS.contextSwitchOn();
	outputMutex.release();

	for ( j=0; j <5; j++)
	{
		for (i=0; i <200000; i++);
		outputMutex.acquire();
		//SMARTS.contextSwitchOff();
		cout << "A";
		fprintf(myOutput, "A");
		//SMARTS.contextSwitchOn();
		outputMutex.release();
	}

	outputMutex.acquire();
	cout << "\nA wants m3";
	fprintf(myOutput, "\nA wants m3");
	outputMutex.release();

	m3.acquire();

	for (j = 0; j <5; j++)
	{
		for (i = 0; i <200000; i++);
		outputMutex.acquire();
		//SMARTS.contextSwitchOff();
		cout << "A";
		fprintf(myOutput, "A");
		//SMARTS.contextSwitchOn();
		outputMutex.release();
	}
	outputMutex.acquire();
	cout << "\nA send event to C";
	fprintf(myOutput, "\nA send event to C");
	outputMutex.release();

	evAtoC.send('C', NULL, 0);

	outputMutex.acquire();
	cout << "\nA send event to B";
	fprintf(myOutput, "\nA send event to B");
	outputMutex.release();

	evAtoB.send('B', NULL, 0);
	
	for ( j = 0; j <5; j++)
	{
		for (i = 0; i <200000; i++);
		outputMutex.acquire();
		//SMARTS.contextSwitchOff();
		cout << "A";
		fprintf(myOutput, "A");
		//SMARTS.contextSwitchOn();
		outputMutex.release();
	}
	
	outputMutex.acquire();
	cout << "\nA does release to m3";
	fprintf(myOutput, "\nA does release to m3");
	outputMutex.release();

	m3.release();

	for (j = 0; j <5; j++)
	{
		for (i = 0; i <200000; i++);
		outputMutex.acquire();
		//SMARTS.contextSwitchOff();
		cout << "A";
		fprintf(myOutput, "A");
		//SMARTS.contextSwitchOn();
		outputMutex.release();
	}


	outputMutex.acquire();
	//SMARTS.contextSwitchOff();
	cout << "\n *************   A Finish   *********************";
	fprintf(myOutput, "\n *************   A Finish    *********************");

	//SMARTS.contextSwitchOn();
	outputMutex.release();
}

void b( )
{
	long i;
	int j;
	//SMARTS.contextSwitchOff();
	outputMutex.acquire();
	cout << "\n *************   B Start    *********************";
	fprintf(myOutput, "\n *************   B Start    *********************");
	//SMARTS.contextSwitchOn();
	outputMutex.release();

	for (j=0; j <5; j++)
	{
		for (i=0; i < 200000; i++);
		//SMARTS.contextSwitchOff();
		outputMutex.acquire();
		cout << "B";
		fprintf(myOutput, "B");
		//SMARTS.contextSwitchOn();
		outputMutex.release();
	}

	outputMutex.acquire();
	cout << "\nB wait to event from A";
	fprintf(myOutput, "\nB wait to event from A");
	outputMutex.release();

	char x;
	evAtoB.wait(x);

	for (j = 0; j <5; j++)
	{
		for (i = 0; i < 200000; i++);
		//SMARTS.contextSwitchOff();
		outputMutex.acquire();
		cout << "B";
		fprintf(myOutput, "B");
		//SMARTS.contextSwitchOn();
		outputMutex.release();
	}

	//SMARTS.contextSwitchOff();
	outputMutex.acquire();
	cout << "\n *************   B Finish   *********************";
	fprintf(myOutput, "\n *************   B Finish   *********************");
	//SMARTS.contextSwitchOn();
	outputMutex.release();
}

void c( )
{
	long i;
	int j;
	//SMARTS.contextSwitchOff();
	outputMutex.acquire();
	cout << "\n *************   C Start    *********************";
	fprintf(myOutput, "\n *************   C Start    *********************");
	//SMARTS.contextSwitchOn();
	outputMutex.release();
	for ( j=0; j <5; j++)
	{
		for ( i=0; i < 200000; i++);
		//SMARTS.contextSwitchOff();
		outputMutex.acquire();
		cout << "C";
		fprintf(myOutput, "C");
		//SMARTS.contextSwitchOn();
		outputMutex.release();
	}
	outputMutex.acquire();
	cout << "\nC wait to event from A";
	fprintf(myOutput, "\nC wait to event from A");
	outputMutex.release();

	char x;
	evAtoC.wait(x);
	
	for (j = 0; j <5; j++)
	{
		for (i = 0; i < 200000; i++);
		//SMARTS.contextSwitchOff();
		outputMutex.acquire();
		cout << "C";
		fprintf(myOutput, "C");
		//SMARTS.contextSwitchOn();
		outputMutex.release();
	}
	outputMutex.acquire();
	cout << "\nC wants m3";
	fprintf(myOutput, "\nC wants m3");
	outputMutex.release();

	m3.acquire();

	for (j = 0; j <5; j++)
	{
		for (i = 0; i < 200000; i++);
		//SMARTS.contextSwitchOff();
		outputMutex.acquire();
		cout << "C";
		fprintf(myOutput, "C");
		//SMARTS.contextSwitchOn();
		outputMutex.release();
	}

	outputMutex.acquire();
	cout << "\nC does release to m3";
	fprintf(myOutput, "\nC does release to m3");
	outputMutex.release();

	m3.release();

	//SMARTS.contextSwitchOff();
	outputMutex.acquire();
	cout << "\n *************   C Finish   *********************";
	fprintf(myOutput, "\n *************   C Finish   *********************");
	//SMARTS.contextSwitchOn();
	outputMutex.release();
}

void d()
{
	long i;
	int j;
	outputMutex.acquire();
	//SMARTS.contextSwitchOff();
	cout << "\n *************   D Start    *********************";
	fprintf(myOutput, "\n *************   D Start    *********************");
	//SMARTS.contextSwitchOn();
	outputMutex.release();

	for (j = 0; j <5; j++)
	{
		for (i = 0; i <200000; i++);
		outputMutex.acquire();
		//SMARTS.contextSwitchOff();
		cout << "D";
		fprintf(myOutput, "D");
		//SMARTS.contextSwitchOn();
		outputMutex.release();
	}

	outputMutex.acquire();
	cout << "\nD wants m4";
	fprintf(myOutput, "\nD wants m4");
	outputMutex.release();

	m4.acquire();

	for (j = 0; j <5; j++)
	{
		for (i = 0; i <200000; i++);
		outputMutex.acquire();
		//SMARTS.contextSwitchOff();
		cout << "D";
		fprintf(myOutput, "D");
		//SMARTS.contextSwitchOn();
		outputMutex.release();
	}
	outputMutex.acquire();
	cout << "\nD send event to F";
	fprintf(myOutput, "\nD send event to F");
	outputMutex.release();

	evDtoF.send('F', NULL, 0);

	outputMutex.acquire();
	cout << "\nD send event to E";
	fprintf(myOutput, "\nD send event to E");
	outputMutex.release();

	evDtoE.send('E', NULL, 0);

	for (j = 0; j <5; j++)
	{
		for (i = 0; i <200000; i++);
		outputMutex.acquire();
		//SMARTS.contextSwitchOff();
		cout << "D";
		fprintf(myOutput, "D");
		//SMARTS.contextSwitchOn();
		outputMutex.release();
	}

	outputMutex.acquire();
	cout << "\nD does release to m4";
	fprintf(myOutput, "\nD does release to m4");
	outputMutex.release();

	m4.release();

	for (j = 0; j <5; j++)
	{
		for (i = 0; i <200000; i++);
		outputMutex.acquire();
		//SMARTS.contextSwitchOff();
		cout << "D";
		fprintf(myOutput, "D");
		//SMARTS.contextSwitchOn();
		outputMutex.release();
	}


	outputMutex.acquire();
	//SMARTS.contextSwitchOff();
	cout << "\n *************   D Finish   *********************";
	fprintf(myOutput, "\n *************   D Finish    *********************");

	//SMARTS.contextSwitchOn();
	outputMutex.release();
}

void e()
{
	long i;
	int j;
	//SMARTS.contextSwitchOff();
	outputMutex.acquire();
	cout << "\n *************   E Start    *********************";
	fprintf(myOutput, "\n *************   E Start    *********************");
	//SMARTS.contextSwitchOn();
	outputMutex.release();

	for (j = 0; j <5; j++)
	{
		for (i = 0; i < 200000; i++);
		//SMARTS.contextSwitchOff();
		outputMutex.acquire();
		cout << "E";
		fprintf(myOutput, "E");
		//SMARTS.contextSwitchOn();
		outputMutex.release();
	}

	outputMutex.acquire();
	cout << "\nE wait to event from D";
	fprintf(myOutput, "\nE wait to event from D");
	outputMutex.release();

	char x;
	evDtoE.wait(x);

	for (j = 0; j <5; j++)
	{
		for (i = 0; i < 200000; i++);
		//SMARTS.contextSwitchOff();
		outputMutex.acquire();
		cout << "E";
		fprintf(myOutput, "E");
		//SMARTS.contextSwitchOn();
		outputMutex.release();
	}

	//SMARTS.contextSwitchOff();
	outputMutex.acquire();
	cout << "\n *************   E Finish   *********************";
	fprintf(myOutput, "\n *************   E Finish   *********************");
	//SMARTS.contextSwitchOn();
	outputMutex.release();
}

void f()
{
	long i;
	int j;
	//SMARTS.contextSwitchOff();
	outputMutex.acquire();
	cout << "\n *************   F Start    *********************";
	fprintf(myOutput, "\n *************  F Start    *********************");
	//SMARTS.contextSwitchOn();
	outputMutex.release();
	for (j = 0; j <5; j++)
	{
		for (i = 0; i < 200000; i++);
		//SMARTS.contextSwitchOff();
		outputMutex.acquire();
		cout << "F";
		fprintf(myOutput, "F");
		//SMARTS.contextSwitchOn();
		outputMutex.release();
	}
	outputMutex.acquire();
	cout << "\nF wait to event from D";
	fprintf(myOutput, "\nF wait to event from D");
	outputMutex.release();

	char x;
	evDtoF.wait(x);

	for (j = 0; j <5; j++)
	{
		for (i = 0; i < 200000; i++);
		//SMARTS.contextSwitchOff();
		outputMutex.acquire();
		cout << "F";
		fprintf(myOutput, "F");
		//SMARTS.contextSwitchOn();
		outputMutex.release();
	}
	outputMutex.acquire();
	cout << "\nF wants m4";
	fprintf(myOutput, "\nF wants m4");
	outputMutex.release();

	m4.acquire();

	for (j = 0; j <5; j++)
	{
		for (i = 0; i < 200000; i++);
		//SMARTS.contextSwitchOff();
		outputMutex.acquire();
		cout << "F";
		fprintf(myOutput, "F");
		//SMARTS.contextSwitchOn();
		outputMutex.release();
	}

	outputMutex.acquire();
	cout << "\nF does release to m4";
	fprintf(myOutput, "\nF does release to m4");
	outputMutex.release();

	m4.release();

	//SMARTS.contextSwitchOff();
	outputMutex.acquire();
	cout << "\n *************   F Finish   *********************";
	fprintf(myOutput, "\n *************   F Finish   *********************");
	//SMARTS.contextSwitchOn();
	outputMutex.release();
}

void main( )
{
	clrscr();

	SMARTS.externalFunctions(timerInterruptHandler, scheduler, myTaskEnd, RMS);
	
	SMARTS.declareTask(a,'A',402,3);
	SMARTS.declareTask(b,'B',401,3);
	SMARTS.declareTask(c,'C',400,3);
	SMARTS.declareTask(d, 'D', 402, 3);
	SMARTS.declareTask(e, 'E', 401, 3);
	SMARTS.declareTask(f, 'F', 400, 3);
	SMARTS.runTheTasks();

	/*This data works for EDF and roundRobin:
	SMARTS.declareTask(a, 'A', 50, 3);
	SMARTS.declareTask(b, 'B', 70, 4);
	SMARTS.declareTask(c, 'C', 100, 5);
	SMARTS.runTheTasks();
    */

	/*This data doesn't works for EDF and roundRobin:
	SMARTS.declareTask(a, 'A', 3, 3);
	SMARTS.declareTask(b, 'B', 3, 4);
	SMARTS.declareTask(c, 'C', 3, 5);
	SMARTS.runTheTasks();
	*/
	

	/*This data doesn't works for roundRobin but  works for EDF:
	SMARTS.declareTask(a, 'A', 6, 3);
	SMARTS.declareTask(b, 'B', 17, 4);
	SMARTS.declareTask(c, 'C', 12, 5);
	SMARTS.runTheTasks();
	*/
}



