/*
 *	Predmet: Operativni Sistemi 1
 *
 *	Student: Milos Milisavljevic 2015/0588
 *	Fajl: thread.cpp
 *
 *	Opis: Klasa Thread je glavna klasa za niti
 *	Ona ih stvara, pokrece, uspavljuje i ostalo
 */

#include "thread.h"
#include "kernel.h"
#include "pcb.h"
#include "SCHEDULE.H"
#include "sleep.h"

#include <stdio.h>

class Kernel;

/*
 * Konstruktor klase Thread
 * Stvara PCB niti
 */
Thread::Thread(StackSize stackSize, Time timeSlice){
	#ifndef BCC_BLOCK_IGNORE
		lock();
	#endif

	myPCB = new PCB(this, stackSize, timeSlice);

	#ifndef BCC_BLOCK_IGNORE
		unlock();
	#endif
}

/*
 * start() pokrece nit
 * Postavlja stanje na READY
 * Stvara stek
 * Ubacuje je u Scheduler
 */
void Thread::start(){
	#ifndef BCC_BLOCK_IGNORE
		lock();
	#endif

	myPCB->state = PCB::READY;

	myPCB->createStack();

	Scheduler::put(myPCB);

	#ifndef BCC_BLOCK_IGNORE
		unlock();
	#endif
}

/*
 * Destruktor klase Thread
 * Poziva metodu waitToComplete()
 * Brise PCB
 */
Thread::~Thread(){
	#ifndef BCC_BLOCK_IGNORE
		lock();
	#endif

	waitToComplete();
	delete myPCB;

	#ifndef BCC_BLOCK_IGNORE
		unlock();
	#endif
}

/*
 * dispatch() vrsi promenu konteksta
 * Poziva kernelovu metodu dispatch()
 */
void dispatch(){
	#ifndef BCC_BLOCK_IGNORE
		lock();
	#endif

	Kernel::Dispatch();

	#ifndef BCC_BLOCK_IGNORE
		unlock();
	#endif
}

/*
 * waitToComplete()
 *
 * runningThread je nit koja je pozvala metodu
 * this je nit na kojoj se blokira
 * Blokira se runningThread dok se nit this ne zavrsi
 * Kad se zavrsi nit na kojoj se blokiralo, u njenoj wrapper metodi ce se odblokirati sve niti
 *
 * Kada roditelj nit udje u destruktor, ona poziva waitToComplete() svoje dece. Tako da se blokira dok se sva deca ne zavrse
 */
void Thread::waitToComplete(){

	#ifndef BCC_BLOCK_IGNORE
		lock();
	#endif

	// Ne moze da se blokira na samoj sebi
	if( myPCB == (PCB*)Kernel::runningThread ) {
		#ifndef BCC_BLOCK_IGNORE
			unlock();
		#endif
		return;
	}

	// Ne moze da se blokira na niti koja je zavrsena
	if(myPCB->state == PCB::FINISHED){
		#ifndef BCC_BLOCK_IGNORE
			unlock();
		#endif
		return;
	}

	// Ne moze da se blokira na niti koja jos nije ni zapoceta
	if(myPCB->state == PCB::NEW){
		#ifndef BCC_BLOCK_IGNORE
			unlock();
		#endif
		return;
	}

	// Ne moze da se blokira na startThread-u
	if(this == Kernel::startThread){
		#ifndef BCC_BLOCK_IGNORE
			unlock();
		#endif
		return;
	}

	// Ne moze da se blokira na idleThread-u
	if(this == Kernel::idleThread){
		#ifndef BCC_BLOCK_IGNORE
			unlock();
		#endif
		return;
	}

	Kernel::runningThread->state = PCB::BLOCKED;
	myPCB->waitList->put((PCB*)Kernel::runningThread);
	dispatch();

	#ifndef BCC_BLOCK_IGNORE
		unlock();
	#endif
}

/*
 * sleep() uspavljuje nit na vreme odredjeno parametrom
 * Nit se stavi u stanje BLOCKED
 * Stavlja se u Scheduler
 */
void Thread::sleep(Time timeToSleep){
	#ifndef BCC_BLOCK_IGNORE
		lock();
	#endif

	Kernel::runningThread->state = PCB::BLOCKED;
	Kernel::sleepList->put( (PCB*)Kernel::runningThread, timeToSleep);
	dispatch();

	#ifndef BCC_BLOCK_IGNORE
		unlock();
	#endif
}



























