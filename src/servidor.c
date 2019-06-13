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
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include "message.h"

void Escribir_Puerto (int puerto);

/* FUNCION MAIN DEL PROGRAMA SERVIDOR */
int main(int argc,char* argv[]) {

   /* El esqueleto de la funcion principal muestra, de forma esquematica la secuencia 
   de operaciones y la correspondiente traza que se sugiere */
   int puerto;
   int socket_UDP, socket_TCP;
   int fd, socket_fd;
   int leidos = 0;
   struct sockaddr_in dirS, dirC; 
   UDP_Msg msj;
   char buf[4096];
   char* res;
   socklen_t sizeDirS = sizeof(dirS);
   socklen_t sizeDirC = sizeof(dirC);

   /* Creacion del socket UDP */
   if ((socket_UDP = socket(AF_INET, SOCK_DGRAM, 0)) == 0) { 
      fprintf(stderr,"SERVIDOR: Creacion del socket UDP: ERROR\n"); 
      exit(1); 
   } else 
      fprintf(stderr,"SERVIDOR: Creacion del socket UDP: OK\n");

   /* Asignacion de la direccion local (del servidor) Puerto UDP*/
   bzero(&dirS, sizeof(dirS));
   dirS.sin_family = AF_INET; 
   dirS.sin_addr.s_addr = inet_addr(HOST_SERVIDOR); 
   dirS.sin_port = htons(0); 
       
   if (bind(socket_UDP, (struct sockaddr *) &dirS, sizeof(dirS)) < 0) { 
      fprintf(stderr,"SERVIDOR: Asignacion del puerto servidor: ERROR\n");
      exit(1); 
   } else
      fprintf(stderr,"SERVIDOR: Asignacion del puerto servidor: OK\n");

   /* Escribimos el puerto de servicio */
   bzero(&dirS, sizeof(dirS));
   getsockname(socket_UDP, (struct sockaddr *) &dirS, &sizeDirS);
   puerto = htons(dirS.sin_port);
   Escribir_Puerto(puerto);

   /* Creacion del socket TCP de servicio */
   if ((socket_TCP = socket(AF_INET, SOCK_STREAM, 0)) == 0) { 
      fprintf(stderr,"SERVIDOR: Creacion del socket TCP: ERROR\n"); 
      exit(1); 
   } else 
      fprintf(stderr,"SERVIDOR: Creacion del socket TCP: OK\n");

   /* Asignacion de la direccion local (del servidor) Puerto TCP*/
   bzero(&dirS, sizeof(dirS));
   dirS.sin_family = AF_INET; 
   dirS.sin_addr.s_addr = inet_addr(HOST_SERVIDOR);
   dirS.sin_port = htons(0); 
       
   if (bind(socket_TCP, (struct sockaddr *) &dirS, sizeof(dirS)) < 0) { 
      fprintf(stderr,"SERVIDOR: Asignacion del puerto servidor: ERROR\n");
      exit(1); 
   } else
      fprintf(stderr,"SERVIDOR: Asignacion del puerto servidor: OK\n");

   /* Escribimos el puerto de servicio */
   bzero(&dirS, sizeof(dirS));
   getsockname(socket_TCP, (struct sockaddr *) &dirS, &sizeDirS);
   puerto = htons(dirS.sin_port);

   /* Aceptamos conexiones por el socket */
   if(listen(socket_TCP, 20) < 0){
      fprintf(stderr,"SERVIDOR: Aceptacion de peticiones: ERROR\n");
      exit(1);
   } else
      fprintf(stderr,"SERVIDOR: Aceptacion de peticiones: OK\n");

   /* Puerto TCP ya disponible */
   fprintf(stderr,"SERVIDOR: Puerto TCP reservado: OK\n");

   while(1 /* Bucle de procesar peticiones */) {
      fprintf(stderr,"SERVIDOR: Esperando mensaje.\n");
      res = "OK";
      bzero(&dirC, sizeof(dirC));
      bzero(&msj, sizeof(msj));
      if(recvfrom(socket_UDP, (char *) &msj, sizeof(msj), 0, (struct sockaddr *) &dirC, &sizeDirC) < 0){
         fprintf(stderr,"SERVIDOR: mensaje del cliente: ERROR\n");
         exit(1);
      }
      /* Recibo msj */
      fprintf(stderr,"SERVIDOR: mensaje del cliente: OK\n");

      if(ntohl(msj.op) == QUIT){
         fprintf(stderr,"SERVIDOR: QUIT\n");
         msj.op = htonl(OK);
         if(sendto(socket_UDP, (char *) &msj, sizeof(msj), 0, (struct sockaddr *) &dirC, sizeof(dirC)) < 0){
            fprintf(stderr,"SERVIDOR: Enviando del resultado [OK]: ERROR\n");
            exit(1);
         } else {
            fprintf(stderr,"SERVIDOR: Enviando del resultado [OK]: OK\n");
            break;
         }
      }
      else {
         fprintf(stderr,"SERVIDOR: REQUEST(%s,%s)\n", msj.local, msj.remoto);
         
         if((fd = open(msj.remoto, O_RDONLY)) < 0){
            msj.op = htonl(ERROR);
            res = "ERROR";
         } else {
            msj.op = htonl(OK);
            msj.puerto = htons((uint16_t) puerto);
         }
         /* Envio del resultado */
         if(sendto(socket_UDP, (char *) &msj, sizeof(msj), 0, (struct sockaddr *) &dirC, sizeof(dirC)) < 0){
            fprintf(stderr,"SERVIDOR: Enviando del resultado [%s]: ERROR\n", res);
            exit(1);
         } else 
            fprintf(stderr,"SERVIDOR: Enviando del resultado [%s]: OK\n", res);
      }
      /* Esperamos la llegada de una conexion */
      if(strcmp(res, "OK") == 0){
         if((socket_fd = accept(socket_TCP, (struct sockaddr *) &dirC, &sizeDirC)) < 0) {
            fprintf(stderr,"SERVIDOR: Llegada de un mensaje: ERROR\n");
         } else {
            fprintf(stderr,"SERVIDOR: Llegada de un mensaje: OK\n");
            while((leidos = read(fd, buf, sizeof(buf))) > 0){
               write(socket_fd, buf, leidos);
            }
            close(socket_fd);
         }
         close(fd);
      }
   }
   fprintf(stderr,"SERVIDOR: Finalizado\n");
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
      fprintf(stderr,"SERVIDOR: Puerto guardado en fichero %s: OK\n",FICHERO_PUERTO);
   }
}



