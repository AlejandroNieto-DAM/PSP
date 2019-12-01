/* 
 * File:   main.c
 * Author: neozizou
 *
 * Created on 16 de octubre de 2012, 13:24
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

/*VARIABLES Y PROCEDIMIENTOS GLOBALES (Compartidos por las Hebras)*/
//int i = 5; /*VALOR INICIAL A 5 PARA QUE COMPITAN REALMENTE*/

int i = 5;

void *incrementa( void *ptr ){
     int i = 0;
     int *numero_hebra;
     numero_hebra = (int *) ptr;
     int num_aleatorio;
     
     while (i < 10){
        i++; 
        printf("HebraInc(%d) incrementa el valor a %d \n", *numero_hebra, i);
        num_aleatorio = (rand()%(11-1))/5; //genero un número aleatorio para establecer los segundos a dormir
        sleep(num_aleatorio); //dormimos la hebra para que se produzca cambio de contexto ligero entre hebras
     }   
     
     printf ("La HebraInc(%d) llegó a la meta!! \n", numero_hebra);
     
}

void *decrementa( void *ptr ){
     int *numero_hebra;
     numero_hebra = (int *) ptr;
     int num_aleatorio;
     int i = 10;
     
     while (i > 0){
        i--; 
 		printf("HebraDec(%d) decrementa el valor a %d \n", *numero_hebra, i);
        num_aleatorio = (rand()%(11-1))/5;
        sleep(num_aleatorio);//dormimos la hebra para que se produzca cambio de contexto ligero entre hebras
     }   
     
     printf ("La HebraDec(%d) llegó a la meta!! \n", numero_hebra);
     
}

int main(){
     
     //Inicialización de Semilla para los números aleatorios
     srand(time(NULL));

    /* Create independent threads each of which will execute function */
	int num_hebras = 5;
	int j = 0;
	pthread_t id_hebra_inc[num_hebras] ; //vector de punteros a hebras incrementadoras
	pthread_t id_hebra_dec[num_hebras] ; //vector de punteros a hebras decrementadoras
	
	//Crear las hebras incrementadoras
	for( j = 0 ; j < num_hebras ; j++ )
		pthread_create( &(id_hebra_inc[j]), NULL, incrementa, (void *) &j );
	
	//Crear las hebras decrementadoras
	for(   j = 0 ; j < num_hebras ; j++ )
		pthread_create( &(id_hebra_dec[j]), NULL, decrementa, (void *) &j );	

    //Los join de las hebras incrementadoras
	int iret;
	for(j = 0 ; j < num_hebras ; j++ ) { 
		iret = pthread_join( id_hebra_inc[j], NULL );
		printf("HebraInc(%d) devuelve: %d y libera los recursos correctamente\n", j, iret);	
	}
	
	//Los join de las hebras decrementadoras
	for( j = 0 ; j < num_hebras ; j++ ) { 
		iret = pthread_join( id_hebra_dec[j], NULL );
		printf("HebraDec(%d) devuelve: %d y libera los recursos correctamente\n", j, iret);	
	}
		 
    exit(0); //FIN DEL PROCESO
}


