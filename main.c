
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "directorio.h"
#include "cola.h"
// Estructura que almacena los parámetros de entrada
typedef struct{
	int numProcesos;
	char* directorio;
	char* salida;

}argumentos;


int opcion;
int boolDirectorio = 0;
int boolProcesos = 0;
int cantidadParametros = 0;
int boolAyuda = 0;
argumentos args;
int llamada;





int main(int argc, char *argv[]){


	llamada = strcmp(argv[0],"./UsoDisco");

	// Se verifica primero que el usuario hizo la llamada de manera al programa de manera correcta
	if (llamada!=0){
		printf("\n*** ERROR UsoDisco*** \n\n");
		printf("El comando de llamada es 'UsoDisco'\n");
		exit(EXIT_FAILURE);
	}

    while ((opcion = getopt(argc, argv,"n:d:h")) != -1) {
    	switch (opcion) {

    	// El usuario introduce la opción de ayuda
		case 'h':
			cantidadParametros ++;
			boolAyuda++;
			break;

		// Cantidad de procesos trabajadores introducido por el usuario
		case 'n':
			cantidadParametros ++;
			boolProcesos ++;
		    args.numProcesos = atoi(optarg);
		    if (args.numProcesos<=0){
		    	printf("\n*** ERROR UsoDisco*** \n\n");
		    	printf("Debe indicar un nivel de concurrencia valido\n");
		    	exit(EXIT_FAILURE);
		    }
		    break;

		// Directorio introducido por el usuario
		case 'd':
			cantidadParametros ++;
			boolDirectorio ++;
			args.directorio = (char*)malloc(sizeof(optarg));
		    strcpy(args.directorio,optarg);
		    break;

    	}

	}


	/* CASOS DE ERRORES DONDE SE EJECUTAN DOS COMANDOS A LA VEZ*/

	if ((boolAyuda) && (boolDirectorio) && (boolProcesos)){
		printf("\n*** ERROR UsoDisco*** \n\n");
		printf("Calcula de forma concurrente, el espacio en disco utilizado por los archivos regulares.");
		printf(" a partir de un directorio usando procesos.\n");
		printf("SINTAXIS:\n");
		printf("Usted ha ingresado una cantidad de parámetros erróneos.\n");
		printf("Existen dos formas de parámetros:\n");
		printf("\t\tOpcion1: [-h]: Muestra un mensaje de ayuda\n");
		printf("\t\tOpcion2: [-n i] [-d directorio] salida: Ejecuta el programa UsoDisco\n");
		printf("Corra nuevamente el programa.\n");
		exit(EXIT_FAILURE);
    }

    if ((boolAyuda) && (boolDirectorio)){
    	printf("\n*** ERROR UsoDisco*** \n\n");
    	printf("Calcula de forma concurrente, el espacio en disco utilizado por los archivos regulares,");
		printf(" a partir de un directorio usando procesos.\n");
		printf("SINTAXIS:\n");
		printf("Usted ha ingresado una cantidad de parámetros erróneos.\n");
		printf("Existen dos formas de parámetros:\n");
		printf("\t\tOpcion1: [-h]: Muestra un mensaje de ayuda\n");
		printf("\t\tOpcion2: [-n i] [-d directorio] salida: Ejecuta el programa UsoDisco\n");
		printf("Corra nuevamente el programa.\n");
		exit(EXIT_FAILURE);
    }

    if ((boolAyuda) && (boolProcesos)){
    	printf("\n*** ERROR UsoDisco*** \n\n");
    	printf("Calcula de forma concurrente, el espacio en disco utilizado por los archivos regulares.");
    	printf(" a partir de un directorio usando procesos.\n");
		printf("SINTAXIS:\n");
		printf("Usted ha ingresado una cantidad de parámetros erróneos.\n");
		printf("Existen dos formas de parámetros:\n");
		printf("\t\tOpcion1: [-h]: Muestra un mensaje de ayuda\n");
		printf("\t\tOpcion2: [-n i] [-d directorio] salida: Ejecuta el programa UsoDisco\n");
		printf("Corra nuevamente el programa.\n");
		exit(EXIT_FAILURE);
    }

    /* CASO EN EL QUE EL USUARIO SELECCIONÓ LA OPCIÓN DE AYUDA*/
	if (boolAyuda){
		printf("\n *** PROGRAMA UsoDisco *** \n\n");
		printf(" La forma de invocación es la siguiente: ");
		printf("./UsoDisco [-n i] [-d directorio] salida\n");
		printf(" -n i: Nivel de concurrencia solicitado. Por defecto crea un solo proceso trabajador \n");
		printf(" -d directorio: Especifica un directorio desde donde calcula el espacio utilizado. ");
		printf("Por defecto hace el cálculo desde el directorio actual\n");
		printf(" salida: Archivo que contendrá la lista de directorios y el espacio en bloques ");
		printf("ocupado por los archivos regulares. A continuación se muestra un ejemplo de salida\n");
		exit(EXIT_SUCCESS);
	}


    /* CASOS DE ERRORES POR FALTA DE ARGUMENTO DE SALIDA */


    if ((cantidadParametros==0) && (argc==1)){
    	printf("\n*** ERROR UsoDisco*** \n\n");
    	printf("Debe introducir un nombre para el archivo de salida\n");
		exit(EXIT_FAILURE);
    }

    if ((cantidadParametros==1) && (argc==3)){
    	printf("\n*** ERROR UsoDisco*** \n\n");
    	printf("Debe introducir el nombre del archivo de salida \n");
    	exit(EXIT_FAILURE);
    }

	if ((cantidadParametros==2) && (argc==5)){
		printf("\n*** ERROR UsoDisco*** \n\n");
    	printf("Debe introducir el nombre del archivo de salida \n");
    	exit(EXIT_FAILURE);
    }

    /* ASIGNACIÓN DE VALORES DEFAULT Y GUARDAR EL NOMBRE DE LA SALIDA DEL PROGRAMA*/


    // Caso en el que el usuario introdujo tanto un directorio como un nivel de concurrencia
    if ((boolDirectorio) & (boolProcesos)){
    	args.salida = (char*)malloc(sizeof(argv[5]));
    	strcpy(args.salida,argv[5]);
    }

    // Caso en el que el usuario introdujo el nivel de concurrencia, mas no un directorio
     if ((!boolDirectorio) & (boolProcesos)){
     	args.directorio = (char*)malloc(sizeof(100));
    	args.directorio = getcwd(NULL,0);
     	args.salida = (char*)malloc(sizeof(argv[3]));
    	strcpy(args.salida,argv[3]);
    }

    // Caso en el que el usuario introdujo un directorio, mas no un nivel de concurrencia
    if ((boolDirectorio) & (!boolProcesos)){
    	args.numProcesos = 1;
    	args.salida = (char*)malloc(sizeof(argv[3]));
    	strcpy(args.salida,argv[3]);

    }

    // Caso en el que usuario no introdujo un nivel de concurrencia ni un directorio
    if ((!boolDirectorio) & (!boolProcesos)){
     	args.salida = (char*)malloc(sizeof(argv[1]));
    	strcpy(args.salida,argv[1]);
    }
}
