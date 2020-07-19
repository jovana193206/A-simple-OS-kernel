#ifndef IVTENTRY_H_
#define IVTENTRY_H_

class KernelEv;

typedef unsigned char IVTNo;
typedef void interrupt (*pInterrupt)(...);

class IVTEntry {
public:
	static IVTEntry* allEntries[256];
	pInterrupt oldRoutine;
	KernelEv* myKernEv;
	IVTNo ivtNo;

	IVTEntry(IVTNo ivtNo, pInterrupt interruptRoutine);
	~IVTEntry();

	void callOldRout();
	void signal();
};



#endif /* IVTENTRY_H_ */
