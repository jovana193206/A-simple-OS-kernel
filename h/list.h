#ifndef LIST_H_
#define LIST_H_

#include<iostream.h>
#include "global.h";
#include "pcb.h";

extern volatile int dispatchRequested;
void dispatch();

class ListPCB {
	struct Elem {
				PCB *data;
				Elem *next;
				Elem(PCB* t, Elem *n = 0) { data = t; next = n; }
			};

	void deleteList();
public:
	Elem *first, *last, *current, *previous;

	ListPCB();
	~ListPCB();

	ListPCB& add(PCB* newElem);
	ListPCB& addInTimeOrder(PCB* newElem);
	PCB* takeFromBeginning();
	void deleteCurrent();
	void toBeginning();
	void next();
};

extern ListPCB* allThreads;
extern ListPCB* blockedOnTime;

#endif /* LIST_H_ */
