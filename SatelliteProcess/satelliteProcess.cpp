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


using namespace std;

void manejadorMal( int segnal )
{
    printf("Paquete mal entregao...%d\n", segnal);
}

void manejadorBien( int segnal )
{
    printf("Final de envio de paquetes...%d\n", segnal);
}



int main(){
    int fd2[2];
    int fd[2];
    char buffer[80];
    pid_t pidHijo, compruebaNum;
    
    bool bien = false;
    bool mal = false;
    int numero = 0;
    int suma = 0;
    int totalPaquetes = 0;
    
    char saludoFunciona[]=  "Funsiona!...\0";
    
    
    pidHijo = fork();
    switch(pidHijo){
        case -1:
            cerr << "Failed to fork" << endl;
            exit(1);
        case 0:
    
            compruebaNum = fork();
            
            switch (compruebaNum) {
                case -1:
                    cerr << "Failed to fork" << endl;
                    exit(1);
                    
                case 0:
                    
                    signal(SIGUSR1, manejadorMal);
                    signal(SIGUSR2, manejadorBien);
                    while(1){};
                    break;
                    
                default:
                    
                    while(bien == false && mal == false){
                        
                        cin >> numero;
                        
                        
                        if(numero >= 0){
                            
                            suma += numero;
                            totalPaquetes = (totalPaquetes*10) + numero;
                            
                        } else if (numero < 0){
                           
                            if(numero == -1){
                                
                                bien = true;
                                kill(compruebaNum, SIGUSR2);
                                sleep(1);
                                cout << KGRN << totalPaquetes << RST << endl;
                                //cout << KGRN << "Valor total de todos los paquetes --> " << suma << RST << endl;
                                write(fd[1], &suma, sizeof(suma));
                                
                            } else {
                                
                                kill(compruebaNum, SIGUSR1);
                                sleep(1);
                                mal = true;
                                cout << KRED << totalPaquetes << numero << RST << endl;
                                
                            }
                        }
                    }
                    
                    
                     //escribo en pipe
                    kill(compruebaNum, SIGTERM);
                    break;
            }
            
            
        default:
            
            //pidHijo = wait(NULL);
            
            close(fd[1]);//cierra el descriptor de salida
            wait(NULL);
            read(fd[0], &suma, sizeof(suma));//leo el pipe
            //cout << "Las suma --> ";
            cout << KGRN << "Valor total de todos los paquetes --> " << suma << RST << endl;
            
            kill(pidHijo, SIGTERM);
            //cout << "Padre finalizaoooo!" << endl;
        
            break;
    }
                  
    
}
                  
                  

