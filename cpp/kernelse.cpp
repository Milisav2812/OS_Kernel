/*
 *	Predmet: Operativni Sistemi 1
 *
 *	Student: Milos Milisavljevic 2015/0588
 *	Fajl: kernelse.cpp
 *
 *	Opis: Klasa KernelSem predstavlja jezgro semafora
 *	U njoj su definisane sve metode koje koristi semafor
 */

#include "kernelse.h"
#include "semaphor.h"
#include "kernel.h"
#include "SCHEDULE.h"
#include "pcb.h"
#include "thread.h"

#include <stdio.h>

int ret;

/*
 * Konstruktor KernelSem()
 * Postavlja vrednost semafora
 */
KernelSem::KernelSem(int init){
	#ifndef BCC_BLOCK_IGNORE
		lock();
	#endif

	value = init;

	#ifndef BCC_BLOCK_IGNORE
		unlock();
	#endif
}

/*
 * Destruktor KernelSem()
 * Sve niti koje su blokirane na semaforu stavlja u Scheduler
 */
KernelSem::~KernelSem(){
	#ifndef BCC_BLOCK_IGNORE
		lock();
	#endif

	PCB* temp = blockThread.get();
	while(temp){
		Scheduler::put(temp);
		temp = blockThread.get();
	}

	#ifndef BCC_BLOCK_IGNORE
		unlock();
	#endif
}

/*
 * wait() vrsi blokiranje niti
 * Verzija 1: toBlock=0. Sluzi kao provera da li bi se nit blokirala. Ako je val<0, samo se izlazi iz metode
 * Verzija 2: toBlock>0. Klasican wait()
 */
int KernelSem::wait(int toBlock){
	#ifndef BCC_BLOCK_IGNORE
		lock();
	#endif

	if(toBlock==0){
		if(value<=0){
			ret = -1;
		}else{
			value--;
			ret = 0;
		}
	}
	else{
		if(--value<0){
			Kernel::runningThread->state = PCB::BLOCKED;
			blockThread.put((PCB*)Kernel::runningThread);
			dispatch();
			ret = 1;
		}else{
			ret = 0;
		}
	}
	#ifndef BCC_BLOCK_IGNORE
		unlock();
	#endif
	return ret;
}

/*
 * signal() odblokira prvu nit u listi
 */
void KernelSem::signal(){
	#ifndef BCC_BLOCK_IGNORE
		lock();
	#endif

	if(value++<0){
		PCB* temp = blockThread.get();
		Scheduler::put(temp);
	}

	#ifndef BCC_BLOCK_IGNORE
		unlock();
	#endif
}

/*
 * val() vraca vrednost semafora
 */
int KernelSem::val() const {
	return value;
}

























