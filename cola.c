/**
*
* Archivo con el tipo cola y sus operaciones
*/

#include <stdio.h>
#include <stdlib.h>
#include "cola.h"

/**
* Crea una cola vacia
* @return Cola vacia
*/
TIPO_COLA* crear_cola() {
    TIPO_COLA *c = (TIPO_COLA *)malloc(sizeof(TIPO_COLA));
    c->tam = 0;
    c->primero = NULL;
    c->ultimo = NULL;
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
        printf("*** PILA DE DIRECTORIOS PENDIENTES ***\n");
        printf("Tamano de la cola = %d\n\n",c->tam);
        TIPO_CAJA* aux = c->primero;
        while (aux != NULL){
            char* nombre =  aux->nombre_directorio;
            printf("Nombre del directorio: %s\n",nombre);
            aux = aux->sig;
        }
    }
}