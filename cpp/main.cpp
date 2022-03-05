/*
 *	Predmet: Operativni Sistemi 1
 *
 *	Student: Milos Milisavljevic 2015/0588
 *	Fajl: main.h
 *
 *	Opis: Metoda Main
 *	Poziva metodu userMain
 */

#include <stdio.h>
#include "kernel.h"


extern int userMain(int argc, char* argv[]);

int main(int argc, char* argv[]){

	Kernel::startOS();

	int result = userMain(argc, argv);

	Kernel::shutDown();

	return 0;
}



