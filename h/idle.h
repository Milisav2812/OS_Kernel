/*
 *	Predmet: Operativni Sistemi 1
 *
 *	Student: Milos Milisavljevic 2015/0588
 *	Fajl: idle.h
 *
 *	Opis: Klasa Idle predstavlja idle nit
 *	Ona se pokrece kad je Scheduler prazan
 */

#ifndef H_IDLE_H_
#define H_IDLE_H_

#include "Thread.h"

class Thread;

class Idle : public Thread{
public:
	Idle();
	virtual void run();
	void start();
};

#endif /* H_IDLE_H_ */
