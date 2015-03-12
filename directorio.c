#include "directorio.h"

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

TIPO_COLA* crear_cola() {
    TIPO_COLA *c = (TIPO_COLA *)malloc(sizeof(TIPO_COLA));
    c->tam = 0;
    c->primero = NULL;
    c->ultimo = NULL;
    printf("cree una cola\n");
    return c;
}

/**
* Agrega un elemento al final de la cola
* @param *a nodo del arbol a encolar
* @param *c cola a encolar elemento
*/
void encolar (char* elemento, TIPO_COLA *c) {
    TIPO_CAJA *aux = (TIPO_CAJA *)malloc(sizeof(TIPO_CAJA));
    aux->nombre_directorio = elemento;
    aux->sig = NULL;
    if (c->tam == 0) {
       c->primero=aux;
       c->ultimo=aux;
    }
    else {
      c->ultimo->sig = aux;
      c->ultimo = aux;
    }
    c->tam++;

}



int cola_vacia(TIPO_COLA *c){
  if (c->tam == 0){
    return 1;
  }
  else {
    return 0;
  }
}
/**
* Obtiene el primer elemento de la cola y lo elimina
* @param *c cola a revisar el tamano
* @return nodo del arbol a revisar
*/
char* desencolar (TIPO_COLA *c) {
    TIPO_CAJA* aux = c->primero;
    char* nombre = aux->nombre_directorio;
    c->primero = c->primero->sig;
    free(aux);
    c->tam--;
    return nombre;

}

/**
* Destruye una cola
* @param *c cola a destruir
*/
void destruir_cola (TIPO_COLA *c) {
    TIPO_CAJA* aux = c->primero;
    TIPO_CAJA* aux1;
    while (c->tam>0) {
        aux1 = aux;
        aux = aux->sig;
        free(aux1);
    }

    free(aux);
}

void imprimir_cola(TIPO_COLA *c){
    if (cola_vacia(c)){
        fprintf(stderr,"La cola está vacía\n.");
    return;
    }
    else{
        printf("\n");
        printf("*** COLA DE DIRECTORIOS PENDIENTES ***\n");
        printf("Tamano de la cola = %d\n\n",c->tam);
        TIPO_CAJA* aux = c->primero;
        while (aux != NULL){
            char* nombre =  aux->nombre_directorio;
            printf("Nombre del directorio: %s\n",nombre);
            aux = aux->sig;
        }
    }
}


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


