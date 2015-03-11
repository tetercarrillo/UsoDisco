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

#include "directorio.h"
#include "cola.h"

int total_directorios =0;
int total_procesos=0;
int total_archivos=0;
int total_regulares=0;


void ManejoDirectorios(char* raiz){
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
            total_archivos++;

            /**
             * Estructura para determinar información de un directorio
             */
            struct stat statFile;
            if (S_ISDIR(statFile.st_mode)){
                total_directorios++;
                sprintf(nombre_archivo,"%s",ruta);

                //DESARROLLO


            }
        }
    }
}
