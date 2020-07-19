#ifndef EVENT_H_
#define EVENT_H_

#include "ivtEntry.h";

typedef unsigned char IVTNo;

#define PREPAREENTRY(ivtNo, callOld)\
		void interrupt inter##ivtNo(...);\
		IVTEntry newEntry##ivtNo = IVTEntry(ivtNo, &inter##ivtNo);\
		void interrupt inter##ivtNo(...){\
			newEntry##ivtNo.signal();\
			if(callOld == 1) newEntry##ivtNo.callOldRout();\
			dispatch();\
		}


class Event {
public:
	Event(IVTNo ivtNo);
	~Event();

	void wait();

protected:
	friend class KernelEv;
	void signal(); //can call KernelEv

private:
	KernelEv* myImpl;
};




#endif /* EVENT_H_ */
