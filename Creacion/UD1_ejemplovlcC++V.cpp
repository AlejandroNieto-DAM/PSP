#include <iostream>
#include <string>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <cstdlib>


using namespace std;

void ejecutarPrograma(string PATH_PROGRAMA, string PATH_ARCHIVO) {
    pid_t pid = fork(), w;
	int status;
    
    if (pid == -1){
        cerr << "No hay memoria..." << endl;
       	exit(EXIT_FAILURE);
    } else if (pid == 0) {
    	cout << "Child process: My process id = " << getpid() << endl;
    	cout << "Child process: Value returned by fork() = " << pid << endl;
       	execl(PATH_PROGRAMA.c_str(), PATH_PROGRAMA.c_str(), PATH_ARCHIVO.c_str(), (char *) 0);
    } 

    do {
        w = waitpid(pid, &status, WUNTRACED | WCONTINUED);
        if (w == -1)  {
            exit(EXIT_FAILURE);
        }

        if (WIFEXITED(status)) {
            cout << "exited, status:" << WEXITSTATUS(status) << endl;
        } else if (WIFSIGNALED(status)) {
            cout << "killed by signal" << WTERMSIG(status) << endl;
        } else if (WIFSTOPPED(status)) {
        	cout << "stopped by signal" << WSTOPSIG(status) << endl;
        } else if (WIFCONTINUED(status)) {
            cout << "continua ..." << endl;
        }
    } while (!WIFEXITED(status) && !WIFSIGNALED(status));
}

int main(){
	string PATH_PROGRAMA = "/usr/bin/vlc";
    string PATH_ARCHIVO = "/home/tarspace/Escritorio/test.ogg";

    ejecutarPrograma(PATH_PROGRAMA, PATH_ARCHIVO);

    // CONTINUACIÓN PROGRAMA ...
    for (int i = 0; i < 10; i++) {
    	cout << "HOLA CLASE ... !! " << endl;
    }

    return EXIT_SUCCESS;
 }

