/*
 *	Predmet: Operativni Sistemi 1
 *
 *	Student: Milos Milisavljevic 2015/0588
 *	Fajl: sleep.h
 *
 *	Opis: Klasa Sleep predstavlja listu uspavanih niti
 */

#ifndef H_SLEEP_H_
#define H_SLEEP_H_

#include "thread.h"

class PCB;

struct sleepElem{
public:
	PCB* item; // Nit
	Time sleepTime; // Vreme spavanja
	sleepElem* next; // Pokazivac na sledecu nit u listi
};

class Sleep{
public:

	Sleep();
	~Sleep();

	PCB* get();
	void put(PCB*,Time);
	int length() const;
	void pisi();

private:
	friend Kernel;

	sleepElem* head; // Glava liste
	int len; // Broj elemenata
};


#endif /* H_SLEEP_H_ */
