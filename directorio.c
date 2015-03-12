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




void ManejoDirectorios(TIPO_COLA* c, char* raiz){
    // Variable para abrir el directorio
    DIR* dir;
    // Variable para leer el directorio
    struct dirent* dp;
    int tamano_raiz,tamano_archivo, tamano_total;
    char* nombre_archivo;

    tamano_raiz = strlen(raiz);

    // Se abre el directorio y se hace la verificación pertinente
    dir = opendir(raiz);
    if (dir == NULL){
        printf ("\n ***ERROR**** Problemas abriendo el directorio: '%s'\n", raiz);
        exit(EXIT_FAILURE);
    }

    int i = 0;
    // Lectura del directorio
    while ((dp = readdir(dir)) != NULL){
        //Verificaciones pertinentes
        if ((strcmp(dp->d_name,".") != 0) && (strcmp(dp->d_name,"..") != 0)){
            unsigned char tipo;
            tamano_archivo = strlen(dp->d_name);
            tamano_total = tamano_raiz+tamano_archivo;
            char* ruta = (char*)malloc((tamano_total+2)*sizeof(char*));
            // Verificación que se reservo el espacio de manera correcta
            if (ruta==NULL){
                printf("***ERROR*** Problemas en la reserva de espacio\n");
            }
            // Concatenación de la ruta de los tipos de archivos dentro del directorio
            strcpy(ruta,raiz);
            strcat(ruta,"/");
            strcat(ruta,dp->d_name);
            strcat(ruta,"\0");
            //printf("LA RUTA ES %s\n",ruta);

            /**
             * Estructura para determinar información de un directorio
             */

            tipo = obtenerTipo(ruta);

            if (tipo == DT_DIR){
                ManejoDirectorios(c,ruta);
                encolar(ruta,c);
            }
            else if (tipo == DT_REG){
            }
        }
    }
}

unsigned char obtenerTipo(char* rutaArchivo){
    struct stat fileData;

    if (stat(rutaArchivo,&fileData)==-1) return DT_UNKNOWN;

        switch (fileData.st_mode & S_IFMT){
          case S_IFBLK:  return DT_BLK;
          case S_IFCHR:  return DT_CHR;
          case S_IFDIR:  return DT_DIR;
          case S_IFIFO:  return DT_FIFO;
          case S_IFLNK:  return DT_LNK;
          case S_IFREG:  return DT_REG;
          case S_IFSOCK: return DT_SOCK;
          default:       return DT_UNKNOWN;
    }
  }


