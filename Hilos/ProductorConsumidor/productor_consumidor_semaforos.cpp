//
//  productor_consumidor.cpp
//  
//
//  Created by Alejandro Nieto Alarcon on 06/12/2019.
//
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <iostream>
#include <string>
#include <semaphore.h>
#include <vector>

using namespace std;

#define RESET_COLOR    "\x1b[0m"
#define NEGRO_T        "\x1b[30m"
#define NEGRO_F        "\x1b[40m"
#define ROJO_T     "\x1b[31m"
#define ROJO_F     "\x1b[41m"
#define VERDE_T        "\x1b[32m"
#define VERDE_F        "\x1b[42m"
#define AMARILLO_T "\x1b[33m"
#define    AMARILLO_F  "\x1b[43m"
#define AZUL_T     "\x1b[34m"
#define    AZUL_F      "\x1b[44m"
#define MAGENTA_T  "\x1b[35m"
#define MAGENTA_F  "\x1b[45m"
#define CYAN_T     "\x1b[36m"
#define CYAN_F     "\x1b[46m"
#define BLANCO_T   "\x1b[37m"
#define BLANCO_F   "\x1b[47m"

int * buffer;
int dim_buffer;

int numHebrasProductoras = 0;
int terminanHebrasProductoras = 1; //1 si terminan 0 si no
int numHebrasConsumidoras = 0;
int terminanHebrasConsumidoras = 1; //1 si terminan 0 si no


//Declaracion semáforos.
sem_t vacio; //Hueco vacio.
sem_t lleno; //Hueco lleno.
sem_t semaforoEntreHebras; //Semaforo exclusion mutua.

int entrada, salida;

/**
 * @brief Esta funcion hace un print de cada posición del buffer, mostrando la celda cambiada de un color verde.
 * @param int posicionCambiada que es la posición que imprimiremos de forma diferente para ver el cambio.
 * @pre dim_buffer > 0
 * @return no devuelve nada.
 * @post
 */

void printBufferConsumidora(int posicionCambiada){
    cout << "**** PRINT DEL BUFFER ****" << endl;
    for(int i = 0;  i < dim_buffer; i++){
        if(i == posicionCambiada){
            cout << VERDE_T << buffer[i] << RESET_COLOR << " ";
        } else {
            cout << buffer[i] << " ";
        }
        
    }
    
    cout << endl;
}


/**
 * @brief Esta funcion hace un print de cada posición del buffer, mostrando la celda cambiada de un color rojo.
 * @param int posicionCambiada que es la posición que imprimiremos de forma diferente para ver el cambio.
 * @pre dim_buffer > 0
 * @return no devuelve nada.
 * @post
 */

void printBufferProductora(int posicionCambiada){
    cout << "**** PRINT DEL BUFFER ****" << endl;
    for(int i = 0;  i < dim_buffer; i++){
        if(i == posicionCambiada){
            cout << ROJO_T << buffer[i] << RESET_COLOR << " ";
        } else {
            cout << buffer[i] << " ";
        }
        
    }
    
    cout << endl;
}


/**
 * @brief Esta funcion hace un print de cada posición del buffer.
 * @param
 * @pre dim_buffer > 0
 * @return no devuelve nada.
 * @post
 */

void printBufferNormal(){
    cout << "**** PRINT DEL BUFFER ****" << endl;
    for(int i = 0;  i < dim_buffer; i++){
        cout << buffer[i] << " ";
    }
    
    cout << endl;
}

/**
 * @brief Una hebra productora espera a un hueco vacio en el buffer para empezar a producir.
 * @param void *ptr es el puntero que nos dará el id de la hebra para poder identificarla.
 * @pre En el main deberá crearse al menos una hebra productora.
 * @return no devuelve nada.
 * @post un hueco vacío del buffer quedará lleno.
 */

void* funcionHebraProductora( void *ptr ){

    int *numero_hebra;
    numero_hebra = (int *) ptr;
    
    sem_wait( &vacio );
    sem_wait( &semaforoEntreHebras );
    buffer[entrada] = 1;
    printf("HebraProductora(%d) produce en la posicion %d \n", *numero_hebra, entrada);
    printBufferProductora(entrada);
    entrada = (entrada + 1) % dim_buffer;
    sem_post( &semaforoEntreHebras );
    sem_post ( &lleno );
    
    return 0;

}


/**
 * @brief Una hebra consumidora espera a un hueco lleno en el buffer para empezar a consumir.
 * @param void *ptr es el puntero que nos dará el id de la hebra para poder identificarla.
 * @pre En el main deberá crearse al menos una hebra consumidora.
 * @return no devuelve nada.
 * @post un hueco lleno del buffer quedará vacío.
 */

void* funcionHebraConsumidora( void *ptr ){

    int *numero_hebra;
    numero_hebra = (int *) ptr;
    
    sem_wait( &lleno );
    sem_wait( &semaforoEntreHebras );
    buffer[salida] = 0;
    printf("HebraConsumidora(%d) consume en la posicion %d \n", *numero_hebra, salida);
    printBufferConsumidora(salida);
    salida = (salida + 1) % dim_buffer;
    sem_post( &semaforoEntreHebras );
    sem_post ( &vacio );
    
    return 0;
}


