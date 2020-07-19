#include <iostream.h>;
#include "thread.h";
#include "pcb.h";
#include "list.h";
#include "global.h";

extern volatile int dispatchRequested;

void dispatch() {
#ifndef BCC_BLOCK_IGNORE
	lockCout
#endif
	dispatchRequested = 1;
#ifndef BCC_BLOCK_IGNORE
	asm {
		int 8h
	}
	unlockCout
#endif
}

ID Thread::getId() { return myPCB->id; }
ID Thread::getRunningId() { return runningPCB->id; }
Thread* Thread::getThreadById(ID id) {
	lock   //critical section because it depends of the value of current in allThreads
	for(allThreads->toBeginning(); allThreads->current != 0; allThreads->next())
		if(allThreads->current->data->id == id) {
			Thread* ret = allThreads->current->data->myThread;
			unlock
			return ret;
		}
	unlock
	return 0;
}

Thread::Thread(StackSize stackSize, Time timeSlice) {
	#ifndef BCC_BLOCK_IGNORE
		lockCout
	#endif
	myPCB = new PCB(stackSize, timeSlice, this);
	#ifndef BCC_BLOCK_IGNORE
		unlockCout
	#endif
	allThreads->add(myPCB);
}

Thread::~Thread() {
	this->waitToComplete();
	#ifndef BCC_BLOCK_IGNORE
		lockCout
	#endif
	delete myPCB;
	#ifndef BCC_BLOCK_IGNORE
		unlockCout
	#endif
}

void Thread::waitToComplete() {
	if(myPCB->state != FINISHED) {
		runningPCB->state = BLOCKED;
		myPCB->waitingToComplete->add((PCB*)runningPCB);
		dispatch();
	}
}

void Thread::start() {
	myPCB->start();
}



