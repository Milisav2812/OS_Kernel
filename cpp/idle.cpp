/*
 *	Predmet: Operativni Sistemi 1
 *
 *	Student: Milos Milisavljevic 2015/0588
 *	Fajl: idle.cpp
 *
 *	Opis: Klasa Idle predstavlja idle nit
 *	Ona se pokrece kad je Scheduler prazan
 */

#include "kernel.h"
#include "thread.h"
#include "pcb.h"

#include <dos.h>
#include <stdio.h>

/*
 * Konstruktor idle niti
 * Pravimo nit koja ce imati neku minimalnu vrednost skack-a, jer nece raditi nista
 * TimeSlice je 1, jer cemo na svakom otkucaju da proveravamo da li ima spremne niti u Scheduler-u
 * Ako nema, ova nit se vrti u while petlji
 */
Idle::Idle() : Thread(256, 1) {}

/*
 * run() metoda idle niti
 */
void Idle::run(){

	while(1);
}

/*
 * start() metoda idle niti
 * Stvara stek i postavlja stanje na READY
 * Ne stavljamo je u Scheduler!
 */
void Idle::start(){

	#ifndef BCC_BLOCK_IGNORE
		lock();
	#endif

	myPCB->state = PCB::READY;
	myPCB->createStack();

	#ifndef BCC_BLOCK_IGNORE
		unlock();
	#endif

}


