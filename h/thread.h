/*
 *	Predmet: Operativni Sistemi 1
 *
 *	Student: Milos Milisavljevic 2015/0588
 *	Fajl: thread.h
 *
 *	Opis: Klasa Thread je glavna klasa za niti
 *	Ona ih stvara, pokrece, uspavljuje i ostalo
 */

#ifndef H_THREAD_H_
#define H_THREAD_H_

typedef unsigned long StackSize;
const StackSize defaultStackSize = 4096;

typedef unsigned int Time;
const Time defaultTimeSlice = 2; // 2 x 55ms

class PCB;

class Thread{
public:

	void start();
	void waitToComplete();
	virtual ~Thread();

	static void sleep(Time timeToSleep);

protected:
	friend class Kernel;
	friend class Idle;
	friend class PCB;
	Thread(StackSize stackSize = defaultStackSize, Time timeSlice = defaultTimeSlice);
	virtual void run() {}

private:
	PCB* myPCB;
};

void dispatch();
extern void tick();

#endif /* H_THREAD_H_ */
