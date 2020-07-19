#include <iostream.h>;
#include "semaphor.h";
#include "kernSem.h";
#include "global.h";

int Semaphore::val() const { return myImpl->val; }

Semaphore::Semaphore(int init) {
	#ifndef BCC_BLOCK_IGNORE
		lockCout
	#endif
	myImpl = new KernelSem(init);
	#ifndef BCC_BLOCK_IGNORE
		unlockCout
	#endif
}
Semaphore::~Semaphore() {
	#ifndef BCC_BLOCK_IGNORE
		lockCout
	#endif
	delete myImpl;
	#ifndef BCC_BLOCK_IGNORE
		unlockCout
	#endif
}

int Semaphore::wait(Time maxTimeToWait) { return myImpl->wait(maxTimeToWait); }
void Semaphore::signal() { myImpl->signal(); }


