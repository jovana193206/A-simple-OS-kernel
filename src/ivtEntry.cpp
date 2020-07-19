#include "ivtEntry.h";
#include <dos.h>;
#include "kernelEv.h";

IVTEntry* IVTEntry::allEntries[256] = {0};

IVTEntry::IVTEntry(IVTNo ivtNo, pInterrupt interruptRoutine) {
	allEntries[ivtNo] = this;
	this->ivtNo = ivtNo;

	#ifndef BCC_BLOCK_IGNORE
	oldRoutine = getvect(ivtNo);
	setvect(ivtNo, interruptRoutine);
	#endif
	myKernEv = 0;
}

IVTEntry::~IVTEntry() {
	myKernEv = 0;
	#ifndef BCC_BLOCK_IGNORE
	setvect(ivtNo, oldRoutine);
	#endif
}

void IVTEntry::callOldRout() {
	(*oldRoutine)();
}

void IVTEntry::signal() {
	if(myKernEv != 0)
		myKernEv->signal();
}


