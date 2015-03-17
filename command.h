/*
*
* Nom: Joan Guillem Castell Ros-Zanet login:ls23841
* Nom: Arnau Perramon login:ls26027
*
* File: comand.h
* Fitxer .h on tenim el tipus Command declarat i totes les funcions públiques 
* del command.c.
*
*/

// Tipus CommandLine: guardem la informació del usuari.
// commandName: nom de la comanda.
// dataP: cadena de caràcters dinàmica que conté la primera part de la informació que introduïm per la línea de comandes.
// dataS: cadena de caràcters dinàmica que conté la segona part de la informació que introduïm per la línea de comandes.
// NOTA: els dos camps de dades són opcionals, hi ha comandes que no tenan cap tipus de dades.
typedef struct command {
	char commandName[12];
	char *dataP;
	char *dataS;
}CommandLine;

// setCommand: esperem a que el usuari introdueixi la comanda
// per la linea de comanda.
int setCommand(CommandLine *c, int socketFD);

// connectToServer: Fa tota al part de la rutina de conexió.
// Retorna el file descriptor del socket que hem iniciat.
int connectToServer(CommandLine *c, Client user);

// doCommand: s'executa la comanda introduida per linea de 
// comanda.
void doCommand(CommandLine *c, Client user, int socketFD);

// serverCommands: fa les commandes que li arriben.
// Retorna 1 si li ha arribat una trama de desconexió, sinó retorna 0.
int serverCommands(int socketClient, char buff[MAX_FRAME], RoomList rooms, pthread_mutex_t mtxCom);
