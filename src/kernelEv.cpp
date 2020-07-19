#include "kernelEv.h";
#include "ivtEntry.h";
#include "global.h";


KernelEv::KernelEv(IVTNo ivtNo) {
	creator = (PCB*)runningPCB;
	val = 1;
	this->ivtNo = ivtNo;
	lock
	if ((IVTEntry::allEntries[ivtNo]) != 0)
		IVTEntry::allEntries[ivtNo]->myKernEv = this;
	unlock
}

KernelEv::~KernelEv() {
	creator = 0;
    lock
	IVTEntry::allEntries[ivtNo]->myKernEv = 0;
	unlock
}

void KernelEv::wait() {
	if((PCB*)runningPCB == creator) {
		val = 0;
		runningPCB->state = BLOCKED;
		dispatch();
	}
}

void KernelEv::signal() {
	if(val == 0) {
		creator->state = READY;
		PCB::put(creator);
		val = 1;
	}
}


