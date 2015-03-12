

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

typedef struct{
	int fd[2];
} pipe_struct;


// Revisa si hay algun proceso ocupado
int estado_ocupado (int* arreglo_estados, int num_procesos){
	int i,disponible;
	for (i = 0; i < num_procesos; ++i){
		// Caso en el que hay alguien ocupado
		if (arreglo_estados[i] == 1){
			return 1;
		}
	}
	return 0;
}

// Revisa si hay algun proceso disponible
int estado_disponible (int* arreglo_estados, int num_procesos){
	int i,disponible;
	for (i = 0; i < num_procesos; ++i){
		// Caso en el que hay alguien disponible
		if (arreglo_estados[i] == 0){
			return 1;
		}
	}
	return 0;
}


// Indica la primera posicion donde hay un proceso diponible
int proceso_disponible(int* arreglo_estados, int num_procesos){
	int i,disponible;
	for (i = 0; i < num_procesos; ++i){
		// Hay alguien desocupado
		if (arreglo_estados[i] == 0){
			return i;
		}
	}
	return -1;

}

void manejoSalida(char* informacion, char* archivo_salida){
	FILE *fp;
	char * auxiliar;

	fp = fopen (archivo_salida, "a");

  	auxiliar = strtok (informacion,"$");
  	fprintf(fp, "%s\n",auxiliar);

  	while (auxiliar != NULL){
    auxiliar = strtok (NULL, "$");
    fprintf(fp, "%s\n",auxiliar);
  }
  fclose(fp);

}

void manejoHijo(char* arreglo,TIPO_COLA* c, char* salida, int enlaces,int* estado_procesos)
{
	int lqs = 0, i=0;
	char* aux;
	int tamanio = 27;
	char cero	= '0';
	char uno	= '1';
	char dos	= '2';
	char tres	= '3';
	char cuatro	= '4';
	char slash	= '/';
	char nulo	= '\0';
	char salto	= '\n';
	char* tmp1,tmp2,tmp,tmp4;

	while (arreglo[i] != nulo) {
		
		if (arreglo[i] == slash)
		{
			i = i+1;
			if (arreglo[i] == uno)
			{ /* directorio */
				char* tmp1 = (char*) malloc(sizeof(char)*100);
				int j = 0;
				i = i+2;

				memset(tmp1,0,100);
				printf("entre a al primer caso\n");

				while (arreglo[i] != salto)
				{
					tmp1[j] = arreglo[i];
					i++;
					j++;
				}

				encolar(tmp1, c);
				printf("%s\n",tmp1);
				free(tmp1);
			}

			else if (arreglo[i] == dos)
			{ /* regular */
				char* tmp2 = (char*) malloc(sizeof(char)*100);
				int j = 0;
				i=i+2;

				memset(tmp2,0,100);

				while (arreglo[i] != salto)
				{
					tmp2[j] = arreglo[i];
					i++;
					j++;
				}
				manejoSalida(tmp2, salida);
				printf("%s\n",tmp2);
				free(tmp2);
			}

			else if (arreglo[i] == tres) 
			{	/* enlaces logicos */
				char* tmp3 = (char*) malloc(sizeof(char)*100);
				int j = 0;
				i=i+2;

				memset(tmp3,0,100);

				while (arreglo[i] != salto)
				{	
					tmp3[j] = arreglo[i];
					i++;
					j++;
				}
				lqs = atoi(tmp3);
				enlaces = enlaces + lqs; /*lqs: lo que ocupa en disco el archivo actual*/
				free(tmp3);
			}

			else if (arreglo[i] == cuatro) 
			{	/* Proceso a liberar */
				char* tmp4 = (char*) malloc(sizeof(char)*100);
				int j = 0;
				i++;

				memset(tmp4,0,100);

				while (arreglo[i] != salto)
				{
					tmp4[j] = arreglo[i];
					i++;
					j++;
				}
				lqs = atoi(tmp4);
				estado_procesos[lqs] = 0; /* Liberamos el proceso */
				free(tmp4);
			}
			i++;
		}

		else
		{
			printf("TIENES PEOS DE PANA\n");
			i++;
		}
	}
}

void manejoTrabajadores (int posicion, pipe_struct* arreglo_pipes, pipe_struct* pipe_principal){
	while(1){

		char* leer_informacion_raiz = (char*) malloc(sizeof(char)*100);
		char* escribir_informacion_raiz = (char*) malloc(sizeof(char)*5000);
		char* auxiliar = (char*) malloc(sizeof(char)*10);
		// Se suspende temporalmente el proceso
		kill(getpid(),SIGSTOP);

		// Manejo de pipes para obtener la ruta del directorio proveniente del padre
		close(arreglo_pipes[posicion].fd[1]);
		read(arreglo_pipes[posicion].fd[0],leer_informacion_raiz, (strlen(leer_informacion_raiz)+1));

		// Llamada a la función que maneja los directorios
		escribir_informacion_raiz = ManejoDirectorios(leer_informacion_raiz);
		free(leer_informacion_raiz);
		sprintf(auxiliar,"/4 %d",posicion);
		strcat(escribir_informacion_raiz,auxiliar);
		strcat(escribir_informacion_raiz,"\n");

		close(pipe_principal[0].fd[0]);
		read(pipe_principal[0].fd[1],escribir_informacion_raiz, (strlen(escribir_informacion_raiz)+1));
		free(escribir_informacion_raiz);

		// SE LE MANDA UNA SEÑAL AL PADRE Y ESA SEÑAL SIGNIFICA QUE DEBE LEER EL PIPE


	}
}


