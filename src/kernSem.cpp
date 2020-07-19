#include <iostream.h>;
#include "kernSem.h";
#include "list.h";
#include "global.h";


KernelSem::KernelSem(int init) {
	val = init;
	#ifndef BCC_BLOCK_IGNORE
		lockCout
	#endif
	blocked = new ListPCB();
	#ifndef BCC_BLOCK_IGNORE
		unlockCout
	#endif
}

KernelSem::~KernelSem() {
	while(val < 0) signal();
	#ifndef BCC_BLOCK_IGNORE
		lockCout
	#endif
	delete blocked;
	#ifndef BCC_BLOCK_IGNORE
		unlockCout
	#endif
}

int KernelSem::wait(Time maxTimeToWait) {
	lock
	if(--val < 0) {
		runningPCB->state = BLOCKED;
		runningPCB->blockedOnSem = this;
		runningPCB->blockedTime = maxTimeToWait;
		blocked->add((PCB*)runningPCB);
		if(maxTimeToWait != 0) {
			runningPCB->timeLimitedBlock = 1;
			blockedOnTime->addInTimeOrder((PCB*)runningPCB);
		}
		unlock;
		dispatch();
	}
	unlock

	if(runningPCB->timerReleased == 1)  {
		runningPCB->timerReleased == 0;
		return 0;
	}
	return 1;
}

void KernelSem::signal() {
	lock
	if(val++ < 0) {
		PCB* temp = blocked->takeFromBeginning();
		if(temp->timeLimitedBlock == 1) {
			for(blockedOnTime->toBeginning(); blockedOnTime->current != 0; blockedOnTime->next()) {
				if(blockedOnTime->current->data->id == temp->id) {
					if(blockedOnTime->current != blockedOnTime->last)
						blockedOnTime->current->next->data->blockedTime += blockedOnTime->current->data->blockedTime;
					blockedOnTime->deleteCurrent();
					break;
				}
			}
		}
		temp->blockedOnSem = 0;
		temp->timeLimitedBlock = 0;
		temp->state = READY;
		PCB::put(temp);
	}
	unlock
}


