/*
*
* Nom: Joan Guillem Castell Ros-Zanet login:ls23841
* Nom: Arnau Perramon login:ls26027
*
* File: server.c
* Codi del server.
*
*/

/*Includes*/
#include "lib.h"

int socketFD; // socketFD: file descriptor del socket del servidor.
UserList users; // users: linked list amb la informació dels usuaris.
RoomList rooms; // rooms: linked list amb la informació de les sales.
pthread_t thread;	// thread: variable del thread.
static pthread_mutex_t mtxCom = PTHREAD_MUTEX_INITIALIZER; // mtxCom: mutex que s'encarrega de cuidar els comandes.

// RSI: Rutina de interrupció
void RSI(){
	writeMsg("\nInterrupcio per CTRL+C.\n");
	pthread_mutex_destroy(&mtxCom);
	saveRooms(rooms);
	deleteList(&users);
	roomsDeleteList(&rooms);
	close(socketFD);
	exit(-1);
}

// servDedicated: funció del servidor dedicat.
void* servDedicated(void* arg){
	int *socket = (int*) arg;
	int end = 0;
	char buff[MAX_FRAME];
	char name[8];
	int clientFD = *socket; //fem que clientFD sigui el punter socket, ja que, si tenim més d'un client i només fem 
							//servir el punter, només enviariem l'informació del últim file descriptor que ha passat 
							//per aquesta funció.
	while(end == 0){
		if(read(clientFD, buff, MAX_FRAME) < MAX_FRAME){
			end = ERROR;
		}else{
			end = serverCommands(clientFD, buff, rooms, mtxCom);
		}
	}
	getNameBySocket(users, clientFD, name);
	setSocket(users, name, -1);
	bzero(buff, MAX_FRAME);
	sprintf(buff, "%s s'ha desconnectad\n", name);
	writeMsg(buff);
	close(clientFD);
	pthread_exit(NULL);
}

// getConnectionInfo: extreu la informació del fitxer "config.dat".
// Retorna OK, si ha acabat amb éxit, o ERROR, si hi hagut algn error.
int getConnectionInfo(Connection *c){
	char *portAux;	// portAux: cadena de caràcter que guarda el port del fitxer.
	int i = 0;
	char aux;	// aux: variable que fem servir per parsejar el fitxer.
	int fileDescriptor;	// fileDescriptor: variable on es guardarà un file descriptor.
	fileDescriptor = getFD("config.dat"); // Obtenim el file descriptor de "config.dat".
	if(fileDescriptor < 0){	// Comprovem la validesa del file descriptor.
		writeMsg("Error al obrir el fitxer config.dat\n");
		exit(ERROR);
	}
	read(fileDescriptor, &aux, 1);
	portAux = (char*) malloc(sizeof(char));
	memset(c->ip, 0, 16);
	while(aux != '\n'){
		if(i < 16){
			c->ip[i] = aux;	// Guardem caràcter a caràcter la ip que ens dona el fitxer.
		}
		i++;
		read(fileDescriptor, &aux, 1);
	}
	if(i > 16){
		return ERROR;
	}
	i = 0;
	read(fileDescriptor, &aux, 1);
	while(aux != '\0' && aux != '\n'){
		portAux[i] = aux;
		portAux = (char*) realloc(portAux, (sizeof(char)*(i+1)));
		i++;
		read(fileDescriptor, &aux, 1);
	}
	portAux[i] = '\0';
	c->port = atoi(portAux);
	close(fileDescriptor);
	return OK;
}

// getInfo: agafa la informació del fitxer dels usuaris.
void getInfo(int fdUsers, char *buff){
	char aux; // aux: caràcter auxiliar per parsejar el fitxer.
	int i = 0; // i: contardor.
	read(fdUsers, &aux, 1);
	while(aux != ':' && aux != '>'){
		if(aux != '<' && aux != '\n'){
			buff[i] = aux;
			i++;
		}
		read(fdUsers, &aux, 1);
	}
}

// getPass: agafa la contrasenya del fitxer dels usuaris.
void getPass(int fdUsers, char *buff){
	char aux; // aux: caràcter auxiliar per parsejar el fitxer.
	char *pass; // pass: cadena de caràcters dinàmica on es guardarà la contrasenya.
	char *md5; // md5: cadena de caràcters on guardem la contrasenya(pass) codificat en MD5.
	int i = 0; // i: contardor.
	pass = (char*)malloc(sizeof(char));
	read(fdUsers, &aux, 1);
	while(aux != '>'){
		read(fdUsers, &aux, 1);
		if(aux != '>' && aux != '\n' && aux != '<' && aux != ':'){
			pass[i] = aux;
			i++;
			pass = (char*)realloc(pass, sizeof(char)*(i+1));
		}
	}
	pass[i] = '\0';
	md5 = MD5Data(pass, strlen(pass), NULL);
	strcpy(buff, md5);
}



