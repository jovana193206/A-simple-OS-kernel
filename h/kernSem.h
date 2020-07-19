#ifndef KERNSEM_H_
#define KERNSEM_H_

#include "semaphor.h";

class ListPCB;

class KernelSem {
public:
	int val;
	ListPCB* blocked;

	KernelSem(int init = 1);
	~KernelSem();

	int wait(Time maxTimeToWait);
	void signal();



};


#endif /* KERNSEM_H_ */