/**
 * @brief Una hebra productora espera a un hueco vacio en el buffer para empezar a producir indefinidamente.
 * @param void *ptr es el puntero que nos dará el id de la hebra para poder identificarla.
 * @pre En el main deberá crearse al menos una hebra productora.
 * @return no devuelve nada.
 * @post un hueco vacío del buffer quedará lleno.
 */

void* funcionHebraProductoraNoTermina( void *ptr ){
    
    int *numero_hebra;
    numero_hebra = (int *) ptr;
    
    while(1){
  
        sem_wait( &vacio );
        sem_wait( &semaforoEntreHebras );
        buffer[entrada] = 1;
        printf("HebraProductora(%d) produce en la posicion %d \n", *numero_hebra, entrada);
        printBufferProductora(entrada);
        entrada = (entrada + 1) % dim_buffer;
        sem_post( &semaforoEntreHebras );
        sem_post ( &lleno );

    }
    
    return NULL;
}


/**
 * @brief Una hebra consumidora espera a un hueco lleno en el buffer para empezar a consumir indefinidamente.
 * @param void *ptr es el puntero que nos dará el id de la hebra para poder identificarla.
 * @pre En el main deberá crearse al menos una hebra consumidora.
 * @return no devuelve nada.
 * @post un hueco lleno del buffer quedará vacío.
 */

void* funcionHebraConsumidoraNoTermina( void *ptr ){
    
    int *numero_hebra;
    numero_hebra = (int *) ptr;
    
    
    while(1){
        
        sem_wait( &lleno );
        sem_wait( &semaforoEntreHebras );
        buffer[salida] = 0;
        printf("HebraConsumidora(%d) consume en la posicion %d \n", *numero_hebra, salida);
        printBufferConsumidora(salida);
        salida = (salida + 1) % dim_buffer;
        sem_post( &semaforoEntreHebras );
        sem_post ( &vacio );
        
    }
    
    return NULL;
}


int main(int argc, char* argv[]){

    //Inicializamos las variables globales con los argumentos que nos pasan por el terminal.
    dim_buffer = atoi(argv[1]);
    numHebrasProductoras = atoi(argv[2]);
    terminanHebrasProductoras = atoi(argv[3]);
    numHebrasConsumidoras = atoi(argv[4]);
    terminanHebrasConsumidoras = atoi(argv[5]);
    
   
    //Declaracion ids hebras productoras.
    int idProductoras[numHebrasProductoras];
    for(int i = 0; i < numHebrasProductoras; i++){
        idProductoras[i] = i;
    }
    
    //Declaracion id hebras consumidoras.
    int idConsumidoras[numHebrasConsumidoras];
    for(int i = 0; i < numHebrasConsumidoras; i++){
        idConsumidoras[i] = i;
    }
    
    //Inicializamos el buffer totalmente vacío.
    buffer = new int [dim_buffer];
    for(int i = 0; i < dim_buffer; i++){
        buffer[i] = 0;
    }
    
    //Creamos un vector de hebras productoras y otro de consumidoras.
    pthread_t id_hebra_productora[numHebrasProductoras];
    pthread_t id_hebra_consumidora[numHebrasConsumidoras];
    
    
    //Inicializamos semáforos.
    sem_init( &semaforoEntreHebras, 0, 1);
    sem_init( &vacio, 0, dim_buffer);
    sem_init( &lleno, 0, 0);
    
    entrada = 0;
    salida = 0;
    
    printBufferNormal();

    if(numHebrasProductoras > 0){
        for( unsigned i = 0 ; i < numHebrasProductoras ; i++ ){
            if(terminanHebrasProductoras == 1){
                pthread_create( &(id_hebra_productora[i]), NULL, funcionHebraProductora, (void *) &idProductoras[i]);
            } else if(terminanHebrasProductoras == 0) {
                pthread_create( &(id_hebra_productora[i]), NULL, funcionHebraProductoraNoTermina, (void *) &idProductoras[i]);
            }
        }
    }
    
    for( unsigned i = 0 ; i < numHebrasConsumidoras ; i++ ){
        if(terminanHebrasConsumidoras == 1){
            pthread_create( &(id_hebra_consumidora[i]), NULL, funcionHebraConsumidora, (void *) &idConsumidoras[i]);
        } else  if (terminanHebrasConsumidoras == 0){
            pthread_create( &(id_hebra_consumidora[i]), NULL, funcionHebraConsumidoraNoTermina, (void *) &idConsumidoras[i]);
        }
    }
    
    int iret;
    
    for( unsigned i = 0 ; i < numHebrasConsumidoras ; i++ ){
        iret = pthread_join( id_hebra_consumidora[i], NULL);
        printf("HebraConsumidora(%d) devuelve: %d y libera los recursos correctamente\n", i, iret);
    }
    
    for( unsigned i = 0 ; i < numHebrasProductoras ; i++ ){
        iret = pthread_join( id_hebra_productora[i], NULL);
        printf("HebraProductora(%d) devuelve: %d y libera los recursos correctamente\n", i, iret);
    }
     
    printBufferNormal();

    sem_destroy( &semaforoEntreHebras );
    sem_destroy( &vacio );
    sem_destroy( &lleno );
    
    delete buffer;
}
