//
//  DekkerIntento1.cpp
//  
//
//  Created by Alejandro Nieto Alarcon on 16/11/2019.
//

#include "DekkerIntento1.hpp"
#include <unistd.h>
#include <iostream>

using namespace std;

int globalVariable = 0;
int turno = 0;
int fd[2];


void seccionCritica(int &i){
    cout << "Seccion critica lol" << endl;
    i++;
}

int main(){
    
    pid_t pidHijo;
    pipe(fd);
    pidHijo = fork();
    
    write(fd[1], &turno, sizeof(turno));
    
    switch (pidHijo) {
        case -1:
            exit(-1);
            break;
        case 0:
            
            cout << "nene" << endl;
            
            while(1){
                
                read(fd[0], &turno, sizeof(turno));
                cout << "Turno en hijo" << turno << endl;
                
                while(turno != 0){
                    
                    seccionCritica(globalVariable);
                    cout << "Hola soy el hijo y el numero es: " << globalVariable << endl;
                   
                    turno = 0;
                    write(fd[1], &turno, sizeof(turno));
                    cout << "Escrito hijo " << turno << endl;
                    
                }
                
                sleep(1);
             
            }
                
            
            
            break;
        default:
            
            cout << "padre" << endl;
            
            while(1){

                read(fd[0], &turno, sizeof(turno));
                cout << "Turno en padre" << turno << endl;
                
                while(turno != 1){
                    
                    globalVariable++;
                    cout << "Hola soy el padre y el numero es: " << globalVariable << endl;
                    
                    turno = 1;
                    write(fd[1], &turno, sizeof(turno));
                    cout << "Escrito padre " << turno << endl;
                    
                }
                
                sleep(1);
                
            }
            break;
    }
}
