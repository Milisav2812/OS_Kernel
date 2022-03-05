/*
 *	Predmet: Operativni Sistemi 1
 *
 *	Student: Milos Milisavljevic 2015/0588
 *	Fajl: list.h
 *
 *	Opis: Klasa List predstavlja jednostruko ulancanu listu
 */

#ifndef H_LIST_H_
#define H_LIST_H_

class PCB;

struct Elem{
public:
	PCB* item; // Nit
	Elem* next; // Pokazivac na sledecu nit u listi
};

class List{
public:

	List();
	~List();

	PCB* get();
	void put(PCB*);
	int length() const;

	// Pomocne metode
	void pisi();
	int verify(PCB*);
	PCB* get_first_without_state();
	PCB* get_specific(PCB*);
	PCB* get_specific_and_delete(PCB*);


private:
	Elem* head; // Glava liste
	int len; // Broj elemenata
};


#endif /* H_LIST_H_ */
