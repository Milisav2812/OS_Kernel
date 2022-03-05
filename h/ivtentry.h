/*
 *	Predmet: Operativni Sistemi 1
 *
 *	Student: Milos Milisavljevic 2015/0588
 *	Fajl: ivtentry.h
 *
 *	Opis: Klasa IVTEntry predstavlja ulaz u tabelu prekidnih rutina(Interrupt Vector Table)
 *	Nama je potreban ulaz 9 jer se na njemu nalazi prekidna rutina tastature
 *	Na ulaz 9 vezujemo novu prekidnu rutinu koja poziva signal() dogadjaja
 */

#ifndef H_IVTENTRY_H_
#define H_IVTENTRY_H_

typedef void interrupt (*interruptRoutine)(...);

class IVTEntry{
public:
	IVTEntry(int, interruptRoutine);
	~IVTEntry();
	void signal();
	void callOldInterrupt();
private:
	friend class KernelEv;

	static IVTEntry* IVTTable[];
	unsigned ivtNo;
	KernelEv* myEvent;
	interruptRoutine oldIR;
};


#endif /* H_IVTENTRY_H_ */
