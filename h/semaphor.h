/*
 *	Predmet: Operativni Sistemi 1
 *
 *	Student: Milos Milisavljevic 2015/0588
 *	Fajl: semaphor.h
 *
 *	Opis: Klasa Semaphore omogucuje koncept semafora
 *	tj. sinhronizaciju niti
 *	Sve metode u Semaphore se izvrsavaju pomocu klase KernelSem, koje predstavlja jezgro
 */

#ifndef H_SEMAPHOR_H_
#define H_SEMAPHOR_H_

class KernelSem;

class Semaphore{
public:
	Semaphore(int init = 1);
	virtual ~Semaphore();

	virtual int wait(int toBlock);
	virtual void signal();

	int val () const;

private:
	KernelSem* myImpl;
};

#endif /* H_SEMAPHOR_H_ */
