/*
*
* Nom: Joan Guillem Castell Ros-Zanet login:ls23841
* Nom: Arnau Perramon login:ls26027
*
* File: auxiliar.c
* Funcions auxiliars.
*
*/

/*Includes*/
#include "lib.h"

// writeMsg: escriu un missatge per pantalla.
void writeMsg(char buff[MAX]){
	write(1, buff, strlen(buff));
}

// getFD: funció on obtenim el file descriptor del fitxer que volem obrir.
int getFD(char fileName[11]){
	int fd; // fd: file descriptor.
	fd = open(fileName, O_RDONLY);
	if(fd < 0){
		return ERROR;
	}
	return fd;
}

// saveRooms: guarda les dades de les sales en el fitxer "rooms.dat".
int saveRooms(RoomList rooms){
	int fd;
	char info[MAX];
	fd = open("rooms.dat", O_CREAT|O_WRONLY|O_TRUNC, S_IRUSR|S_IWUSR|S_IXUSR);	//Li donem permisos de lectura i escriptura,
																				//sinó al tornar a obrir donarà error.
	if(fd < 0){
		writeMsg("Error al obrir el fitxer\n");
		return ERROR;
	}
	roomsGoToFirst(&rooms);
	roomsNextNode(&rooms);
	while(rooms.pdi != NULL){
		sprintf(info, "<%s>:<%s>:<%d>", rooms.pdi->admin, rooms.pdi->name, rooms.pdi->maxUsers);
		write(fd, info, strlen(info));
		roomsNextNode(&rooms);
		if(rooms.pdi != NULL){
			write(fd, "\n", strlen("\n"));
		}
	}
	write(fd, "\0", strlen("\0")); 
	close(fd);
	return OK;
}
