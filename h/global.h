#ifndef GLOBAL_H_
#define GLOBAL_H_

#include <iostream.h>
#include "pcb.h";
//#include "ivtEntry.h";

//typedef unsigned char IVTNo;

extern volatile unsigned int lockFlag;

#define lockCout asm {\
			pushf;\
			cli;\
	}

#define unlockCout asm popf

//entry to a critical section
#define lock lockFlag = 0;

//ending of a critical section
#define unlock lockFlag = 1;\
	if(dispatchRequested) {\
		dispatch();\
	}

extern volatile int dispatchRequested;

extern volatile unsigned int activeThreads;


extern PCB* idle;

extern void idleFun();


#endif /* GLOBAL_H_ */
