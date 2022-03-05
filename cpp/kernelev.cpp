/*
 *	Predmet: Operativni Sistemi 1
 *
 *	Student: Milos Milisavljevic 2015/0588
 *	Fajl: kernelev.cpp
 *
 *	Opis: Klasa KernelEv predstavlja jezgro klase Event
 *	U njoj su definisane sve metode koje Event koristi
 */

#include "kernelev.h"
#include "event.h"
#include "ivtentry.h"
#include "kernel.h"
#include "kernelse.h"

/*
 * Konstruktor KernelEv()
 * Novokreirani dogadjaj se stavlja u niz IVTTable na mesto ivtNo, ako je Entry vec definisan
 */
KernelEv::KernelEv(IVTNo ivtNo){
	if(IVTEntry::IVTTable[ivtNo]){
		this->ivtNo = ivtNo;
		this->owner = Kernel::runningThread->myThread; // Vlasnik dogadjaja je runningThread
		this->mySem = new KernelSem(0);
		IVTEntry::IVTTable[ivtNo]->myEvent = this;
	}
}

/*
 * Destruktor KernelEv
 */
KernelEv::~KernelEv(){
	delete mySem;
}

/*
 * wait() poziva wait() metodu semafora
 */
int KernelEv::wait(){
	if(Kernel::runningThread->myThread != owner) return 0;
	return mySem->wait(1);
}

/*
 * signal() poziva signal() metodu semafora
 */
void KernelEv::signal(){
	mySem->signal();
}








