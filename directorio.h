#ifndef DIRECTORIOH
#define DIRECTORIOH

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

typedef struct caja {
  char* nombre_directorio;
  struct caja *sig;
} TIPO_CAJA;


typedef struct cola {
    int tam;
    TIPO_CAJA *primero;
    TIPO_CAJA *ultimo;
}  TIPO_COLA;

TIPO_COLA* crear_cola();

void encolar (char* elemento, TIPO_COLA *c);

int cola_vacia(TIPO_COLA *c);

char* desencolar (TIPO_COLA *c);

void destruir_cola (TIPO_COLA *c);

void imprimir_cola(TIPO_COLA *c);

void ManejoDirectorios(TIPO_COLA* c, char* raiz);

unsigned char obtenerTipo(char* rutaArchivo);

#endif // DIRECTORIOH