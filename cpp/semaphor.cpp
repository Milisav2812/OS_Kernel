/*
 *	Predmet: Operativni Sistemi 1
 *
 *	Student: Milos Milisavljevic 2015/0588
 *	Fajl: semaphor.cpp
 *
 *	Opis: Klasa Semaphore omogucuje koncept semafora
 *	tj. sinhronizaciju niti
 *	Sve metode u Semaphore se izvrsavaju pomocu klase KernelSem, koje predstavlja jezgro
 */

#include "kernel.h"
#include "kernelse.h"
#include "semaphor.h"

/*
 * Konstruktor Semaphore()
 */
Semaphore::Semaphore(int init){
	#ifndef BCC_BLOCK_IGNORE
		lock();
	#endif

	myImpl = new KernelSem(init);

	#ifndef BCC_BLOCK_IGNORE
		unlock();
	#endif
}

/*
 * Destruktor Semaphore()
 */
Semaphore::~Semaphore(){
	#ifndef BCC_BLOCK_IGNORE
		lock();
	#endif

	delete myImpl;

	#ifndef BCC_BLOCK_IGNORE
		unlock();
	#endif
}

/*
 * wait() metoda
 * Poziva metodu wait() klase KernelSem
 */
int Semaphore::wait(int toBlock){
	#ifndef BCC_BLOCK_IGNORE
		lock();
	#endif

	int rez = myImpl->wait(toBlock);

	#ifndef BCC_BLOCK_IGNORE
		unlock();
	#endif

	return rez;
}

/*
 * signal() metoda
 * Poziva metodu signal() klase KernelSem
 */
void Semaphore::signal(){
	#ifndef BCC_BLOCK_IGNORE
		lock();
	#endif

	myImpl->signal();

	#ifndef BCC_BLOCK_IGNORE
		unlock();
	#endif
}

/*
 * val() vraca vrednost semafora
 */
int Semaphore::val() const{
	return myImpl->val();
}

























