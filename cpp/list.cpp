/*
 *	Predmet: Operativni Sistemi 1
 *
 *	Student: Milos Milisavljevic 2015/0588
 *	Fajl: list.cpp
 *
 *	Opis: Klasa List predstavlja jednostruko ulancanu listu
 */

#include "list.h"
#include "kernel.h"
#include "pcb.h"
#include "thread.h"

#include <stdio.h>

/*
 * Konstruktor List()
 */
List::List(){
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
 * Destruktor List()
 * Brise svaki element u listi
 */
List::~List(){
	#ifndef BCC_BLOCK_IGNORE
		lock();
	#endif

	Elem* t;
	while(head!=0){
		t = head;
		head = head->next;
		delete t;
		len--;
	}

	#ifndef BCC_BLOCK_IGNORE
		lock();
	#endif
}

/*
 * get() vraca prvi element u listi i time ga brise
 */
PCB* List::get(){
	if(head==0) return 0;

	#ifndef BCC_BLOCK_IGNORE
		lock();
	#endif

	Elem* temp;
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
 * put() stavlja novi element na kraj liste
 */
void List::put(PCB* item){
	#ifndef BCC_BLOCK_IGNORE
		lock();
	#endif
	if(head==0){

		head = new Elem();
		head->item = item;
		head->next = 0;

	}
	else{

		Elem* t = head;
		while(t->next!=0) t = t->next;
		Elem* newone = new Elem();
		newone = new Elem();
		newone->item = item;
		newone->next = 0;
		t->next = newone;

	}
	len++;

	#ifndef BCC_BLOCK_IGNORE
		unlock();
	#endif
}

/*
 * length() vraca broj elemenata u listi
 */
int List::length() const{
	return len;
}

/*
 * pisi() je pomocna metoda
 * Ispisuje onoliko slova "A" koliko ima elemenata
 */
void List::pisi()
{
	Elem* tek = head;
	while( tek != 0 )
	{
		printf("A ");
		tek = tek->next;
	}
	printf("\n");
}


/*
 * verify() proverava da li se odredjeni element nalazi u listi
 * Vraca 1 ako je element pronadjen
 * 0 u suprotnom
 */
int List::verify(PCB* item){

	if(head==0) return 0;

	#ifndef BCC_BLOCK_IGNORE
		lock();
	#endif

	Elem* t = head;
	while(t!=0){
		if(t->item == item){
			#ifndef BCC_BLOCK_IGNORE
				unlock();
			#endif
			return 1;
		}
		t = t->next;
	}

	#ifndef BCC_BLOCK_IGNORE
		unlock();
	#endif
	return 0;
}

/*
 * get_specific() vraca odredjeni element bez izbacivanja iz liste
 * Vraca 0 ako element nije pronadjen
 */
PCB* List::get_specific(PCB* item){
	if(head==0) return 0;

	#ifndef BCC_BLOCK_IGNORE
		lock();
	#endif

	Elem* t = head;
	while(t->next!=0){
		if(t->item == item){
			#ifndef BCC_BLOCK_IGNORE
				unlock();
			#endif
			return t->item;
		}
		t = t->next;
	}

	#ifndef BCC_BLOCK_IGNORE
		unlock();
	#endif
	return 0;
}

/*
 * get_specific_and_delete() vraca odredjeni element i izbacuje ga iz liste
 * Vraca 0 ako element nije pronadjen
 */
PCB* List::get_specific_and_delete(PCB* item){
	if(head==0) return 0;

	#ifndef BCC_BLOCK_IGNORE
		lock();
	#endif

	Elem* t = head;
	Elem* p = 0;
	while(t->next!=0){
		if(t->item == item){

			PCB* temp = t->item;
			p->next = t->next;
			delete t;

			#ifndef BCC_BLOCK_IGNORE
				unlock();
			#endif
			return temp;
		}
		p = t;
		t = t->next;
	}

	#ifndef BCC_BLOCK_IGNORE
		unlock();
	#endif
	return 0;
}

/*
 * get_first_without_state() vraca prvi element u listi i time ga brise
 * Ne manje stanje u READY
 */
PCB* List::get_first_without_state(){
	if(head==0) return 0;

	#ifndef BCC_BLOCK_IGNORE
		lock();
	#endif

	Elem* temp;
	temp = head;
	if(head->next)
		head = head->next;
	else
		head = 0;

	len--;

	PCB* t = temp->item;
	delete temp;

	#ifndef BCC_BLOCK_IGNORE
		unlock();
	#endif
	return t;
}




















