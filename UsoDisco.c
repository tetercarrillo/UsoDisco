
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "directorio.h"


typedef struc{
	int numProcesos
	char*

}

int numProcesos;                    // Numero de Procesos que realizaran el programa
int option;
char dir[100];						//Almacena el path a examinar
int booleanoDirectorio = 0;			//Indice que si el parametro -d fue introducido por consola
int booleanoNumProcesos = 0;		//Indice que si el parametro -n fue introducido por consola
int cantidadParametro = 0;
char archivoSalida [100];



void proceso(char salida[100], int numProcesos){
	 maestro(dir,salida,numProcesos);


}


/** Almacena los datos introducidos por consola, y aquellos que no fueron pasados, son asignados por defecto.*/
void main(int argc, char *argv[]){



	    while ((option = getopt(argc, argv,"n:d:h")) != -1) { //Funcion para leer los parametros de entrada
	    	switch (option) {

			case 'h':
				contadorParametro ++;
				printf("./UsoDisco [-n i] [-d directorio] salida \n");
				printf(" -n i: Nivel de concurrencia solicitado. Por defecto crea un solo proceso trabajador \n\n");
				printf(" -d directorio:  especifica un directorio desde donde calcula el espacio utilizado. ");
				printf("Por defecto hace el cálculo desde el directorio actual\n\n");
				printf(" salida: rchivo que contendrá la lista de directorios y el espacio en bloques ");
				printf("ocupado por los archivos regulares. A continuación se muestra un ejemplo de salida\n\n");
				exit(EXIT_FAILURE);

			case 'n': 								//Numero de Procesos
				contadorParametro ++;
				booleanoNumProcesos ++;
			    numProcesos = atoi(optarg);
			    break;

			case 'd': 								//directorio a examinar
				contadorParametro ++;
				booleanoDirectorio ++;
			    strcpy(dir,optarg);
			    break;

	    	}


		}

		if (contadorParametro == 3){ // Caso en que introduce -h -n i -d directorio

		    	printf("\n Uso Disco \n");  //Informacion del juego
				printf("SINTAXIS:\n");
				printf("Calcula de forma concurrente, el espacio en disco utilizado por los archivos regulares a partir de un directorio usando procesos.\n");
				printf("Usted ha ingresado una cantidad de parametros erroneos.\n");
				printf("Existe dos formas de parametros:\n");
				printf("\t\tOpcion1: [-h]: Muestra un mensaje de ayuda\n");
				printf("\t\tOpcion2: [-n i] [-d directorio] salida: Ejecuta el programa UsoDisco\n");
				printf("Introduzca la opcion deseada\n");
				exit(EXIT_FAILURE);

	    }

	    if ( (contadorParametro==0) && (argc==1)){  //Caso en que introduce la opcion de correr el programa pero faltan parametros para hacerlo
	    		printf("\n Uso Disco \n");  //Informacion del juego
				printf("SINTAXIS:\n");
				printf("Calcula de forma concurrente, el espacio en disco utilizado por los archivos regulares a partir de un directorio usando procesos.\n");
				printf("Usted ha ingresado una cantidad de parametros erroneos.\n");
				printf("Existe dos formas de parametros:\n");
				printf("\t\tOpcion1: [-h]: Muestra un mensaje de ayuda\n");
				printf("\t\tOpcion2: [-n i] [-d directorio] salida: Ejecuta el programa UsoDisco\n");
				printf("Introduzca la opcion deseada\n");
				exit(EXIT_FAILURE);

	    }

	    if (!booleanoDirectorio){   // Si no fue introducido el directorio por consola. Por defecto es el directorio donde se encuentra el programa
	    	char *DirectorioActual = getcwd(NULL,0);  // Funcion que dice el directorio actual donde se esta trabajando
			strcpy(dir,DirectorioActual);

	    }

	    if (!booleanoNumProcesos){	// Si no fue introducido el numero de Procesos a crear por consola. Por defecto se crea un solo proceso
	    	numProcesos = 1;

	    }

	    if (booleanoDirectorio & booleanoNumProcesos){
	    	strcpy(archivoSalida,argv[5]);
	    }

	     if (!booleanoDirectorio & booleanoNumProcesos){
	    	strcpy(archivoSalida,argv[3]);
	    }

	     if (booleanoDirectorio & !booleanoNumProcesos){
	    	strcpy(archivoSalida,argv[3]);
	    }

	     if (!booleanoDirectorio & !booleanoNumProcesos){
	    	strcpy(archivoSalida,argv[1]);
	    }
