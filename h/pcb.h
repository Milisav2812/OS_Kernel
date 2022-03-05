/*
 *	Predmet: Operativni Sistemi 1
 *
 *	Student: Milos Milisavljevic 2015/0588
 *	Fajl: pcb.h
 *
 *	Opis: Klasa PCB predstavlja jezgro klase Thread
 *	U njoj se stvara PCB niti, njen stek i ostalo
 */

#ifndef H_PCB_H_
#define H_PCB_H_

#include "thread.h"
#include "kernel.h"
#include "list.h"

class Kernel;
class Thread;

class PCB{
public:
	PCB(Thread* myThread, StackSize stackSize, Time timeSlice);
	~PCB();

	Thread* myThread;

	static const int NEW, READY, BLOCKED, SLEEP, FINISHED;

	StackSize stackSize; // Velicina steka
	unsigned int ss,sp; // Stack Pointer
	unsigned* stack; // Pokazivac na stek

	volatile int state; // Stanje kreirane niti

	Time timeSlice; // Vreme dodeljeno niti
	volatile Time timeCount; // Koliko vremena je nit radila

	List* waitList; // Lista niti koje cekaju na ovu da se zavrsi(waitToComplete)

	void createStack();

	friend class Kernel;
	friend class Idle;
};

#endif /* H_PCB_H_ */
