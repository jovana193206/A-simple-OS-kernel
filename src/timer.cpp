#include <iostream.h>;
#include <dos.h>;
#include "pcb.h";
#include "global.h";
#include "SCHEDULE.H";
#include "list.h";

extern void tick();

extern volatile unsigned int lockFlag = 1;

//global variable which holds the time remaining for the running thread to hold the CPU
extern volatile int runningTime = 0;

//is the switch of context explicitly requested
extern volatile int dispatchRequested = 0;

//variables that temporarily hold the values of sp,ss and bp in timer routine
unsigned tsp, tss, tbp;

void interrupt timer(...) {

	if(!dispatchRequested) {
		#ifndef BCC_BLOCK_IGNORE
		asm int 60h;                      // old timer call
		#endif

		if(lockFlag) {
            if(blockedOnTime->first != 0) {
            	blockedOnTime->first->data->blockedTime--;
            	while((blockedOnTime->first->data->blockedTime == 0) && (blockedOnTime->first != 0)) {
            		PCB* temp = blockedOnTime->takeFromBeginning();
            		if(temp->blockedOnSem != 0) {
            			temp->blockedOnSem->val++;
            			for(temp->blockedOnSem->blocked->toBeginning(); temp->blockedOnSem->blocked->current != 0;
            					temp->blockedOnSem->blocked->next()) {
            			            	if(temp->blockedOnSem->blocked->current->data->id == temp->id) {
            			            		temp->blockedOnSem->blocked->deleteCurrent();
            			            		break;
            			            	}
            			}
            		}
            		temp->blockedOnSem = 0;
            		temp->timerReleased = 1;
            		temp->timeLimitedBlock = 0;
            		temp->state = READY;
            		PCB::put(temp);
            	}
            }
         }
	}


	if((!dispatchRequested) && (runningPCB->timeSlice != 0))
		runningTime--;

	//context switch block
	if(dispatchRequested || ((runningTime == 0) && (runningPCB->timeSlice != 0))) {
		if(lockFlag) {
			dispatchRequested = 0;
			#ifndef BCC_BLOCK_IGNORE
				asm {
					mov tsp, sp
					mov tss, ss
					mov tbp, bp
				}
			#endif
			runningPCB->sp = tsp;
			runningPCB->ss = tss;
			runningPCB->bp = tbp;

			PCB::put((PCB*)runningPCB);
			runningPCB = PCB::get();

			tsp = runningPCB->sp;
			tss = runningPCB->ss;
			tbp = runningPCB->bp;
			runningTime = runningPCB->timeSlice;
			#ifndef BCC_BLOCK_IGNORE
				asm {
					mov sp, tsp
					mov ss, tss
					mov bp, tbp
				}
			#endif
		} //end of if(lockFlag) block
		else dispatchRequested = 1;
	} //end of context switch block
	tick(); //call of a function that is defined in the public test.


}



//INITIALIZATION OF IVT ENTRIES AND CONSTRUCTION OF GLOBAL VARIABLES

typedef void interrupt (*pInterrupt)(...);

//old timer interrupt routine address
pInterrupt oldRoutine;

extern void idleFun() {
	while(activeThreads >= 0) {}
}

extern void init() {

	#ifndef BCC_BLOCK_IGNORE
		asm cli;
		oldRoutine = getvect(0x8);
		setvect(0x8, &timer);
		setvect(0x60, oldRoutine);
		asm sti;
		lockCout
	#endif

	allThreads = new ListPCB();

	#ifndef BCC_BLOCK_IGNORE
		unlockCout
	#endif
	#ifndef BCC_BLOCK_IGNORE
		lockCout
	#endif

	mainPCB = new PCB();   //creates a PCB for the context of main function

	#ifndef BCC_BLOCK_IGNORE
		unlockCout
	#endif
	#ifndef BCC_BLOCK_IGNORE
		lockCout
	#endif

	idle = new PCB((StackSize)100,(Time)1,&idleFun);

	#ifndef BCC_BLOCK_IGNORE
		unlockCout
	#endif
	#ifndef BCC_BLOCK_IGNORE
		lockCout
	#endif

	blockedOnTime = new ListPCB();

	#ifndef BCC_BLOCK_IGNORE
		unlockCout
	#endif
	runningPCB = mainPCB;

}



//RESTAURATION OF HOST SYSTEM'S IVT ENTRIES AND DESTRUCTION OF GLOBAL VARIABLES

extern void restore() {

	#ifndef BCC_BLOCK_IGNORE
		lockCout
	#endif
		for(allThreads->toBeginning(); allThreads->current != 0; allThreads->next()) {
			#ifndef BCC_BLOCK_IGNORE
			lockCout
			#endif
				delete allThreads->current->data;
				allThreads->current->data = 0;
			#ifndef BCC_BLOCK_IGNORE
			unlockCout
			#endif
		}
	#ifndef BCC_BLOCK_IGNORE
		unlockCout
	#endif
	#ifndef BCC_BLOCK_IGNORE
		lockCout
	#endif
		delete allThreads;
	#ifndef BCC_BLOCK_IGNORE
		unlockCout
	#endif
	#ifndef BCC_BLOCK_IGNORE
		lockCout
	#endif
		delete mainPCB;
	#ifndef BCC_BLOCK_IGNORE
		unlockCout
	#endif
	#ifndef BCC_BLOCK_IGNORE
		lockCout
	#endif
		delete idle;
	#ifndef BCC_BLOCK_IGNORE
		unlockCout
	#endif
	#ifndef BCC_BLOCK_IGNORE
		lockCout
	#endif
		delete blockedOnTime;
	#ifndef BCC_BLOCK_IGNORE
		unlockCout
	#endif

	#ifndef BCC_BLOCK_IGNORE
		asm cli;
		setvect(0x8, oldRoutine);
		asm sti;
	#endif

}



