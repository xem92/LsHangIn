/*
*
* Nom: Joan Guillem Castell Ros-Zanet login:ls23841
* Nom: Arnau Perramon login:ls26027
*
* File: client.c
* Codi del client.
*
*/

/*Includes*/
#include "lib.h"

int socketFD;

// RSI: Rutina de interrupció
void RSI(){
	writeMsg("\nInterrupcio per CTRL+C.\n");
	close(socketFD);
	exit(-1);
}

// login: funció on demanem al usuari les seves dades.
void login(Client *user){
	char *buff; // buff: buffer per agafar el nom i la contrasenya del usuari.
	char *md5;
	int nb; // nb: tamany de la cadena de caracters dinàmica.
	int ok = 0; // ok: "boolean", si el nom del usuari és correcte ok=1, sinó ok=0.
	int i; // i: contador.
	//Inicialitzem el signal de CTRL+C
	signal(SIGINT, RSI);
	fd_set set;
	while(ok == 0){
		writeMsg("Nom usuari:\n");
		FD_ZERO(&set);
		FD_SET(STDIN_FILENO, &set);
		select(255, &set, NULL, NULL, NULL);
		if (FD_ISSET(STDIN_FILENO, &set)){
			ioctl(STDIN_FILENO, FIONREAD, &nb);
			buff = malloc(nb+1);
			read(0, buff, nb);
		}
		if(strlen(buff) < 9){
			if(strlen(buff) == 9){
			} else {
				for(i = strlen(buff); i < 9; i++){
					buff[i - 1] = '\0';
				}
			}
			strcpy(user->name, buff);
			ok = 1;
		} else {
			writeMsg("\nNota: Nom d'usuari massa llarg\n\n");
		}
	}
	writeMsg("Password:\n");
	free(buff);
	FD_ZERO(&set);
	FD_SET(STDIN_FILENO, &set);
	select (255, &set, NULL, NULL, NULL);
	if (FD_ISSET(STDIN_FILENO, &set)){
		ioctl(STDIN_FILENO, FIONREAD, &nb);
		buff = malloc(nb+1);
		read(0, buff, nb);
	}
	buff[nb-1] = '\0';
	md5 = MD5Data(buff, strlen(buff), NULL);
	strcpy(user->passwordMD5, md5);
}

// main: funció principal del client.
int main(){
	Client client; // client: variable que conté tota la informació del client.
	CommandLine command; // command: estructura de la comanda introduida per la line de comandes.
	socketFD = -1; // socketFD: file descriptor del socket. Inicilitzat a -1, indicant que no està connectat a cap socket.
	login(&client);
	while(1){
		while(socketFD < 0){
			if(setCommand(&command, socketFD) > 0){
				socketFD = connectToServer(&command, client);
			}else{
				writeMsg("Comanda incorrecta\n");
			}
		}
		writeMsg("Benvingut a LSHangIn.\n");
		while(1){
			switch(setCommand(&command, socketFD)){
				case 1:
					doCommand(&command, client, socketFD);
					break;
				case 2:
					break;
				default:
					writeMsg("Comanda incorrecta\n");
					break;
			}
		}
		close(socketFD);
	}
	return 0;
}
