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
int contadorPararProductora = 0;
int contadorPararConsumidora = 0;

sem_t vacio;
sem_t lleno;
sem_t semaforoEntreHebras;

int entrada, salida, contador;


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

void printBufferNormal(){
    cout << "**** PRINT DEL BUFFER ****" << endl;
    for(int i = 0;  i < dim_buffer; i++){
        cout << buffer[i] << " ";
    }
    
    cout << endl;
}


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

void* funcionHebraConsumidora( void *ptr ){

    int *numero_hebra;
    numero_hebra = (int *) ptr;
    
    sem_wait( &lleno );
    sem_wait( &semaforoEntreHebras );
    buffer[salida] = 0;
    printf("HebraConsumidora(%d) consume en la posicion %d \n", *numero_hebra, salida);
    printBufferConsumidora(salida);
    salida = (salida + 1) % dim_buffer;
    contador = contador - 1;
    
    
    sem_post( &semaforoEntreHebras );
    sem_post ( &vacio );
    
    return 0;
}

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

void* funcionHebraConsumidoraNoTermina( void *ptr ){
    
    int *numero_hebra;
    numero_hebra = (int *) ptr;
    
    int contadorEstoQueEs1 = 0;
    
    while(1){
        
        sem_wait( &lleno );
        sem_wait( &semaforoEntreHebras );
   
        salida = (salida + 1) % dim_buffer;
        contador = contador - 1;
        
        buffer[salida] = 0;
        printf("HebraConsumidora(%d) consume en la posicion %d \n", *numero_hebra, salida);
        printBufferConsumidora(salida);
        
        sem_post( &semaforoEntreHebras );
        sem_post ( &vacio );
        
    }
    
    return NULL;
}


int main(int argc, char* argv[]){

    dim_buffer = atoi(argv[1]);
    numHebrasProductoras = atoi(argv[2]);
    terminanHebrasProductoras = atoi(argv[3]);
    numHebrasConsumidoras = atoi(argv[4]);
    terminanHebrasConsumidoras = atoi(argv[5]);
    
    contadorPararConsumidora = numHebrasProductoras;
    contadorPararProductora = numHebrasConsumidoras;
   
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
    
    pthread_t id_hebra_productora[numHebrasProductoras];
    pthread_t id_hebra_consumidora[numHebrasConsumidoras];
    
    sem_init( &semaforoEntreHebras, 0, 1);
    sem_init( &vacio, 0, dim_buffer);
    sem_init( &lleno, 0, 0);
    
    entrada = 0;
    salida = 0;
    contador = 0;
    
    printBufferNormal();
    
    for( unsigned i = 0 ; i < numHebrasConsumidoras ; i++ ){
        if(terminanHebrasConsumidoras == 1){
            pthread_create( &(id_hebra_consumidora[i]), NULL, funcionHebraConsumidora, (void *) &idConsumidoras[i]);
        } else  if (terminanHebrasConsumidoras == 0){
            pthread_create( &(id_hebra_consumidora[i]), NULL, funcionHebraConsumidoraNoTermina, (void *) &idConsumidoras[i]);
        }
    }
    
    if(numHebrasProductoras > 0){
        for( unsigned i = 0 ; i < numHebrasProductoras ; i++ ){
            if(terminanHebrasProductoras == 1){
                pthread_create( &(id_hebra_productora[i]), NULL, funcionHebraProductora, (void *) &idProductoras[i]);
            } else if(terminanHebrasProductoras == 0) {
                pthread_create( &(id_hebra_productora[i]), NULL, funcionHebraProductoraNoTermina, (void *) &idProductoras[i]);
            }
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
