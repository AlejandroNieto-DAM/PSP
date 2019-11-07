#include <iostream>
#include <string>

// Required by for routine
#include <sys/types.h>
#include <unistd.h>

using namespace std;
int variable = 0;

int main(){
    
    string sIdentifier;
    
    //todo el que quiera del padre ...
    
    pid_t pID = fork(); //Se crea un proceso hijo clonado del padre (exactamente igual)
    if (pID == 0)   //A partir de aqui empiezan a ejercutarse independientemente los dos procesos aunque que se intercalen las intrucciones depende del S.O.
                    // child
                    //el 0 es una manera de distinguir que instrucciones son del padre y cuales del hijo
    {
        // Code only executed by child process
        int i = 0;
        sIdentifier = "Child Process: ";
        while (i < 10){
            variable++;
            cout << sIdentifier << " Global variable: " << variable << endl;
            i++;
        }
        cout << sIdentifier << "Papá he terminaooo!" << endl;
        _exit(0);
    }
    else if (pID < 0)            // failed to fork //Control de excecpciones
    {
        cerr << "Failed to fork" << endl;
        exit(1);
        // Throw exception
    }
    else                                   // parent
    {
        // Code only executed by parent process
        sIdentifier = "Parent Process:";
        int i = 0;
        while (i < 10){
            variable++;
            cout << sIdentifier << " Global variable: " << variable << endl;
            i++;
        }
    }
    
    // executed only by parent
    int antonio = 0;
    while( antonio < 10000){
        cout << sIdentifier << "Yo también termino." << endl;
        antonio++;
    }
    ///cout << sIdentifier;
    //cout << " Global variable: " << globalVariable;
    //cout << " Stack variable: "  << iStackVariable << endl;
    exit(0);
}

