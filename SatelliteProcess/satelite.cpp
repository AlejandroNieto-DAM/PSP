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

//Globales para poder utilizarlos en los manejadores.
int fd1[2], fd2[2];
pid_t pidHijo, pidPadre, pidHijoHijo;
char nuevaCadena[MAX_BUF] = "";



/**
 * @brief Calcula la suma total de los datos recibidos en una cadena de string separados por espacios.
 * @param String cadenaTotalDeNumeros en la cual se encuentran los numeros introducidos en el paquete separados por espacios.
 * @pre El proceso hijo mandará una señal al nieto para calcularlo.
 * @return int sumaTotal que es el resultado de sumar los numeros una vez convertidos a int de la cadena.
 * @post El nieto mandara una señal al hijo para que lea el pipe y recoja los datos.
 */
int calcularSumaTotal(string cadenaTotalDeNumeros){
    //cout << "calcularSuma" << endl;
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
    return sumaTotal;
}


/**
 * @brief Hace de intermediario entre el proceso padre y el nieto mandando la informacion recibida del padre al nieto y recogiendo lo que escriba el nieto para mandarlo al padre.
 * @param int segnal que es la señal recibida del padre.
 * @pre El proceso hijo debe recibir una señal del padre.
 * @pre El padre debe de haber escrito en el pipe la informacion a recoger.
 * @return
 * @post El hijo recogerá lo que escriba el nieto en el pipe y lo escribira para que pueda recogerlo el padre.
 */
void gestion_hijo_padre( int segnal){
    //printf("Hijo recibe seÒal del padre..%d\n", segnal);
    close(fd1[1]);
    read(fd1[0], nuevaCadena, MAX_BUF);

    close(fd2[0]);
    write(fd2[1], nuevaCadena, sizeof(nuevaCadena));
    
    kill(pidHijoHijo, SIGUSR2);
    
    int numeroRead = 0;
    close(fd1[1]);
    read(fd1[0], &numeroRead, sizeof(numeroRead));
    
    close(fd2[0]);
    write(fd2[1], &numeroRead, sizeof(numeroRead));
    
    kill(pidPadre, SIGUSR1);
}


/**
 * @brief Lee la informacion escrita por el proceso hijo en el pipe y llama a la funcion calcularSumaTotal que le devolverá un valor int el cual escribirá en el pipe para que sea leido por el proceso hijo.
 * @param int segnal que es la señal recibida del hijo.
 * @pre El proceso nieto debe recibir una señal del hijo.
 * @pre El hijo debe de haber escrito en el pipe la informacion a recoger.
 * @return
 * @post Escribirá en el pipe el valor recibido de la funcion.
 */
void gestion_hijo_hijo( int segnal ){
    //printf("Hijo_hijo recibe seÒal..%d\n", segnal);
    close(fd2[1]);
    read(fd2[0], nuevaCadena, MAX_BUF);
    
    int suma = calcularSumaTotal(nuevaCadena);
    
    close(fd1[0]);
    write(fd1[1], &suma, sizeof(suma));
}


/**
 * @brief
 * @param int segnal que es la señal recibida del padre.
 * @pre El proceso padre debe recibir una señal del hijo.
 * @return
 * @post.
 */
void gestion_padre( int segnal ){
    //printf("Padre recibe la señal..%d\n", segnal);
}


/**
 * @brief Es el cuerpo del proceso padre. Se introduce un numero y dependiendo de una condicion hará una cosa u otra hasta que se introduzca un -1 que hará que el proceso padre llame al hijo.
 * @param string cadenaRepresentativa --> numeros introducidos hasta el -1.
 * @param string totalPaquetes --> valor de todos los paquetes introducidos.
 * @param int sumaTotalPaquetes --> es la suma de todos los numeros de todos los paquetes introducidos.
 * @pre
 * @return
 * @post Dependiendo del paquete mandará una señal al hijo y este le devolverá la suma de los numeros del paquete introducido hasta el -1.
 */
void cuerpoPadre(string &cadenaRepresentativa, string &cadenaNumero, int &sumaTotalPaquetes, string &totalPaquetes){
    //cout << "Entro en el padre!! " << endl;
    
    system("clear");
    system("clear");
    
    cout << KGRN << totalPaquetes << cadenaRepresentativa << RST;
    
    //TODO cambiar intro por un espacio
    int numero = 0;
    cin >> numero;

    if(numero == -1){
        
        totalPaquetes += cadenaRepresentativa + "-1 ";
        close(fd1[0]);
        write(fd1[1], &cadenaRepresentativa, sizeof(cadenaRepresentativa));
        kill(pidHijo, SIGUSR1);
        pause();
        
        int numeroRead = 0;
        close(fd2[1]);
        read(fd2[0], &numeroRead, sizeof(numeroRead));
        
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
    
    string cadenaRepresentativa = "";
    int sumaTotalPaquetes = 0;
    string totalPaquetes = "";
    
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
            while(1){cuerpoPadre(cadenaRepresentativa, sumaTotalPaquetes, totalPaquetes);};
            break;
    }
}
