#include "event.h";
#include "global.h";
#include "kernelEv.h";




Event::Event(IVTNo ivtNo) {
	#ifndef BCC_BLOCK_IGNORE
		lockCout
	#endif
		if(IVTEntry::allEntries[ivtNo] != 0) myImpl = new KernelEv(ivtNo);  //u necijem projektu pisalo == ali nema logike ??
		else myImpl = 0;
	#ifndef BCC_BLOCK_IGNORE
		unlockCout
	#endif
}

Event::~Event() {
	#ifndef BCC_BLOCK_IGNORE
		lockCout
	#endif
	delete myImpl;
	#ifndef BCC_BLOCK_IGNORE
		unlockCout
	#endif
}

void Event::wait() {
	#ifndef BCC_BLOCK_IGNORE
		lockCout
	#endif
	myImpl->wait();
	#ifndef BCC_BLOCK_IGNORE
		unlockCout
	#endif
}

void Event::signal() {
	myImpl->signal();
}
