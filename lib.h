/*
*
* Nom: Joan Guillem Castell Ros-Zanet login:ls23841
* Nom: Arnau Perramon login:ls26027
*
* File lib.h
* Arxiu .H on tindrem tots els include que ens fan falta 
* per la realització de la pràctica.
*
*/

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include <arpa/inet.h>
#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <glob.h>
#include <netdb.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <netinet/in.h>
#include <sys/ipc.h>
#include <sys/socket.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <termcap.h>
#include <fcntl.h>
#include <sys/termios.h>
#include <sys/ioctl.h>
#include <sys/select.h>
#include <conio.h> 
#include <termios.h> 
#include <sys/sem.h>
#include <sys/msg.h>
#include <pthread.h> 
#include "md5.h"

#define MAX_FRAME 115 // MAX_FRAME: valor del tamany de la nostra trama.
#define MAX 256 // MAX: constant que en determina el màxim de caràcters que pot tenir un missatge.
#define ERROR -1 // ERROR: constant de valor -1, per quan una funció té una fallida.
#define OK 10 	// OK: constant de valor 1, per quan una funció acaba de forma sactisfactória.
#define MAX_DATA 100

#include "config.h"
#include "RoomsList.h"
#include "auxiliar.h"
#include "log.h"
#include "UserList.h"
#include "sockets.h"
#include "command.h"
