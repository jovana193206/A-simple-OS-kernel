#include <iostream.h>;
#include "pcb.h";
#include "list.h";
#include "SCHEDULE.H";
#include <dos.h>;

extern volatile PCB* runningPCB = 0;
//PCB that holds the context of the main function, which should be restored in the end
extern PCB* mainPCB = 0;
extern PCB* idle = 0;

int PCB::lastID = 0;

void PCB::put(PCB* pcb) {
	if((pcb != mainPCB) && (pcb != idle) && (pcb->state != BLOCKED) && (pcb->state != FINISHED)) {
		#ifndef BCC_BLOCK_IGNORE
			lockCout
		#endif
		Scheduler::put(pcb);
		#ifndef BCC_BLOCK_IGNORE
			unlockCout
		#endif
	}
}

PCB* PCB::get() {
	#ifndef BCC_BLOCK_IGNORE
		lockCout
	#endif
	PCB* temp = Scheduler::get();
	#ifndef BCC_BLOCK_IGNORE
		unlockCout
	#endif
	if(temp == 0) {
		if(activeThreads == 0)  {
			temp = mainPCB;
		}
		else temp = idle;  //default PCB which does nothing
	}
    return temp;
}

void PCB::start() {
	if(state == NEW) {
		lock
		Scheduler::put(this);
		unlock
		state = READY;
		activeThreads++;
	}
}

void PCB::wrapper() {
	runningPCB->myThread->run();
	runningPCB->state = FINISHED;
	activeThreads--;
	runningPCB->waitingToComplete->toBeginning();
    while(runningPCB->waitingToComplete->current != 0) {
    	PCB* temp = runningPCB->waitingToComplete->takeFromBeginning();
    	temp->state = READY;
    	PCB::put(temp);
    }
	dispatch();
}

PCB::PCB(StackSize stackSize, Time timeSlice, Thread* thread) {

	if(stackSize < 200) stackSize = 200;
	if(stackSize > maxStackSize) stackSize = maxStackSize;
	stackSize /= sizeof(unsigned);

	#ifndef BCC_BLOCK_IGNORE
		lockCout
	#endif
	stack = new unsigned[stackSize];
	#ifndef BCC_BLOCK_IGNORE
		unlockCout
	#endif

	stack[stackSize-1] = 0x200; // PSW register with I bit set
	#ifndef BCC_BLOCK_IGNORE
		stack[stackSize-2] = FP_SEG(wrapper);
		stack[stackSize-3] = FP_OFF(wrapper);
		sp = FP_OFF(stack + (stackSize - 12));
		ss = FP_SEG(stack + (stackSize - 12));
		bp = FP_OFF(stack + (stackSize - 12));
	#endif

	this->timeSlice = timeSlice;
	state = NEW;
	id = ++lastID;
	myThread = thread;
	#ifndef BCC_BLOCK_IGNORE
		lockCout
	#endif
	waitingToComplete = new ListPCB();
	#ifndef BCC_BLOCK_IGNORE
		unlockCout
	#endif

	blockedOnSem = 0;
	blockedTime = 0;
	timeLimitedBlock = 0;
	timerReleased = 0;
}

PCB::PCB(StackSize stackSize, Time timeSlice, pFunction function) {  //For idle PCB

	if(stackSize < 200) stackSize = 200;
	if(stackSize > maxStackSize) stackSize = maxStackSize;
	stackSize /= sizeof(unsigned);

	#ifndef BCC_BLOCK_IGNORE
		lockCout
	#endif
	stack = new unsigned[stackSize];
	#ifndef BCC_BLOCK_IGNORE
		unlockCout
	#endif

	stack[stackSize-1] = 0x200; // PSW register with I bit set
	#ifndef BCC_BLOCK_IGNORE
		stack[stackSize-2] = FP_SEG(function);
		stack[stackSize-3] = FP_OFF(function);
		sp = FP_OFF(stack + (stackSize - 12));
		ss = FP_SEG(stack + (stackSize - 12));
		bp = FP_OFF(stack + (stackSize - 12));
	#endif

	this->timeSlice = timeSlice;
	state = NEW;
	id = -1;
	myThread = 0;
	waitingToComplete = 0;

	blockedOnSem = 0;
	blockedTime = 0;
	timeLimitedBlock = 0;
	timerReleased = 0;
}

PCB::PCB() {    //For mainPCB
	#ifndef BCC_BLOCK_IGNORE
		lockCout
	#endif
	stack = new unsigned[defaultStackSize];
	#ifndef BCC_BLOCK_IGNORE
		unlockCout
	#endif
	sp = 0;
	ss = 0;
	bp = 0;
	timeSlice = 0;
	state = NEW;
	id = 0;
	myThread = 0;
	waitingToComplete = 0;

	blockedOnSem = 0;
	blockedTime = 0;
	timeLimitedBlock = 0;
	timerReleased = 0;
}

PCB::~PCB() {
	//deleting the element of allThreads list which contains this PCB
	if((this != mainPCB) && (this != idle)) {
		lock
		for(allThreads->toBeginning(); allThreads->current != 0; allThreads->next()) {
			if(allThreads->current->data->id == id) {
				allThreads->deleteCurrent();
				break;
			}
		}
		unlock
	}

 	#ifndef BCC_BLOCK_IGNORE
		lockCout
	#endif
	delete stack;
	#ifndef BCC_BLOCK_IGNORE
		unlockCout
	#endif
	if(waitingToComplete != 0) {
		#ifndef BCC_BLOCK_IGNORE
			lockCout
		#endif
		delete waitingToComplete;
		#ifndef BCC_BLOCK_IGNORE
			unlockCout
		#endif
	}
	blockedOnSem = 0;
}



