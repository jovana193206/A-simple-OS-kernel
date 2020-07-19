#ifndef KERNELEV_H_
#define KERNELEV_H_

#include "pcb.h";
#include "event.h"

class KernelEv {
public:
	PCB* creator;
	int val;
	IVTNo ivtNo;

	KernelEv(IVTNo ivtNo);
	~KernelEv();

	void wait();
	void signal();
};


#endif /* KERNELEV_H_ */
