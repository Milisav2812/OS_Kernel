/*
 *	Predmet: Operativni Sistemi 1
 *
 *	Student: Milos Milisavljevic 2015/0588
 *	Fajl: event.h
 *
 *	Opis: Klasa Event predstavlja omotac za Dogadjaj
 *	Dogadjaj je semafor. Za njega nije potrebno da bude neka nit u Scheduler-u koja ce odblokirati
 *	Signal za ovaj semafor se poziva prekidnom rutinom tastature
 */

#include "event.h"
#include "kernelev.h"
#include "kernel.h"

/*
 * Konstruktor Event()
 * Stvara objekat klase KernelEv
 */
Event::Event(IVTNo ivtNo){
	#ifndef BCC_BLOCK_IGNORE
		lock();
	#endif

	myImpl = new KernelEv(ivtNo);

	#ifndef BCC_BLOCK_IGNORE
		unlock();
	#endif
}

/*
 * Destruktor Event()
 */
Event::~Event(){
	#ifndef BCC_BLOCK_IGNORE
		lock();
	#endif

	delete myImpl;

	#ifndef BCC_BLOCK_IGNORE
		unlock();
	#endif
}

/*
 * wait()
 * Poziva wait() KernelEv
 */
void Event::wait(){
	#ifndef BCC_BLOCK_IGNORE
		lock();
	#endif

	myImpl->wait();

	#ifndef BCC_BLOCK_IGNORE
		unlock();
	#endif
}

/*
 * signal()
 * Poziva signal() KernelEv
 */
void Event::signal(){
	#ifndef BCC_BLOCK_IGNORE
		lock();
	#endif

	myImpl->signal();

	#ifndef BCC_BLOCK_IGNORE
		unlock();
	#endif
}













