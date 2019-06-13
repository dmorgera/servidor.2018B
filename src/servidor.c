/* 
 * servidor: servidor pseudo FTP. 
 * 
 * Copyright (c) 2013,2018 DATSI. Francisco Rosales <frosal@fi.upm.es> 
 * Todos los derechos reservados. 
 * 
 * Publicado bajo Licencia de Proyecto Educativo Práctico 
 * <http://laurel.datsi.fi.upm.es/~ssoo/LICENCIA/LPEP> 
 * 
 * Queda prohibida la difusión total o parcial por cualquier 
 * medio del material entregado al alumno para la realización  
 * de este proyecto o de cualquier material derivado de este,  
 * incluyendo la solución particular que desarrolle el alumno. 
 */ 

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "message.h"

void Escribir_Puerto (int puerto);

/* FUNCION MAIN DEL PROGRAMA SERVIDOR */
int main(int argc,char* argv[])
{
   /* El esqueleto de la funcion principal muestra, de forma esquematica la secuencia 
   de operaciones y la correspondiente traza que se sugiere */

   /* Creacion del socket UDP */
   fprintf(stdout,"SERVIDOR: Creacion del socket UDP: OK|ERROR\n");

   /* Asignacion de la direccion local (del servidor) Puerto UDP*/
   fprintf(stdout,"SERVIDOR: Asignacion del puerto servidor: OK|ERROR\n");

   /* Escribimos el puerto de servicio */
   Escribir_Puerto(0 /* Numero de puerto asignado */);

   /* Creacion del socket TCP de servicio */
   fprintf(stdout,"SERVIDOR: Creacion del socket TCP: OK|ERROR\n");

   /* Asignacion de la direccion local (del servidor) Puerto TCP*/
   fprintf(stdout,"SERVIDOR: Asignacion del puerto servidor: OK|ERROR\n");

   /* Aceptamos conexiones por el socket */
   fprintf(stdout,"SERVIDOR: Aceptacion de peticiones: OK|ERROR\n");

   /* Puerto TCP ya disponible */
   fprintf(stdout,"SERVIDOR: Puerto TCP reservado: OK|ERROR\n");

   while(1 /* Bucle de procesar peticiones */)
   {
      fprintf(stdout,"SERVIDOR: Esperando mensaje.\n");
   
      /* Recibo mensaje */
      fprintf(stdout,"SERVIDOR: Mensaje del cliente: OK|ERROR\n");
   
      if(1/* Mensaje QUIT*/)
           fprintf(stdout,"SERVIDOR: QUIT\n");
      else
      {
           fprintf(stdout,"SERVIDOR: REQUEST(%s,%s)\n", "local","remoto");
   
           /* Envio del resultado */
           fprintf(stdout,"SERVIDOR: Enviando del resultado [OK|ERROR]: OK|ERROR\n");
           /* Esperamos la llegada de una conexion */
           fprintf(stdout,"SERVIDOR: Llegada de un mensaje: OK|ERROR\n");
      }
   }
   
   fprintf(stdout,"SERVIDOR: Finalizado\n");
   exit(0);
}

/* Funcion auxiliar que escribe un numero de puerto en el fichero */
void Escribir_Puerto (int puerto)
{
   int fd;
   if((fd=creat(FICHERO_PUERTO,0660))>=0)
   {
      write(fd,&puerto,sizeof(int));
      close(fd);
      fprintf(stdout,"SERVIDOR: Puerto guardado en fichero %s: OK\n",FICHERO_PUERTO);
   }
}



