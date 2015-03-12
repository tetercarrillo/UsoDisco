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
#include "cola.h"



char* ManejoDirectorios(char* raiz);

unsigned char obtenerTipo(char* rutaArchivo);

#endif // DIRECTORIOH