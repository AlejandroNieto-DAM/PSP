//
//  satelite.cpp
//  
//
//  Created by Alejandro Nieto Alarcon on 08/11/2019.
//

#include "satelite.hpp"

#include <iostream>
#include <string>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <cstdlib>
#include <stdlib.h>
#include <stdio.h>
#include <sstream>

#define RST  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"

#define MAX_BUF 1024

using namespace std;

int fd1[2], fd2[2];


pid_t pidHijo, pidPadre, pidHijoHijo;
string cadenaRepresentativa, cadenaNumero, sumaNumero;

int numero = 0;
int suma = 0;
string sumaString = "";
int sumaTotalPaquetes = 0;
string totalPaquetes = "";

char nuevaCadena[MAX_BUF] = "";

int calcularSumaTotal(string cadenaTotalDeNumeros){
    
    //cout << "calcularSima" << endl;
    string cadena = cadenaTotalDeNumeros;
    int sumaTotal = 0;
    string numero;

    for(int i = 1; i < cadena.size(); i++){
        if(cadena[i] == ' '){
            sumaTotal += atoi(numero.c_str());
            numero = "";
        } else {
            numero += cadena[i];
        }
    }
    
    //cout << "Suma total antes de enviar --> " << sumaTotal << endl;
    return sumaTotal;
}

void gestion_hijo_padre( int segnal){
    
    //printf("Hijo recibe seÒal del padre..%d\n", segnal);
    
    close(fd1[1]);
    read(fd1[0], nuevaCadena, MAX_BUF);
    //cout << "Hijo lee el pipe escrito por el padre " << endl;

    close(fd2[0]);
    write(fd2[1], nuevaCadena, sizeof(nuevaCadena));
    //cout << "Hijo escribe en el pipe para el segundo hijo " << endl;
    
    kill(pidHijoHijo, SIGUSR2);
    
    int numeroRead = 0;
    close(fd1[1]);
    read(fd1[0], &numeroRead, sizeof(numeroRead));
    //cout << "Hijo lee el pipe escrito por el hijo hijo " << endl;
    
    //cout << "Pero mira la suma << " << numeroRead << endl;
    
    close(fd2[0]);
    write(fd2[1], &numeroRead, sizeof(numeroRead));
    //cout << "Hijo escribe en el pipe para el padre" << endl;
    
    kill(pidPadre, SIGUSR1);
    
}


void gestion_hijo_hijo( int segnal )
{
    //printf("Hijo_hijo recibe seÒal..%d\n", segnal);
    close(fd2[1]);
    read(fd2[0], nuevaCadena, MAX_BUF);
    //cout << "Hijohijo lee el pipe escrito por el hijo " << endl;
    
    //cout << KBLU << "VALOR DE LA CADENA EN EL HIJOHIJO "  << nuevaCadena << RST << endl;
    
    suma = calcularSumaTotal(nuevaCadena);
    //sumaString = to_string(suma);
    
    //cout << KBLU << "VALOR SUMA "  << sumaString.size() << RST << endl;
    
    close(fd1[0]);
    write(fd1[1], &suma, sizeof(suma));
    
}



void gestion_padre( int segnal )
{
    //printf("Padre recibe la señal..%d\n", segnal);
}


void cuerpoPadre(){
    //cout << "Entro en el padre!! " << endl;
    
    system("clear");
    system("clear");
    //cout << "Numero: ";
    
    cout << KGRN << totalPaquetes << cadenaRepresentativa << RST;
    
    //TODO cambiar intro por un espacio
    cin >> numero;
    
    if(numero == -1){
        
        cadenaNumero  = cadenaRepresentativa;
        totalPaquetes += cadenaNumero + "-1 ";
        //cout << KGRN << "String a mandar:" << cadenaNumero << RST << endl;
        close(fd1[0]);
        write(fd1[1], &cadenaNumero, sizeof(cadenaNumero));
        kill(pidHijo, SIGUSR1);
        pause();
        
        
        int numeroRead = 0;
        close(fd2[1]);
        read(fd2[0], &numeroRead, sizeof(numeroRead));
        

        //cout << "Valor nueva cadena =" << numeroRead << endl;
        
        sumaTotalPaquetes += numeroRead;
        cout << KBLU << "Suma total de los paquetes: " << sumaTotalPaquetes << RST << endl;
        sleep(3);
        
        cadenaRepresentativa = "";
     
    } else if(numero < -1){
        
        cout << KRED << "Paquete erroneo --> " << cadenaRepresentativa << numero << RST << endl;
        sleep(3);
        cadenaRepresentativa = "";
        
    } else {
        cadenaRepresentativa += to_string(numero) + " ";
        
    }

}


int main(){
    
    pipe(fd1);
    pipe(fd2);

    pidPadre = getpid();
    pidHijo = fork();
    
    switch(pidHijo){
        case -1:
            cerr << "Failed to fork" << endl;
            exit(1);
        case 0:
            
            pidHijoHijo = fork();
            
            switch (pidHijoHijo) {
                case -1:
                    cerr << "Failed to fork" << endl;
                    exit(1);
                    break;
                case 0:
   
                    signal(SIGUSR2, gestion_hijo_hijo);
                    //cout << "Hijo hijo" << endl;
                    while(1){};
                    break;
                    
                default:
     
                    signal(SIGUSR1, gestion_hijo_padre);
                    //cout << "Hijo" << endl;
                    while(1){};
                    
                    break;
            }
            
        default:
   
            signal(SIGUSR1, gestion_padre);
            //cout << "Padre" << endl;
            while(1){
                cuerpoPadre();
            };
 
            break;
    }
    
    
}
