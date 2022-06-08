/* 
* Proyecto: Modificar Shell
* Fecha: 08/06/2022 
* Autor: John Sanabria
*   john.sanabria@correounivalle.edu.co
*
* Explicación programa: El shell gestiona la *interacción entre el usuario y el sistema operativo 
* solicitándole la entrada, interpretando dicha entrada *para el sistema operativo y gestionando cualquier
* resultado de salida procedente del sistema operativo. *Los shells ofrecen un método para comunicarse con 
 el *sistema operativo. En este programa agregamos *funcionalidades a la shell creada, para detener *procesos en background y listar dichos procesos que están corriendo en el background.
*
* Modificado por: 
* Joshua Sebastian Chicame Muñoz 
*   joshua.chicame@correounivalle.edu.co
* Adolfo Leon Maya Garcia
*   adolfo.maya@correounivalle.edu.co
* Damian Alessandro Espinosa Espinosa
*   damian.espinosa@correounivalle.edu.co
*/

#include <assert.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

#include "leercadena.h"

int main(int argc, char* argv[]) 
{
  pid_t procces_pid[1000];
  int counter = 0;
  
  char command[256];
  char **comando;

  int len;
  while (1) 
  {
    //int len;
    printf("prompt> ");
    leer_de_teclado(256, command);
    if (strcmp(command,"salir") == 0) break;
    comando = de_cadena_a_vector(command);
    
    //Recorre el vector ingresado y aumenta el contador
    len = 0;
    while(comando[len])
    {
      len++;
    } 
    //si la cantidad de comando es cero entonces devuelve al inicio del while
    if(len==0)
    {
      continue;
    }

    /*
    si el comando ingresado es de detener, primero borra el comando de la lista que contiene los procesos 
    de fondo y manda el comando "kill -9" <pid> con el identificador de proceso para que termine el proceso
    */
    if (strcmp(comando[0],"detener") == 0)
    {
      /*
      comando[0] = comando que ingreso el usurio
      comando[1] = pid en formato char que ingreso el usuario
      si comando[1]== "detener" se borrar el pid del array proccess_pid
      */
      if(len == 2)
      {
        int numero;
        for(int position =0; position < counter; position++)
        {
          numero = strtol(comando[1],NULL,10);
          if(numero == procces_pid[position]) 
          {
            for(int eliminate = position;eliminate<counter;eliminate++) 
            {
              procces_pid[eliminate]= procces_pid[eliminate+1];
            }
            counter--;
          }
        }
        /*
        si el primer string del array es  igual a "detener"
        reemplaza deterner con "kill -9 y lo concadena con el identificador 
        del proceso que va a detener"
        */
        char text[50] = "kill -9 ";  
        comando = de_cadena_a_vector(strcat(text,comando[1]));
      }    
    }
    
   /*
   lista el identificador de los comandos que van a correr 
   en background, recorriendo el array procces_pidque guarda los pid
   */
    if (strcmp(comando[0],"tareas") == 0)
    {
      if(counter <= 0)
      {
        printf("No hay tareas\n");
      }
      else 
      {
        printf("PID\n");
        for(int j= 0; j < counter; j++)
        {
          printf("%d\n",procces_pid[j]);
        }  
      }   
    }

		//Manejo de procesos en background 
    int rc = fork();
    assert(rc >= 0);
  
    if (rc == 0) //child proccess
    {
      if(len>0)
      {
        if(strcmp(comando[len-1], "&")== 0)
        {
          comando[len-1] = '\0'; 
        }
      }
      if(execvp(comando[0], comando) < 0)
      {
        if(strcmp(comando[0],"tareas") != 0)
        {
          fprintf(stderr, "Command not found\n");
          exit(1);
        }
      }
      exit(0);
    } 
    else //parent proccess
    {
         if(len==0)
         {
           printf("len 0\n");
           wait(NULL);
           continue;
         }    
         else if(strcmp(comando[len-1], "&") != 0)
         {
           wait(NULL); 
           continue;
         } 
         else 
         {
           procces_pid[counter++]=rc;
           counter++;
           printf("[%d] %d\n",counter,rc);
           continue;
         }
    } 
  
  }
  return 0;
}