void resolver(int concurrencia,char* salida,char* raiz){
	int i,ocupado,disponible,proceso_activo,id_trabajador,enlaces_logicos;
	char* escritura_maestro = (char*) malloc(sizeof(char)*100);
	char *lectura_maestro = (char*) malloc(sizeof(char)*5000);
	// Estructuras necesarias para el manejo de procesos
	pid_t *trabajadores = (pid_t *) malloc(concurrencia*sizeof(pid_t));
	int *estado_trabajadores = (int*) malloc(concurrencia*sizeof(int));

	// Estructuras necesarias para el manejo de pipes
    pipe_struct* pipes_trabajadores = (pipe_struct*) malloc(concurrencia*sizeof(pipe_struct));
    pipe_struct* pipe_maestro = (pipe_struct*) malloc(sizeof(pipe_struct));

    // Pipes donde el maestro le pasa información a los trabajadores
    for (i = 0; i < concurrencia; ++i) {
    	pipe(pipes_trabajadores[i].fd);

    }

    // Pipe donde los trabajadores le pasan información al maestro
    pipe(pipe_maestro[0].fd);


	for (i = 0; i < concurrencia; ++i) {
    	trabajadores[i] = fork();

    	// ERROR en la creación de procesos
      	if (trabajadores[i] < 0) {
        	perror("ERROR EN LA CREACIÓN DE PROCESOS");
        	exit(EXIT_FAILURE);
      	}
      	if(trabajadores[i] == 0){
      		manejoTrabajadores(i,pipes_trabajadores,pipe_maestro);
      	}


    }

    // Todos los procesos en un comienzo estan libres
    for(i = 0; i<concurrencia; i++){
    	estado_trabajadores[i] = 0;
    }

    // Se crea la cola donde se almacenarán todos los directorios
    TIPO_COLA* cola_directorios = crear_cola();
    // Se encola el directorio actual
    encolar(raiz,cola_directorios);

    ocupado = estado_ocupado(estado_trabajadores,concurrencia);

    while ((!(cola_vacia(cola_directorios))) || ((cola_vacia(cola_directorios)) && (ocupado))){

    	disponible = estado_disponible(estado_trabajadores,concurrencia);
    	while(disponible && !(cola_vacia(cola_directorios))){
    		// Inicializacion de todas las variables para poder llevar a cabo el manejo de directorios
    		proceso_activo = proceso_disponible(estado_trabajadores,concurrencia);
    		trabajadores[proceso_activo]=getpid();
    		escritura_maestro = desencolar(cola_directorios);

    		// Se escribe en el pipe la ruta del directorio a manejar
    		close(pipes_trabajadores[proceso_activo].fd[0]);
			read(pipes_trabajadores[proceso_activo].fd[1],escritura_maestro, (strlen(escritura_maestro)+1));
			// Se manda una señal al proceso para que lea el pipe
    		kill (trabajadores[proceso_activo], SIGCONT);
    		// Se indica que el proceso está trabajando
    		estado_trabajadores[proceso_activo] = 1;

    	}

		close(pipe_maestro[1].fd[1]);
		read(pipe_maestro[0].fd[0],lectura_maestro, (strlen(lectura_maestro)+1));

			/*** PROCEDIMIENTO QUE SE ENCARGA DE SEPARAR LA INFORMACION DEL PIPE ***/

			// HAY QUE ACTIVAR EL PROCESO QUE ME PASO LA INFORMACION, ES DECIR VOLVER A PONER QUE ESTA DISPONIBLE
			// ENCOLAR SUS DIRECTORIO
			// ESCRIBIR EN EL ARCHIVO DE SALIDA SUS BLOQUES
			// SUMAR LA CANTIDAD DE ENLACES LOGICOS
		ocupado = estado_ocupado(estado_trabajadores,concurrencia);
    }


}


// Indica la primera posicion donde hay un proceso diponible




int main(int argc, char *argv[]){

	int opcion;
	int boolDirectorio = 0;
	int boolProcesos = 0;
	int cantidadParametros = 0;
	int boolAyuda = 0;
	argumentos args;
	int llamada;

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
     	args.directorio = (char*)malloc(sizeof(1000));
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
    	args.numProcesos = 1;
    	args.directorio = (char*)malloc(sizeof(1000));
    	args.directorio = getcwd(NULL,0);
     	args.salida = (char*)malloc(sizeof(argv[1]));
    	strcpy(args.salida,argv[1]);
    }

    resolver(args.numProcesos,args.salida,args.directorio);
}


