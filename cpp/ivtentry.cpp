/*
 *	Predmet: Operativni Sistemi 1
 *
 *	Student: Milos Milisavljevic 2015/0588
 *	Fajl: ivtentry.cpp
 *
 *	Opis: Klasa IVTEntry predstavlja ulaz u tabelu prekidnih rutina(Interrupt Vector Table)
 *	Nama je potreban ulaz 9 jer se na njemu nalazi prekidna rutina tastature
 *	Na ulaz 9 vezujemo novu prekidnu rutinu koja poziva signal() dogadjaja
 */

#include "ivtentry.h"
#include "kernelev.h"
#include "event.h"

#include <dos.h>

IVTEntry* IVTEntry::IVTTable[256]; // Ulazi u IV Tabelu, ima ih 256

/*
 * Konstruktor IVTEntry()
 * Na ulaz ivtNo stavlja novu prekidnu rutinu definisako u PrepareEntry makrou
 * oldIR cuva staru prekidnu rutinu
 */
IVTEntry::IVTEntry(int ivtNo, interruptRoutine newIR){
	oldIR = 0;
	this->myEvent = 0;

	this->ivtNo = ivtNo;
	IVTTable[ivtNo] = this;

	#ifndef BCC_BLOCK_IGNORE
		lock();
		oldIR = getvect(ivtNo);
		setvect(ivtNo, newIR);
		unlock();
	#endif
}

/*
 * Destruktor IVTEntry()
 * Vraca staru prekidnu rutinu na ulaz 9
 */
IVTEntry::~IVTEntry(){
	#ifndef BCC_BLOCK_IGNORE
		lock();
		setvect(ivtNo, oldIR);
		unlock();
	#endif
}

/*
 * signal() se poziva u novoj prekidnoj rutini
 * On ce da odblokira nit zablokiranu na dogadjaju
 */
void IVTEntry::signal(){
	myEvent->signal();
}

/*
 * callOldInterrupt() poziva staru prekidnu rutinu
 */
void IVTEntry::callOldInterrupt(){
	oldIR();
}




















