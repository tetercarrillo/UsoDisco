#include "directorio.h"
#include "cola.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <pwd.h>
#include <grp.h>
#include <errno.h>
#include <sys/dir.h>
#include <sys/param.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <signal.h>
#include <dirent.h>
#include <time.h>
#include <fcntl.h>




char* ManejoDirectorios(char* raiz){
    // Variable para abrir el directorio
    DIR* dir;
    // Variable para leer el directorio
    struct dirent* dp;

    int tamano_raiz, tamano_archivo,tamano_total, tamano_informacion,tamano_bloques,nro_enlaces;
    char* respuesta = (char*) malloc(sizeof(char)*5000);//!nuevo
    struct stat statbuf;
    unsigned char tipo;
    char* informacion_directorio;



    // Se abre el directorio y se hace la verificación pertinente
    nro_enlaces = 0;
    dir = opendir(raiz);
    if (dir == NULL){
        printf ("\n ***ERROR**** Problemas abriendo el directorio: '%s'\n", raiz);
        exit(EXIT_FAILURE);
    }

    // Lectura del directorio raiz pasado como parámetro
    while ((dp = readdir(dir)) != NULL){
        //Verificaciones pertinentes
        if ((strcmp(dp->d_name,".") != 0) && (strcmp(dp->d_name,"..") != 0)){
            // Se calcula el tamano del string de la ruta
            char* cadena = (char*) malloc(sizeof(char)*150);
            tamano_archivo = strlen(dp->d_name);
            tamano_total = tamano_raiz+tamano_archivo;
            // Se reserva espacio para alojar en ruta el string de la ruta de cada archivo
            char* ruta = (char*)malloc((tamano_total+2)*sizeof(char*));
            // Verificación que se reservo el espacio de manera correcta
            if (ruta==NULL){
                printf("***ERROR*** Problemas en la reserva de espacio\n");
            }
            // Concatenación de la ruta
            strcpy(ruta,raiz);
            strcat(ruta,"/");
            strcat(ruta,dp->d_name);

            // Variable que almacena el tipo de archivo
            tipo = obtenerTipo(ruta);

            // Caso DIRECTORIO
            if (tipo == DT_DIR){
                sprintf(cadena,"/1 %s\n",ruta);
                strcat(respuesta,cadena);
                free(cadena);
            }

            // Caso ARCHIVO REGULAR
            else if (tipo == DT_REG){
                /* Creamos la concatenacion para mostrar: numero de procesador, ruta y $ */
                nro_enlaces = statbuf.st_nlink;
                sprintf(cadena,"/2 %d %s\n",tamano_bloques,ruta);
                strcat(respuesta,cadena);
                free(cadena);
            }

            // Caso ENLACE LÓGICO
            else if (tipo == DT_LNK){
                nro_enlaces = nro_enlaces + 1;
            }
        }

    }
    char* cadena = (char*) malloc(sizeof(char)*150);
    sprintf(cadena,"/3 %d",nro_enlaces);
    strcat(respuesta,cadena);
    strcat(respuesta,"\n");
    free(cadena);
    tamano_informacion = strlen(respuesta);
    informacion_directorio = (char*) malloc(sizeof(char)*tamano_informacion);

    // Se devuelve el string con toda la información pertinente al directorio
    return informacion_directorio;

}

unsigned char obtenerTipo(char* rutaArchivo){
    struct stat fileData;

    if (stat(rutaArchivo,&fileData)==-1) return DT_UNKNOWN;

        switch (fileData.st_mode & S_IFMT){
          case S_IFBLK:  return DT_BLK;
          case S_IFCHR:  return DT_CHR;
          // Tipo directorio
          case S_IFDIR:  return DT_DIR;
          case S_IFIFO:  return DT_FIFO;
          // Enlace lógico
          case S_IFLNK:  return DT_LNK;
          // Archivo Regular
          case S_IFREG:  return DT_REG;
          case S_IFSOCK: return DT_SOCK;
          default:       return DT_UNKNOWN;
    }
  }


