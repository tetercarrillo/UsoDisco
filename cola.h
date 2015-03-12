/**
*
* Archivo con el tipo cola y sus operaciones
*/

#ifndef COLAH
#define COLAH


typedef struct caja {
  char* nombre_directorio;
  struct caja *sig;
} TIPO_CAJA;


typedef struct cola {
    int tam;
    TIPO_CAJA *primero;
    TIPO_CAJA *ultimo;
}  TIPO_COLA;


/**
* Crea una cola vacia
* @return Cola vacia
*/
TIPO_COLA* crear_cola();

/**
* Agrega un elemento al final de la cola
* @param *a nodo del arbol a encolar
* @param *c cola a encolar elemento
*/
void encolar (char* elemento, TIPO_COLA* c);

int cola_vacia(TIPO_COLA *c);

/**
* Obtiene el primer elemento de la cola y lo elimina
* @param *c cola a revisar el tamano
* @return nodo del arbol a revisar
*/
char* desencolar (TIPO_COLA *c);

/**
* Destruye una cola
* @param *c cola a destruir
*/
void destruir_cola (TIPO_COLA *c);

void imprimir_cola(TIPO_COLA *c);




#endif // COLAH