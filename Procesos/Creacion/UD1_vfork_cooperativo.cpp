#include <stdio.h>
#include <iostream>
#include <string>
#include <stdlib.h>

// Required by for routine
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

using namespace std;

int main(){
	
   string sIdentifier;
   int    cooperativeVariable = 0;
   
   pid_t pID = vfork();
   if (pID == 0) {               // child
      // Code only executed by child process
      int i = 0;
      sIdentifier = "Child Process: ";
      while (i < 50){
      	cooperativeVariable++;
      	cout << sIdentifier << " Global variable (HIJO): " << cooperativeVariable << endl;
      	i++;
      	wait((int*) 1);
      }
      cout << sIdentifier << "Papá he terminaooo!" << endl;
      _exit(0);
    }
    else if (pID < 0){            // failed to fork
    
        cerr << "Failed to fork" << endl;
        exit(1);
        // Throw exception
    }
    else {                                   // parent
      // Code only executed by parent process
      sIdentifier = "Parent Process:";
      int i = 0;
      while (i < 50){
      	cooperativeVariable++;
      	cout << sIdentifier << " Global variable (PADRE): " << cooperativeVariable << endl;
      	i++;
      }
    }

    // executed only by parent
	cout << sIdentifier << "Yo también termino." << endl;
    ///cout << sIdentifier;
    //cout << " Global variable: " << globalVariable;
    //cout << " Stack variable: "  << iStackVariable << endl;
    exit(0);
}
                
