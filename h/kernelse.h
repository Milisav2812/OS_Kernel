/*
 *	Predmet: Operativni Sistemi 1
 *
 *	Student: Milos Milisavljevic 2015/0588
 *	Fajl: kernelse.h
 *
 *	Opis: Klasa KernelSem predstavlja jezgro semafora
 *	U njoj su definisane sve metode koje koristi semafor
 */

#ifndef H_KERNELSE_H_
#define H_KERNELSE_H_

#include "kernelse.h"
#include "semaphor.h"
#include "kernel.h"
#include "SCHEDULE.h"
#include "pcb.h"
#include "thread.h"

class KernelSem{
protected:
	KernelSem(int init = 1);
	virtual ~KernelSem();

	virtual int wait(int toBlock);
	virtual void signal();

	int val () const;

private:
	friend class Semaphore;
	friend class KernelEv;

	List blockThread; // Lista blokiranih niti na semaforu
	volatile int value;
};

#endif /* H_KERNELSE_H_ */