// loadUsersList: funció que volca la informació del fitxer users.dat i
// ho volca a la llista.
int loadUsersList(UserList *list){
	char name[8];
	char pass[33];
	char cId[40];
	int id;
	int i;
	int fdUsers; // File descriptor del fitxer user.dat.
	char aux;
	User node;
	fdUsers = getFD("users.dat"); // Obtenim el file descriptor de "users.dat".
	if(fdUsers < 0){
		writeMsg("Error al obrir el fitxer users.dat\n");
		return ERROR;
	}
	while(read(fdUsers, &aux, 1) == 1){
		memset(name, 0, 8);
		memset(pass, 0, 33);
		memset(cId, 0, 40);
		memset(node.name, 0, 8);
		memset(node.passwordMD5, 0, 33);
		getInfo(fdUsers, name);
		read(fdUsers, &aux, 1);
		getPass(fdUsers, pass);
		read(fdUsers, &aux, 1);
		getInfo(fdUsers, cId);	
		id = atoi(cId);
		strcpy(node.name, name);
		strcpy(node.passwordMD5, pass);
		node.id = id;
		node.next = NULL;
		if(insertNode(list, node) < 0){
			return ERROR;
		}
		i++;
	}
	close(fdUsers);
	return OK;
}

// loadRoomsList: funció que volca la informació del fitxer rooms.dat i
// ho volca a la llista de sales.
int loadRoomsList(RoomList *list){
	char admin[8];
	char name[32];
	char cPeople[10];
	char aux;
	int HTMLfd;
	Room node;
	int fdRooms = getFD("rooms.dat"); // Obtenim el file descriptor de "rooms.dat".
	if(fdRooms < 0){
		return ERROR;
	}
	while(read(fdRooms, &aux, 1) == 1){
		memset(admin, 0, 8);
		memset(name, 0, 32);
		memset(cPeople, 0, 10);
		memset(node.admin, 0, 8);
		memset(node.name, 0, 33);
		getInfo(fdRooms, admin);
		read(fdRooms, &aux, 1);
		getInfo(fdRooms, name);
		read(fdRooms, &aux, 1);
		getInfo(fdRooms, cPeople);
		read(fdRooms, &aux, 1);
		node.maxUsers = atoi(cPeople);
		strcpy(node.name, name);
		strcpy(node.admin, admin);
		node.next = NULL;
		if((HTMLfd = initLog(name)) < 0){
			return ERROR;
		}
		node.HTMLfd = HTMLfd;
		if(roomsInsertNode(list, node) < 0){
			return ERROR;
		}
	}
	close(fdRooms);
	return OK;
}

// main: funció principal del servidor.
int main(){
	Connection connection; // connection: estructura amb la informació del servidor.
	int error = 0;
	struct sockaddr_in clientDirection;
	int socketClient;
	//Inicialitzem el signal de CTRL+C
	signal(SIGINT, RSI);
	//Carregem la informació del fitxer config.dat
	if(getConnectionInfo(&connection) < 0){
		writeMsg("Error al carregar les dades de connexio\n");
		exit(ERROR);
	}
	//Carregem la informació del fitxer user.dat
	if(createList(&users) < 0){
		writeMsg("Error al crear fitxer\n");
		exit(ERROR);
	}
	if(loadUsersList(&users) < 0){
		writeMsg("Error al carregar fitxer d'usuaris\n");
		exit(ERROR);
	}
	goToFirst(&users);
	//Carregem la informació del fitxer rooms.dat
	if(roomsCreateList(&rooms) < 0){
		writeMsg("Error al crear fitxer\n");
	}
	loadRoomsList(&rooms);
	roomsGoToFirst(&rooms);
	// Creem socket
	socketFD = pasiveSocket(connection);
	if(socketFD == ERROR){
		writeMsg("Error al crear socket\n");
		deleteList(&users);
		exit(ERROR);
	}
	// Acceptar client
	socklen_t c_len = sizeof(clientDirection);
	while(1){
		socketClient = accept (socketFD, (void *) &clientDirection, &c_len);
		if (socketClient < 0){
			writeMsg("Error al aceptar la peticion del cliente!\n");
			error = -1;
		}
		if(serverProtocol(users, socketClient) < 0 && error == 0){
			writeMsg("Error al aceptar la peticion del cliente!\n");
			close(socketClient);
			error = -1;
		}
		if(error == 0){
			if(pthread_create(&thread, NULL, servDedicated, &socketClient) != 0){
				writeMsg("Error al crear thread\n");
				exit(ERROR);
			}
		}
	}
	return OK;
}
