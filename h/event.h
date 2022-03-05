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

#ifndef H_EVENT_H_
#define H_EVENT_H_

#include "ivtentry.h"

/*
 * PrepareEntry je makro koji ce prekidnu rutinu tastature(9h) da zameni sa novom prekidnom rutinom
 * routine9 je prekidna rutina za tastaturu
 * Nova prekidna rutina poziva staru i vrsi signal na dogadjaju
 */
#define PREPAREENTRY(ivtno,old) \
void interrupt routine##ivtno(...); \
IVTEntry ent##ivtno(ivtno,routine##ivtno); \
void interrupt routine##ivtno(...) { \
	if (old) ent##ivtno.callOldInterrupt(); \
	ent##ivtno.signal(); \
}

typedef unsigned char IVTNo;
class KernelEv;

class Event{
public:
	Event(IVTNo ivtNo);
	~Event();

	void wait();
protected:
	friend class KernelEv;
	void signal();

private:
	KernelEv* myImpl;
};



#endif /* H_EVENT_H_ */
