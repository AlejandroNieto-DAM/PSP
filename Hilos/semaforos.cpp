#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <iostream>
#include <string>
#include <semaphore.h>
#include <vector>

using namespace std ;

sem_t mutex ; // semaforo en memoria compartida

void* proc( void* p ){

	string *message;
	message = (string *) p;

	for(int i = 0; i < 10; i++){
		cout << *message ;
		cout.flush();
		
		int num_aleatorio = (rand()%(11-1))/5;
		sleep(num_aleatorio); //dormimos la hebra para que se produzca cambio de contexto ligero entre hebras
				
	}

	return NULL ;
}

void* proc2( void* p ){

	string *message;
	message = (string *) p;

	sem_wait( &mutex );
	for(int i = 0; i < 10; i++){
		cout << *message ;
		cout.flush();
		int num_aleatorio = (rand()%(11-1))/5;
		sleep(num_aleatorio); //dormimos la hebra para que se produzca cambio de contexto ligero entre hebras
	}
	sem_post( &mutex );

	return NULL ;
}

int main(){

	int num_hebras = 0;

	cout << "¿Cuantos hilos desea? ";
	cin >> num_hebras;

	pthread_t id_hebraA[num_hebras];
	pthread_t id_hebraB[num_hebras];
	pthread_t id_hebraC[num_hebras];

	pthread_t id_hebraA2[num_hebras];
	pthread_t id_hebraB2[num_hebras];
	pthread_t id_hebraC2[num_hebras];

	vector<string> mensajesA(num_hebras);
	vector<string> mensajesB(num_hebras);
	vector<string> mensajesC(num_hebras);

	for(int i=0; i < num_hebras; i++){
		mensajesA[i] = "A";
		mensajesB[i] = "B";
		mensajesC[i] = "C";
	}
	
	
	
	cout << "######SIN SEMAFOROS######" << endl;

	for( unsigned i = 0 ; i < num_hebras ; i++ ){
		pthread_create( &(id_hebraA[i]), NULL, proc, (void *)&mensajesA[i] );
		pthread_create( &(id_hebraB[i]), NULL, proc, (void *)&mensajesB[i] );
		pthread_create( &(id_hebraC[i]), NULL, proc, (void *)&mensajesC[i] );

	}

	for( unsigned i = 0 ; i < num_hebras ; i++ ){
		pthread_join( id_hebraA[i], NULL );
		pthread_join( id_hebraB[i], NULL );
		pthread_join( id_hebraC[i], NULL );
	}
	
	
	
	
	cout << endl << endl << endl << endl;
	cout << "######CON SEMAFOROS######" << endl;

	sem_init( &mutex, 0, 1 );	

	for( unsigned i = 0 ; i < num_hebras ; i++ ){
		pthread_create( &(id_hebraA2[i]), NULL, proc2, (void *)&mensajesA[i] );
		pthread_create( &(id_hebraB2[i]), NULL, proc2, (void *)&mensajesB[i] );
		pthread_create( &(id_hebraC2[i]), NULL, proc2, (void *)&mensajesC[i] );

	}

	for( unsigned i = 0 ; i < num_hebras ; i++ ){
		pthread_join( id_hebraA2[i], NULL );
		pthread_join( id_hebraB2[i], NULL );
		pthread_join( id_hebraC2[i], NULL );
	}

	sem_destroy( &mutex );
	
	cout << endl;
}
