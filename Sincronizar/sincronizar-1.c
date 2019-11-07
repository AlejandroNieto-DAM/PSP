#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

//Librerias para hacer las se�ales.
#include <fcntl.h>
#include <signal.h>

/*-------------------------------------------*/
/* gesti�n de se�ales en proceso padre       */
void manejador( int segnal )
{
 printf("Hijo recibe senial...%d\n", segnal);
}

void manejador2( int segnal )
{
    printf("Hijo esto esto esto senial...%d\n", segnal);
}
/*-------------------------------------------*/
int main()
{
  pid_t pid_hijo;
  pid_hijo = fork(); //creamos hijo   
  
  switch(pid_hijo)
  {
     case -1:
          printf( "Error al crear el proceso hijo...\n");
          exit( -1 );        
     case 0:   //HIJO
          //printf( "Toy reading...\n");
          signal( SIGUSR1, manejador);
          signal( SIGUSR2, manejador2);//MANEJADOR DE SE�AL EN HIJO
          //printf( "Toy reading...\n");
          while(1){};
     break;    
     default: //PADRE envia 2 se�ales
          
          sleep(1);
          kill(pid_hijo, SIGUSR1);//ENVIA SE�AL AL HIJO
          sleep(1);
          kill(pid_hijo, SIGUSR2);//ENVIA SE�aL AL HIJO
          sleep(1);
     break;
  } 
  return 0;
}
