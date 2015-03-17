/*
*
* Nom: Joan Guillem Castell Ros-Zanet login:ls23841
* Nom: Arnau Perramon login:ls26027
*
* File: log.c
* Funcions per crear i modificar els fitxes de les sales..
*
*/
#include "lib.h"

// initLog: crea/obra un fitxer .html per al diari de la sala.
// Retorna el file descriptor.
int initLog(char name[32]){
	int fd; 
	char init[256];
	char fileName[40];
	bzero(init, 256);
	bzero(fileName, 40);
	strcat(fileName, name);
	strcat(fileName, ".html");
	name[strlen(fileName)] = '\0';
	if((fd = open (fileName, O_WRONLY)) < 0){
		if ((fd = open (fileName, O_CREAT|O_WRONLY, S_IRUSR|S_IWUSR|S_IXUSR)) < 0) {
			bzero(init, 256);
			sprintf(init, "Error al obrir el fitxer %s", name);
			writeMsg(init);
			return ERROR;
		}
		sprintf(init, "<!DOCTYPE html>\n<html lang=\"en\">\n\n\t<head>\n\t\t<meta charset=\"utf-8\" />\n\t\t<title>[SO]%s</title>\n\n\t</head>\n\n\t<body id=\"index\" class=\"home\">\n\n\n\t</body>\n</html>", name);
		write(fd, init, strlen(init));
	}
	return fd;
}

// updateLog: actualitza el fitxer amb la nova informació.
void updateLog(int fd, char name[32], char user[8], char type, char data[100]){
	if (fd > 0){
		char update[]="</body>\n</html>";		
		lseek(fd, -15, SEEK_END);
		write(fd, "<p>", 3);
		write(fd, "<font color=\"", strlen("<font color=\""));
		if (type == 'D'){
			write(fd, "red", 3);			
		}else{
			write(fd, "green", 5);			
		}	
		write(fd, "\">[", strlen("\">["));
		write(fd, name, strlen(name));
		write(fd, "] [", strlen("] ["));
		write(fd, &type, 1);
		write(fd, "] [", strlen("] ["));	
		write(fd, user, strlen(user));		
		write(fd, "] ", strlen("] "));	
		write(fd, data, strlen(data));
		write(fd, "</font></p>", strlen("</font></p>"));
		write(fd, update, 15);
	}
}
