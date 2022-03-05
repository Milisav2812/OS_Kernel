/*
 *	Predmet: Operativni Sistemi 1
 *
 *	Student: Milos Milisavljevic 2015/0588
 *	Fajl: kernel.h
 *
 *	Opis: Klasa Kernel pokrece i zaustavlja sistem,
 *	zamenjuje potrebne prekidne rutine i na kraju
 *	vrši njihovu restauraciju
 */

#ifndef H_KERNEL_H_
#define H_KERNEL_H_

#include "thread.h"
#include "pcb.h"
#include "list.h"
#include "idle.h"
#include "sleep.h"

/*
 * Makroi koji obezbedjuju atomicnost
 * Koristi se pushf i popf zbog gnjezdjenja
 * vise atomicnih f-ja
 */
#define lock() asm{\
		pushf;\
		cli;\
	}

#define unlock() asm popf

typedef void interrupt (*pInterrupt)(...);

class Thread;

// Staticka klasa koja sadrzi sve metode i promenljive kojima upravlja jezgro OS-a
class Kernel{
public:

	static void startOS();
	static void shutDown();
	static void Dispatch();

private:
	friend class PCB;
	friend class Thread;
	friend class Idle;
	friend class KernelSem;
	friend class KernelEv;

	static Thread* startThread;
	static Idle* idleThread;
	static PCB* runningThread;

	static Sleep *sleepList;

	static volatile int dispatched;

	static void init();
	static void restore();

	static void interrupt timerIR(...);
	static pInterrupt oldRoutine;

	static void wrapper();
};


#endif /* H_KERNEL_H_ */
