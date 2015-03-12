

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


void manejoProcesos_suspension(int* id_procesos, int* status_procesos,int posicion){
	while(1){
		kill(id_procesos[posicion], SIGSTOP);
		printf("SE SUSPENDIO AL PROCESO %d\n",id_procesos[posicion]);
		status_procesos[posicion] = 0;
	}
}

void manejoProcesos_activacion(int* id_procesos, int* status_procesos,int posicion){
	while(1){
		kill(id_procesos[posicion], SIGCONT);
		printf("SE ACTIVO EL PROCESO %d\n",id_procesos[posicion]);
		status_procesos[posicion] = 1;
	}
}

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

char* concatenacion(int posicion,char* string){
	char* cadena;
	strcat(string,"-");
	sprintf(cadena,"%d",posicion);
	strcat(string,cadena);

	return string;
}


void resolver(int concurrencia,char* salida,char* directorio){
	int i;
	// Estructuras necesarias para el manejo de procesos
	pid_t *trabajadores = (pid_t *) malloc(concurrencia*sizeof(pid_t));
	int *estado_trabajadores = (int*) malloc(concurrencia*sizeof(int));
	int *trabajadores_id = (int*) malloc(concurrencia*sizeof(int));


	for (i = 0; i < concurrencia; ++i) {
    	trabajadores[i] = fork();

    	// ERROR en la creación de procesos
      	if (trabajadores[i] < 0) {
        	perror("ERROR EN LA CREACIÓN DE PROCESOS");
        	exit(EXIT_FAILURE);
      	}
      	if(trabajadores[i] == 0){
      		trabajadores_id[i] = getpid();
      		printf("entre\n");
      		manejoProcesos_suspension(trabajadores_id,estado_trabajadores,i);
      		break;
      	}


    }

    // Estructuras necesarias para el manejo de pipes
    pipe_struct* pipes_trabajadores = (pipe_struct*) malloc(concurrencia*sizeof(pipe_struct));
    pipe_struct* pipe_maestro = (pipe_struct*) malloc(sizeof(pipe_struct));

    // Pipes donde el maestro le pasa información a los trabajadores
    for (i = 0; i < concurrencia; ++i) {
    	pipe(pipes_trabajadores[i].fd);

    }

    // Pipe donde los trabajadores le pasan información al maestro
    pipe(pipe_maestro[0].fd);


    // Se encolan los directorios de la carpeta raiz pasada por terminal o por default se toma la actual
    TIPO_COLA* cola_directorios = crear_cola();
    char* contenido_raiz;
    ManejoDirectorios(cola_directorios,directorio,contenido_raiz);
    int ocupado = estado_ocupado(estado_trabajadores,concurrencia);
    int disponible, j,tamano_string;
    char* ruta_auxiliar;
    char* respuesta;
    char* auxiliar;
    char* informacion_archivos;
    char* informacion_proceso;
    char* informacion_enlace;
    int cont_enlaces = 0;
    while ((!(cola_vacia(cola_directorios))) || ((cola_vacia(cola_directorios)) && (ocupado))){
			disponible = estado_disponible(estado_trabajadores,concurrencia);
			ocupado = estado_ocupado(estado_trabajadores,concurrencia);

			if(!cola_vacia(cola_directorios)){
				ruta_auxiliar = desencolar(cola_directorios);


				if(disponible){
					char* escritura;
					int aux_enlaces;
					j = proceso_disponible(estado_trabajadores,concurrencia);
					// Escritura del pipe
					close(pipes_trabajadores[j].fd[0]);
					write(pipes_trabajadores[j].fd[1], ruta_auxiliar, (strlen(ruta_auxiliar)+1));


					manejoProcesos_activacion(trabajadores_id,estado_trabajadores,j);
					printf("HOLA\n");
					// Lectura del pipe
					close(pipes_trabajadores[j].fd[1]);
					read(pipes_trabajadores[j].fd[0], ruta_auxiliar, (strlen(ruta_auxiliar)+1));


					ManejoDirectorios(cola_directorios,ruta_auxiliar,respuesta);

					// tamano_string = strlen(respuesta);
					// escritura = (char*) malloc(sizeof(char)*tamano_string);
					// escritura = concatenacion(j,respuesta);

					// Division de la informacion pasada por el directorio
					auxiliar = strtok(respuesta,"#");
					informacion_archivos=auxiliar;
					while(auxiliar != NULL){
						auxiliar = strtok(NULL,"#");
						informacion_enlace = auxiliar;
						aux_enlaces = atoi(informacion_enlace);
					}
					cont_enlaces = cont_enlaces + aux_enlaces;

					close(pipe_maestro[0].fd[0]);
			    	write(pipe_maestro[0].fd[1],informacion_archivos, (strlen(informacion_archivos)+1));

					manejoProcesos_suspension(trabajadores_id,estado_trabajadores,j);

					// Lectura
					close(pipe_maestro[0].fd[1]);
			    	read(pipe_maestro[0].fd[0],informacion_archivos, (strlen(informacion_archivos)+1));

			    	manejoSalida(informacion_archivos,salida);
			    	free(respuesta);



	    		}



	    		else{
	    			break;
	    		}
	    	// Escritura



    		}
    		else{
    			break;
    		}

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


