/*
*
* Nom: Joan Guillem Castell Ros-Zanet login:ls23841
* Nom: Arnau Perramon login:ls26027
*
* File: sockets.c
* Funcions per els sockets.
*
*/

/*Includes*/
#include "lib.h" 

// hostnameToIp: recupera la ip a partir del nom del domini.
int hostnameToIp(char *hostname , char *ip){
	struct hostent *he;
	struct in_addr **addr_list;
	int i;
	if((he = gethostbyname(hostname)) == NULL){
		// get the host info
		perror("gethostbyname");
		return ERROR;
    }
	addr_list = (struct in_addr **) he->h_addr_list;
	for(i = 0; addr_list[i] != NULL; i++){
		//Return the first one;
		strcpy(ip , inet_ntoa(*addr_list[i]) );
		return OK;
	}
	return ERROR;
}

//getFrameData: recupera el camp de dada d'una trama.
void getFrameData(char buff[MAX_FRAME], char data[MAX_DATA]){
	int i;
	int j = 0;
	memset(data, 0, MAX_DATA);
	for(i = 15; i < MAX_FRAME; i++){
		data[j] = buff[i];
		j++;
	}
}

// sendMessage: funció encarregada d'enviar els missatges entre les maquines.
void sendMessage(int socketFD, char origin[], char destination[], char type, char data[]){
	int i;
	int j;
	char frame[MAX_FRAME];
	memset(frame, 0, MAX_FRAME);
	for(i = 0; i < 7; i++){
		frame[i] = origin[i];
	}
	j = 0;
	for(i = 7; i < 14; i++){
		frame[i] = destination[j];
		j++;
	}
	frame[14] = type;
	j = 0;
	for(i = 15; i < MAX_FRAME; i++){
		frame[i] = data[j];
		j++;
	}
	write(socketFD, frame, MAX_FRAME);
}

// clientProtocol: funció encarregada del protocol de conexió del client.
int clientProtocol(int socketFD, Client u){
	char message[MAX];
	char data[MAX_DATA];
	char buff[MAX_FRAME];
	sendMessage(socketFD, u.name, "server", 'P', u.passwordMD5);
	read(socketFD, buff, MAX_FRAME);
	getFrameData(buff, data);
	if(buff[14] == 'E'){
		sendMessage(socketFD, u.name, "server", 'E', "Error confirmat");
		sprintf(message, "Connexio fallida: %s\n", data);
		writeMsg(message);
		return ERROR;
	}
	sprintf(message, "%s\n", data);
	writeMsg(message);
	sendMessage(socketFD, u.name, "server", 'C', "Connectat correctament");
	return OK;
}

// activeSocket: funció que crea el socket del client i fa el protocol de connexió.
// Retorna ERROR o el file descriptor del socket si tot ha sortit bé.
int activeSocket(Connection c, Client u){
	struct sockaddr_in svrAddr;
	char ip[16];
	int port;
	int socketFD;
	if(hostnameToIp(c.ip , ip) == ERROR){
		return ERROR;
	}
	port = c.port;
	if(port < 1024 || port > 65535){		
		writeMsg("Port invalid!\n");
		return ERROR;
	}
	socketFD = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(socketFD < 0){		
		writeMsg("Error al crear el socket!\n");
		return ERROR;
	}
	memset(&svrAddr, 0, sizeof (svrAddr));
	svrAddr.sin_family = AF_INET;
	svrAddr.sin_port = htons (port);
	if (inet_aton (ip, &svrAddr.sin_addr) == 0){
		writeMsg("IP invalid !\n");
		return ERROR;
	}
	if (connect (socketFD, (void *) &svrAddr, sizeof (svrAddr)) < 0){
		writeMsg("Connexio fallida!\n");
		return ERROR;
	}
	if(clientProtocol(socketFD, u) < 0){
		return ERROR;
	}
	return socketFD;	
}

