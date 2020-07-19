#include <iostream.h>;
#include "list.h";

extern ListPCB* allThreads = 0;
extern ListPCB* blockedOnTime = 0;

ListPCB::ListPCB() {
	first = 0;
	last = 0;
	current = 0;
	previous = 0;
}
ListPCB::~ListPCB() {
	deleteList();
}

ListPCB& ListPCB::add(PCB* newElem) {
	#ifndef BCC_BLOCK_IGNORE
		lockCout
	#endif
		Elem* temp = new Elem(newElem);
		last = (!first ? first : last->next) = temp;
	#ifndef BCC_BLOCK_IGNORE
		unlockCout
	#endif
	return *this;
}

ListPCB& ListPCB::addInTimeOrder(PCB* newElem) {
	#ifndef BCC_BLOCK_IGNORE
		lockCout
	#endif
		Elem* temp = new Elem(newElem);
		if(first == 0) { first = last = temp; }
		else {
			previous = 0;
			for(current = first; current != 0; next()) {
				if(temp->data->blockedTime <= current->data->blockedTime) {
					current->data->blockedTime -= temp->data->blockedTime;
					temp->next = current;
					if(previous != 0) previous->next = temp;
					if(current == first) first = temp;
					break;
				}
				else temp->data->blockedTime -= current->data->blockedTime;
			}
			if(current == 0) {
				last->next = temp;
				last = temp;
			}
		}
	#ifndef BCC_BLOCK_IGNORE
		unlockCout
	#endif
	return *this;
}

void ListPCB::toBeginning() {
	#ifndef BCC_BLOCK_IGNORE
		lockCout
	#endif
	current = first;
	previous = 0;
	#ifndef BCC_BLOCK_IGNORE
		unlockCout
	#endif
}

void ListPCB::next() {
	#ifndef BCC_BLOCK_IGNORE
		lockCout
	#endif
	previous = current;
	current = current->next;
	#ifndef BCC_BLOCK_IGNORE
		unlockCout
	#endif
}

void ListPCB::deleteList() {
	#ifndef BCC_BLOCK_IGNORE
		lockCout
	#endif
	while (first) {
		Elem *old = first;
		first = first->next;
		#ifndef BCC_BLOCK_IGNORE
			lockCout
		#endif
		delete old;
		#ifndef BCC_BLOCK_IGNORE
			unlockCout
		#endif
	}
	last = 0;
	current = 0;
	previous = 0;
	#ifndef BCC_BLOCK_IGNORE
		unlockCout
	#endif
}

PCB* ListPCB::takeFromBeginning() {
	if (first != 0) {
		#ifndef BCC_BLOCK_IGNORE
			lockCout
		#endif
		if(current == first) current = current->next;
		if(previous == first) previous = 0;
		PCB* t = first->data;
		Elem *temp = first;
		first = first->next;
		if(first == 0) last = 0;
		#ifndef BCC_BLOCK_IGNORE
			lockCout
		#endif
		delete temp;
		#ifndef BCC_BLOCK_IGNORE
			unlockCout
		#endif
		#ifndef BCC_BLOCK_IGNORE
			unlockCout
		#endif
		return t;
	}
	else return 0;
}

void ListPCB::deleteCurrent() {
	#ifndef BCC_BLOCK_IGNORE
		lockCout
	#endif
	Elem* old = current;
    if(current == first) {
    	first = first->next;
    	if(current == last) last = 0;
    	current = first;
    }
    else {
    	if(current == last) last = previous;
    	previous->next = current->next;
    	current = current->next;
    }
	#ifndef BCC_BLOCK_IGNORE
		lockCout
	#endif
	delete old;
	#ifndef BCC_BLOCK_IGNORE
		unlockCout
	#endif
	#ifndef BCC_BLOCK_IGNORE
		unlockCout
	#endif
}

