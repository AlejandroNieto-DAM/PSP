/*
* File:   main.c
* Author: neozizou
*
* Created on 16 de octubre de 2012, 13:24
*/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <iostream>

using namespace std;

/*VARIABLES Y PROCEDIMIENTOS GLOBALES (Compartidos por las Hebras)*/
int i = 5; /*VALOR INICIAL A 5 PARA QUE COMPITAN REALMENTE*/
void *incrementa( void *ptr );
void *decrementa( void *ptr );

int main(){
    //Información de las Hebras
    pthread_t thread1, thread2;
    char message1[] = "Thread(1): ";
    char message2[] = "Thread(2): ";
    int  iret1, iret2;
    
    //Inicialización de Semilla para los números aleatorios
    srand(time(NULL));
    
    /* Create independent threads each of which will execute function */
    iret1 = pthread_create( &thread1, NULL, incrementa, (void*) message1);
    iret2 = pthread_create( &thread2, NULL, decrementa, (void*) message2);
    
    
    /* Wait till threads are complete before main continues. Unless we  */
    /* wait we run the risk of executing an exit which will terminate   */
    /* the process and all threads before the threads have completed.   */
    pthread_join( thread1, NULL);
    pthread_join( thread2, NULL);
    
    printf("Thread(1) devuelve: %d y libera los recursos correctamente\n",iret1);
    printf("Thread(2) devuelve: %d y libera los recursos correctamente\n",iret2);
    return 0;
}

void* incrementa( void *ptr ){
    char *message;
    message = (char *) ptr;
    int num_aleatorio;
    
    while (i < 10){
        
        i++;
        printf("%s incrementa el valor a %d \n", message, i);
        num_aleatorio = (rand()%(11-1))/5;
        sleep(num_aleatorio); //dormimos la hebra para que se produzca cambio de contexto ligero entre hebras
    }
    
    //printf ("La %s llegó a la meta!! \n", message);
    cout << "La " << message << " llegó a la meta!!" << endl;
    return 0;
}

void* decrementa( void *ptr ){
    char *message;
    message = (char *) ptr;
    int num_aleatorio;
    
    while (i > 0){
        
        i--;
        printf("%s decrementa el valor a %d \n", message, i);
        num_aleatorio = (rand()%(11-1))/5;
        sleep(num_aleatorio);//dormimos la hebra para que se produzca cambio de contexto ligero entre hebras
    }
    
    printf ("La %s llegó a la meta!! \n", message);
    return 0;
}
