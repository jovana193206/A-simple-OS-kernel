#ifndef PCB_H_
#define PCB_H_

#include "thread.h";
#include "kernSem.h";

class ListPCB;

const StackSize maxStackSize = 65535;

typedef void (*pFunction)();
typedef enum states {NEW, READY, BLOCKED, FINISHED} States;

class PCB {

public:
	unsigned* stack;
	unsigned sp;
	unsigned ss;
	unsigned bp;
	int timeSlice;  //CPU holding time that was assigned to the thread when it was created
	States state;

	static int lastID;
	ID id;
	Thread* myThread;
	ListPCB* waitingToComplete;

	KernelSem* blockedOnSem;
	Time blockedTime;
	unsigned int timeLimitedBlock; // A flag which tells if this thread was blocked for a limited time interval
	unsigned int timerReleased;    // A flag which tells if this thread was unblocked by Timer because time ran out

	PCB(StackSize stackSize, Time timeSlice, Thread* thread);
	PCB(StackSize stackSize, Time timeSlice, pFunction function = &wrapper);
	PCB();
	~PCB();

	void start();
	static void put(PCB*);
	static PCB* get();

protected:
	static void wrapper();

};


extern volatile PCB* runningPCB;
//PCB that holds the context of the main function, which should be restored in the end
extern PCB* mainPCB;

#endif /* PCB_H_ */
