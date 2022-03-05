/*
 *	Predmet: Operativni Sistemi 1
 *
 *	Student: Milos Milisavljevic 2015/0588
 *	Fajl: kernelev.h
 *
 *	Opis: Klasa KernelEv predstavlja jezgro klase Event
 *	U njoj su definisane sve metode koje Event koristi
 */

#ifndef H_KERNELEV_H_
#define H_KERNELEV_H_

#include "event.h"
#include "thread.h"
#include "pcb.h"
#include "semaphor.h"

class Thread;
class PCB;
class KernelSe;

typedef unsigned char IVTNo;

class KernelEv{
public:
	KernelEv(IVTNo ivtNo);
	~KernelEv();
	int wait();
	void signal();
private:
	friend class Event;

	KernelSem* mySem;
	IVTNo ivtNo;
	Thread* owner;
};


#endif /* H_KERNELEV_H_ */
