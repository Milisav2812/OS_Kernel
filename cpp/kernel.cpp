/*
 *	Predmet: Operativni Sistemi 1
 *
 *	Student: Milos Milisavljevic 2015/0588
 *	Fajl: kernel.cpp
 *
 *	Opis: Klasa Kernel pokrece i zaustavlja sistem,
 *	zamenjuje potrebne prekidne rutine i na kraju
 *	vrši njihovu restauraciju
 */

#include "kernel.h"
#include "SCHEDULE.h"
#include "pcb.h"
#include "thread.h"
#include "sleep.h"

#include <dos.h>
#include <stdio.h>

class Sleep;

// Inicijalizacija statickih promenljivih
pInterrupt Kernel::oldRoutine = 0;
volatile int Kernel::dispatched = 0;
PCB* Kernel::runningThread = 0;
Thread* Kernel::startThread = 0;
Idle* Kernel::idleThread = 0;
Sleep* Kernel::sleepList = new Sleep();

// Pomocne promenljive. Koriste se prilikom promene konteksta u timerIR-u
unsigned tss, tsp;

/*
 * startOS() pokrece sistem
 * Stvara startThread i idleThread
 * Poziva f-ju init()
 */
void Kernel::startOS(){
	#ifndef BCC_BLOCK_IGNORE
		lock();
	#endif

	init();

	startThread = new Thread(defaultStackSize, 1);
	startThread->myPCB->state = PCB::READY;

	/*
	 * Postavlja se startThread da bude runningThread
	 * Vazno je da se ne pokrece. Prilikom prve promene konteksta ce se startThread vezati na stack Main-a
	 * Pa ce nakon povratka u Main znati odakle treba da nastavi
	 */
	runningThread = startThread->myPCB;

	// Stvaranje i pokretanje idle niti
	idleThread = new Idle();
	idleThread->start();


	#ifndef BCC_BLOCK_IGNORE
		unlock();
	#endif
}

/*
 * shutDown() gasi sistem
 * Poziva f-ju restore()
 */
void Kernel::shutDown(){

	// Provera da li su sve niti ugasene
	if(runningThread != startThread->myPCB) return;

	#ifndef BCC_BLOCK_IGNORE
		lock();
	#endif

	delete startThread;
	delete idleThread;
	restore();

	#ifndef BCC_BLOCK_IGNORE
		unlock();
	#endif
}

/*
 * Dispatch() vrsi eksplicitnu promenu konteksta
 */
void Kernel::Dispatch(){
	#ifndef BCC_BLOCK_IGNORE
		lock();
	#endif

	// Manuelno postavljamo promenljivu dispatched na 1 da bismo osigurali da ce doci do promene konteksta
	dispatched = 1;
	timerIR();
	dispatched = 0;

	#ifndef BCC_BLOCK_IGNORE
		unlock();
	#endif
}

/*
 * timerIR() je modifikovana prekidna rutina za tajmer
 * Automatski se poziva svakih 55msec
 */
void interrupt Kernel::timerIR(...){

	// Ako se tajmer sam pozvao, onda se smanjuje vreme uspavanih niti, ako ih ima
	if(!dispatched && sleepList->head!=0){

		sleepList->head->sleepTime -= 1;

		// Probudjena nit se stavlja u Scheduler
		while(sleepList->head->sleepTime == 0){
			Scheduler::put(sleepList->get());
		}
	}

	// Smanjuje se dodeljeno vreme niti za 1
	if(!dispatched && runningThread->timeSlice !=0)
		runningThread->timeCount -= 1;

	// Ako je isteklo dodeljeno vreme niti ili se eksplicitno pozvala metoda, vrsi se promena konteksta
	if(runningThread->timeCount == 0 || dispatched){

		// U slucaju da se uslo u petlju eksplicitnim zahtevom
		dispatched = 0;

		// Cuvamo SP u pomocne promenljive
		#ifndef BCC_BLOCK_IGNORE
				asm{
					mov tsp, sp
					mov tss, ss
				}
		#endif

		// Cuvamo SP u PCB trenutne niti
		runningThread->sp = tsp;
		runningThread->ss = tss;

		// Stavljamo u Scheduler samo ako je nit spremna za rad i ako nije idle nit
		if( runningThread->state == PCB::READY && runningThread != idleThread->myPCB )
			Scheduler::put((PCB*)runningThread);

		runningThread = Scheduler::get();
		// Ako je Scheduler prazan, idleThread postaje runningThread
		if(runningThread==0)
			runningThread=idleThread->myPCB;

		// Novi SP stavljamo u pomocne promenljive
		tsp = runningThread->sp;
		tss = runningThread->ss;

		// Resetovanje counter-a za vreme
		runningThread->timeCount = runningThread->timeSlice == 0 ? -1 : runningThread->timeSlice;

		#ifndef BCC_BLOCK_IGNORE
				asm{
					mov ss, tss
					mov sp, tsp
				}
		#endif
	}

	// Pri svakom otkucaju tajmera se poziva metoda tick()
	if(!dispatched){
			tick();
			// Ako nije eksplicitno trazena promena konteksta, onda se pozova stara prekidna rutina
			#ifndef BCC_BLOCK_IGNORE
					asm int 60h;
			#endif
		}


}

/*
 * init() zamenjuje originalnu prekidnu rutinu tajmera sa modifikovanom
 * oldRoutine cuva staru prekidnu rutinu
 * Na 8h se nalazi nova, a na 60h stara
 */
void Kernel::init(){
	#ifndef BCC_BLOCK_IGNORE
		lock();
		oldRoutine = getvect(0x08);
		setvect(0x08, timerIR);
		setvect(0x60, oldRoutine);
		unlock();
	#endif

}

/*
 * restore() vrsi restauraciju prekidnih rutina
 */
void Kernel::restore(){
	#ifndef BCC_BLOCK_IGNORE
		lock();
		setvect(0x08, oldRoutine);
		unlock();
	#endif
}

/*
 * wrapper() predstavlja omotac virtuelne f-je run()
 * On pokrece run() metodu niti
 * Poziva se prilikom stvaranja niti
 */
void Kernel::wrapper(){

	runningThread->myThread->run();

	#ifndef BCC_BLOCK_IGNORE
		lock();
	#endif

	// Nakon zavrsetka run() metode, postavlja se stanje niti na FINISHED
	runningThread->state = PCB::FINISHED;

	// Sve niti koje su cekale na ovu da se zavrsi sad se stavljaju u scheduler(waitToComplete)
	PCB* temp;
	while(runningThread->waitList->length() > 0){

		temp = runningThread->waitList->get();
		temp->state = PCB::READY;
		Scheduler::put(temp);

	}

	// Manuelno se poziva dispatch() jer program nece sam promeniti kontekst
	dispatch();
}









