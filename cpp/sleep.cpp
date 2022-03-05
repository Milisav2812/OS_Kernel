/*
 *	Predmet: Operativni Sistemi 1
 *
 *	Student: Milos Milisavljevic 2015/0588
 *	Fajl: sleep.h
 *
 *	Opis: Klasa Sleep predstavlja listu uspavanih niti
 */

#include "sleep.h"
#include "kernel.h"
#include "pcb.h"
#include "thread.h"
#include <stdio.h>

/*
 * Konstruktor Sleep()
 */
Sleep::Sleep(){
	#ifndef BCC_BLOCK_IGNORE
		lock();
	#endif

	this->len = 0;
	this->head = 0;

	#ifndef BCC_BLOCK_IGNORE
		unlock();
	#endif
}

/*
 * Destruktor Sleep()
 */
Sleep::~Sleep(){
	#ifndef BCC_BLOCK_IGNORE
		lock();
	#endif

	sleepElem* t;
	while(head!=0){
		t = head;
		head = head->next;
		delete t;
		len--;
	}

	#ifndef BCC_BLOCK_IGNORE
		unlock();
	#endif
}

/*
 * put() stavlja novi element u listu
 * Lista je napravljena tako da je vreme svake niti postavljeno relativno u odnosu na nit koja se nalazi ispred nje
 * Na taj nacin se pri otkucaju tajmera smanjuje vrednost samo prve niti
 * Nova nit se postavlja na odgovarajucu poziciju u listi
 */
void Sleep::put(PCB* item, Time sleepTime){

	#ifndef BCC_BLOCK_IGNORE
		lock();
	#endif

	// Prazna lista
	if(head==0){

		head = new sleepElem();
		head->item = item;
		head->sleepTime = sleepTime;
		head->next = 0;

	}else{

		// Trazimo mesto novom sleepElementu
		int sleep = 0;
		sleepElem *t = head;
		sleepElem *p = 0; // Pokazuje na element iza kojeg treba staviti novi element
		sleep += t->sleepTime;
		while(sleep <= sleepTime && t->next!=0) {
			p = t;
			t=t->next;
			sleep += t->sleepTime;
		}

		// Kreiranje novog elementa
		sleepElem * newElem = new sleepElem();
		newElem->item = item;

		// <= Ispred, => Iza
		// SLEEP <= SLEEPTIME -> Stavljam iza elementa na koji pokazuje t
		// SLEEP > SLEEPTIME -> Stavljam ispred elementa na koji pokazuje t

		// Situacija 1: Ubacujem na prvo mesto u listi. head pokazuje na novi element
		if( p==0 && sleep>sleepTime  ){

			newElem->sleepTime = sleepTime;

			// Podesavanje sleepTime starog elementa
			t->sleepTime -= sleepTime;

			newElem->next = t;
			head = newElem;
		}
		// Situacija 2: Ubacujemo element na kraj liste
		else if( t->next==0 && sleep<=sleepTime ){

			newElem->sleepTime = sleepTime - sleep;

			t->next = newElem;
			newElem->next = 0;
		}
		// Situacija 3: Vise elemenata u listi -> Stavljamo ispred elementa t
		else if(sleep>sleepTime) {

			// Racunanje sleepTime
			int x = t->sleepTime;
			t->sleepTime = sleep - sleepTime;
			newElem->sleepTime = x - t->sleepTime;

			p->next = newElem;
			newElem->next = t;
		}
	}

	len++;
	#ifndef BCC_BLOCK_IGNORE
		unlock();
	#endif
}

/*
 * get() dohvata i brise prvi element u listi
 */
PCB* Sleep::get(){
	if(head==0) return 0;

	#ifndef BCC_BLOCK_IGNORE
		lock();
	#endif

	sleepElem* temp;
	temp = head;
	if(head->next)
		head = head->next;
	else
		head = 0;

	len--;

	PCB* t = temp->item;
	t->state = PCB::READY;
	delete temp;

	#ifndef BCC_BLOCK_IGNORE
		unlock();
	#endif
	return t;
}

/*
 * length() vraca broj elemenata u listi
 */
int Sleep::length() const{
	return len;
}

/*
 * pisi() je pomocna metoda
 * Ispisuje vremena elemenata u listi
 */
void Sleep::pisi()
{
	sleepElem* tek = head;
	while( tek != 0 )
	{
		printf("%d ", tek->sleepTime);
		tek = tek->next;
	}
	printf("\n");
}


























