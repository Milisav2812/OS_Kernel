/*
 *	Predmet: Operativni Sistemi 1
 *
 *	Student: Milos Milisavljevic 2015/0588
 *	Fajl: pcb.cpp
 *
 *	Opis: Klasa PCB predstavlja jezgro klase Thread
 *	U njoj se stvara PCB niti, njen stek i ostalo
 */

#include "pcb.h"
#include "kernel.h"
#include <dos.h>

class Kernel;

// Moguca stanja niti
const int PCB::NEW = 0;
const int PCB::READY = 1;
const int PCB::BLOCKED = 2;
const int PCB::SLEEP = 3;
const int PCB::FINISHED = 4;

/*
 * Konstruktor PCB-a
 * Dodeljuje vrednosti atributima klase
 */
PCB::PCB(Thread* myThread, StackSize stackSize, Time timeSlice){

	#ifndef BCC_BLOCK_IGNORE
		lock();
	#endif

	this->stackSize = stackSize;
	this->myThread = myThread;
	this->timeSlice = timeSlice;

	// U slucaju da je vreme niti beskonacno(=0)
	if(timeSlice==0) this->timeCount = -1;
	else this->timeCount = timeSlice;

	// Stanje niti je NEW, sto znaci da je stvorena a nije spremna za rad
	state = NEW;

	// Pokazivac na stek je 0, jer jos nije stvoren stek
	stack = 0;
	ss = 0;
	sp = 0;

	waitList = new List();
	#ifndef BCC_BLOCK_IGNORE
		unlock();
	#endif
}

/*
 * createStack() stvara stek niti
 * Kao f-ju niti se stavlja metoda Kernel::wrapper() umesto Thread::run()
 * U compile vremenu nemamo pristup run() metodi jer je ona virtuelna
 * Kada bismo je stavili pokrenuli bismo metodu run() klase Thread, koja je prazna
 */
void PCB::createStack(){
	#ifndef BCC_BLOCK_IGNORE
		lock();
	#endif

	// Stvaranje stack-a
	// n/2 jer je sizeof(unsigned) = 2B
	int n = this->stackSize/2;
	unsigned* st = new unsigned[n];
	stack = st;

	// Postavlja I bit na 1
	st[n - 1] = 0x200;

	#ifndef BCC_BLOCK_IGNORE
		st[n - 2] = FP_SEG( Kernel::wrapper );
		st[n - 3] = FP_OFF( Kernel::wrapper );
	#endif

	#ifndef BCC_BLOCK_IGNORE
		// -12 jer se ostavlja prostor za registre: ax,bx,cx,dx,es,ds,si,di
		this->sp = FP_OFF(st + n - 12);
		this->ss = FP_SEG(st + n - 12);
	#endif

	#ifndef BCC_BLOCK_IGNORE
		unlock();
	#endif
}

/*
 * Destruktor klase PCB
 */
PCB::~PCB(){
	#ifndef BCC_BLOCK_IGNORE
		lock();
	#endif

	delete stack;
	delete waitList;

	#ifndef BCC_BLOCK_IGNORE
		unlock();
	#endif
}








































