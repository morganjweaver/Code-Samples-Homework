#include <iostream>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

using namespace std;
void *DentistTask(void* unused);
void *PatientTask(void*ID);
sem_t dentistReady, seatCountWriteAccess, patientsReady;
int seatsFree = 3;

int main (int argc, char *argv[])
{
	if (argc != 2) // argc should be 2 for correct execution
	// We print argv[0] assuming it is the program name
	{
	  cout << "Please try again with one argument for number of patients.\n";
	  return -1;
	}
  	int numPatients = atoi(argv[1]); //get user's desired number of patients

	//firt two semaphores have max val 1 and function as mutexes
	sem_init(&dentistReady, 0, 1);
	sem_init(&seatCountWriteAccess,0,1);

	sem_init(&patientsReady, 0, 0);

	pthread_t dentist;
	pthread_t patientThreads [numPatients];

	int dentistInt;
	int *patientInts = new int[numPatients];

	dentistInt = pthread_create(&dentist, NULL, DentistTask, NULL);
	
	if(dentistInt)
	{
		cout << "Error initializing Dentist thread." << endl;
		
	}
	for(int i = 0; i<numPatients; i++){
		int * ID = new int;
		*ID = i+1;
		patientInts[i] = pthread_create(&patientThreads[i], NULL, PatientTask, ID);
		if (patientInts[i]){cout << "Error initializing Patient threads." << endl;}
	}
		
	pthread_join(dentist, NULL);
	return 0;
}

void * DentistTask(void* unused){
	
	while(true)
	{
	cout << "Dentist trying to acquire patient." << endl;
		//sleep til a patient is ready
	sem_wait(&patientsReady);
	//lock seats when a patient arrives in order to increase free seat count
	cout << "Dentist trying to acquire lock for seatCountWriteAccess." << endl; 
	sem_wait(&seatCountWriteAccess);
	seatsFree++;
	cout <<"Incremented number of free seats to " << seatsFree << endl;
	//signal to patient that dentist is ready to consult
	cout << "Dentist ready to consult." << endl;
	sem_post(&dentistReady);
	//uunlock seat count write access because patient has been seen.
	cout << "Dentist releasing seatCountWriteAccess." << endl;
	sem_post(&seatCountWriteAccess);
	cout << "Dentist consulting patient." << endl;
	}
}

void *PatientTask(void*ID){
int thisID = * (int*) ID;
while(true){
	//lock the mutex for seat count
	cout << "Patient" << thisID << " trying to acquire seatCountWriteAccess." << endl;
	sem_wait(&seatCountWriteAccess);
	//if there is a seat available, sit down.  Decrement free seats by 1.
	if(seatsFree>0){
		seatsFree--;
		cout << "Patient " << thisID << " seated. Remaining chairs : " << seatsFree << endl;
		//signal that the patient is ready
		cout << "Patient " << thisID << " is notifying Dentist that patient is ready..." << endl;
		sem_post(&patientsReady);

		//unlock seats
		cout << "Patient " << thisID << " releasing seatCountWriteAccess." << endl; 	
		sem_post(&seatCountWriteAccess);
		//lock the dentist
		cout << "Patient " << thisID << " waiting for Dentist..." << endl;
		sem_wait(&dentistReady);
		cout << "Patient " << thisID << " is conslting Dentist..." << endl;
	}
	else{
		//unlock seats for writing
		cout << "Patient " << thisID << " leaving without consulting. No chairs available..." << endl;
		sem_post(&seatCountWriteAccess);
	}
}
}