// pasiveSocket: fa tota la rutina de connexió del servidor.
// Retorna el file descriptor del socket, en cas d'error retorna un -1.
int pasiveSocket(Connection c){
	int socketFD;
	char message[MAX];
	uint16_t port;
	int tr = 1;
	struct sockaddr_in socketDirection;
	//Comprobamos que sea un puerto valido
	if (c.port < 1024 && c.port > 65535){
		sprintf(message, "Port invalid!\n");
		write(1, message, strlen (message));
		return ERROR;
	}
	port = c.port;
	socketFD = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (socketFD < 0){
		writeMsg("Error al crear el socket!\n");
		return ERROR;
	}
	bzero (&socketDirection, sizeof(socketDirection));
	socketDirection.sin_family = AF_INET;
	socketDirection.sin_port = htons(port);
	socketDirection.sin_addr.s_addr = INADDR_ANY;
	if(setsockopt(socketFD, SOL_SOCKET, SO_REUSEADDR, &tr, sizeof (int)) == ERROR){
		perror("setsockopt");
		return ERROR;
	}
	if (bind(socketFD, (void *) &socketDirection, sizeof(socketDirection)) < 0){
		writeMsg("Error al vincular el socket i la direccio!\n");
		//Cerramos el socket
		close(socketFD);
		return ERROR;
	}
	listen (socketFD, 5);
	writeMsg("Servidor preparat!\n\n\n");
	return socketFD;
}

// getName: recupera el nom del origen de la trama.
void getName(char name[8], char buff[MAX_FRAME]){
	int i;
	memset(name, 0, 8);
	for(i = 0; i < 8; i++){
		name[i] = buff[i];
	}
}

// getPassword: recupera el password que està guardad en el camp dada de la trama.
void getPassword(char pass[33], char buff[MAX_FRAME]){
	int i;
	int j = 0;
	memset(pass, 0, 33);
	for(i = 15; i < 47; i++){
		pass[j] = buff[i];
		j++;
	}
}

// serverProtocol: funció encarregada del protocol de conexió del servidor.
int serverProtocol(UserList l, int socketClient){
	char name[8];
	char buff[MAX_FRAME];
	char data[MAX_DATA];
	char message[MAX];
	char mander[33]; //eastern egg, si no ho pilles no has tingut infancia 
	User node;
	read(socketClient, buff, MAX_FRAME);
	getName(name, buff);
	if(searchName(l, name, &node) < 0){
		sendMessage(socketClient, "server", name, 'E', "Error d’autentificacio");
		read(socketClient, buff, MAX_FRAME);
		if(buff[14] == 'E'){
			getFrameData(buff, data);
			sprintf(message, "%s\n", data);
			writeMsg(message);
		}else{
			writeMsg("Error no confirmat\n");
		}
		return ERROR;
	}
	//Comprovo si el client ja està connectat, si ho està, es finalitza l'autentificació amb error.
	if(getSocket(l, name) > 0){
		sendMessage(socketClient, "server", name, 'E', "Error d’autentificacio, client ja connectat");
		read(socketClient, buff, MAX_FRAME);
		if(buff[14] == 'E'){
			getFrameData(buff, data);
			sprintf(message, "%s\n", data);
			writeMsg(message);
		}else{
			writeMsg("Error no confirmat\n");
		}
		return ERROR;
	}
	getPassword(mander, buff);
	if(strcmp(mander, node.passwordMD5) != 0){
		sendMessage(socketClient, "server", name, 'E', "Error d’autentificacio");
		read(socketClient, buff, MAX_FRAME);
		if(buff[14] == 'E'){
			getFrameData(buff, data);
			sprintf(message, "%s\n", data);
			writeMsg(message);
		}else{
			writeMsg("Error no confirmat\n");
		}
		return ERROR;
	}
	sendMessage(socketClient, "server", name, 'A', "Autentificacio correcte");
	read(socketClient, buff, MAX_FRAME);
	if(buff[14] == 'E'){
		writeMsg("Error al confirmar\n");
		return ERROR;
	}
	//Fiquem el socket del client.
	setSocket(l, name, socketClient);
	getFrameData(buff, data);
	sprintf(message, "%s\nConnectat amb %s\n", data, name);
	writeMsg(message);
	return OK;
}
