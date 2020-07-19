#include <iostream.h>;
#include "list.h";
#include "pcb.h";
#include "thread.h";

extern volatile unsigned int activeThreads = 0;

extern int userMain(int, char*[]);
extern void init();
extern void restore();

class MainThread : public Thread {
public:
	MainThread(int argc, char* argv[]) {
		this->argc = argc;
		this->argv = argv;
		retVal = 0;
	}
	void run() {
		retVal = userMain(argc,argv);
	}
	int ret() { return retVal; }

private:
	int argc;
	char** argv;
	int retVal;
};


int main(int argc, char* argv[]) {

	init();

	#ifndef BCC_BLOCK_IGNORE
		lockCout
	#endif
	MainThread* userMainThread = new MainThread(argc, argv);
	#ifndef BCC_BLOCK_IGNORE
		unlockCout
	#endif
	userMainThread->start();
	userMainThread->waitToComplete();   // Giving the processor to userMainThread
	int ret = userMainThread->ret();
	#ifndef BCC_BLOCK_IGNORE
		lockCout
	#endif
	delete userMainThread;
	#ifndef BCC_BLOCK_IGNORE
		unlockCout
	#endif

	restore();
	return ret;
}





