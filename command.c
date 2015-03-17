/*
*
* Nom: Joan Guillem Castell Ros-Zanet login:ls23841
* Nom: Arnau Perramon login:ls26027
*
* File: comand.c
* Codi per facilitar el funcionament de les comandes.
*
*/

/*Includes*/
#include "lib.h"

// selectData: funció encarregada de separar la informacó rebude per la linea de comanda.
void selectData(char data[MAX_DATA], char dataP[MAX_DATA], char dataS[MAX_DATA]){
	int i = 0;
	int j = 0;
	memset(dataP, 0, MAX_DATA);
	memset(dataS, 0, MAX_DATA);
	while(data[i] != ':' && data[i] != '\0' && i < 100){
		dataP[i] = data[i];
		i++;
	}
	i++;
	if(data[i] != '\0' && i < 100){
		while(data[i] != ':' && data[i] != '\0' && i < 100){
			dataS[j] = data[i];
			i++;
			j++;
		}
	}
}

// getData: extraiem la informació que ens ve desde la linea de comandes.
// Rebem la linea per parametre la variable on volem guardar la informació, la cadena amb la 
// line de comanda que ens ha introduït l'usuari i apartir d'on hem de començar a 
// extrure les dades.
int getData(CommandLine *c, char *command, int position){
	int i = 0; // i: contardor.
	c->dataP = (char*)malloc(sizeof(char));
	position++;
	while(command[position] != '>' && command[position] != '\0'){
		c->dataP[i] = command[position];
		i++;
		position++;
		c->dataP = (char*)realloc(c->dataP, sizeof(char)*(i+1));
	}
	c->dataP[i] = '\0';
	if(command[position] == '\0'){
		return ERROR;
	}
	position++;
	if(command[position] == ':'){
		i = 0;
		c->dataS = (char*)malloc(sizeof(char));
		while(command[position] != '>' && command[position] != '\0'){
			if(command[position] != '<' && command[position] != ':'){
				c->dataS[i] = command[position];
				i++;
				c->dataS = (char*)realloc(c->dataS, sizeof(char)*(i+1));
			}
			position++;
		}
		c->dataS[i] = '\0';
		if(command[position] == '\0'){
			return ERROR;
		}
	}else{
		c->dataS = (char*)malloc(sizeof(char));
		c->dataS[0] = '\0';
	}
	return 1;
}

// getCommandName: funció on extraiem el nom de la comanda introduida pel usuari.
// Rebem la linea per parametre la variable on volem guardar la informació i la cadena amb la 
// line de comanda que ens ha introduït l'usuari.
int getCommandName(CommandLine *c, char *command){
	int i = 0; // i: contardor.
	while(command[i] != '<' && command[i] != '\0' && i < 12){
		c->commandName[i] = command[i];
		i++;
	}
	if(i == 12){
		return ERROR;
	}
	c->commandName[i] = '\0';
	return i;
}

void getMissageData(char com[MAX_FRAME], char data[102]){
	int nb, i;
	i = 0;
	for(nb = 15; nb < 115 && com[nb] != '\0'; nb++){
		data[i] = com[nb];
		i++;
	}
	data[i] = '\n';
	data[i+1] = '\0';
}

// setCommand: esperem a que el usuari introdueixi la comanda
// per la linea de comanda.
int setCommand(CommandLine *c, int socketFD){
	int position;
	int error = -1;
	char com[2048];
	char *buff;
	char data[102];
	char dataP[MAX_DATA];
	char dataS[MAX_DATA];
	char origin[8];
	char talkMsg[256];
	int readLen;
	int nb;
	fd_set set;
	bzero(com, 2048);
	write(1, "\n$", 2);
	FD_ZERO(&set);
	FD_SET(0, &set);
	FD_SET(socketFD, &set);
	select (16, &set, NULL, NULL, NULL);
	if (FD_ISSET(0, &set)){
		ioctl(STDIN_FILENO, FIONREAD, &nb);
		buff = malloc(nb+1);
		read(0, buff, nb);
		buff[nb-1] = '\0';
		position = getCommandName(c, buff);
		if(position < 0){
			return error;
		}
		if(buff[position] == '<'){
			error = getData(c, buff, position);
		}else if(buff[position] == '\0'){
			c->dataP = (char*)malloc(sizeof(char));
			c->dataS = (char*)malloc(sizeof(char));
			c->dataP[0] = '\0';
			c->dataS[0] = '\0';
			error = 1;
		}
		free(buff);
	}else if(FD_ISSET(socketFD, &set)){
		readLen = read(socketFD, com, MAX_FRAME);
		if(readLen < MAX_FRAME){
			close(socketFD);
			writeMsg("Error: servidor desconectat\n");
			exit(ERROR);
		}
		if(com[14] == 'T'){
			getMissageData(com, data);
			selectData(data, dataP, dataS);
			getName(origin, com);
			sprintf(talkMsg, "[%s] %s: %s", dataP, origin, dataS);
			writeMsg(talkMsg);
		}
		error = 2;
	}
	return error;
}

// connectToServer: Fa tota al part de la rutina de conexió.
// Retorna el file descriptor del socket que hem iniciat.
int connectToServer(CommandLine *c, Client user){
	Connection connection;
	int socketFD;
	if(strcmp(c->commandName, "connect") == 0){
		if(c->dataP[0] != '\0' && c->dataS[0] != '\0'){
			strcpy(connection.ip, c->dataS);
			connection.port = atoi(c->dataP);
			socketFD = activeSocket(connection, user);
			return socketFD;
		}else{
			writeMsg("Comanda erronea\n");
			return ERROR;
		}
	}else{
		writeMsg("Esperant la comanda de connexio...\n");
		return ERROR;
	}
}

// doTalk: Envia missatge a una sala.
void doTalk(int socketFD, Client user, CommandLine *c){
	char data[MAX_DATA];
	char dataP[MAX_DATA];
	char buff[MAX_FRAME];
	int dataLength;
	int i, j, ex, a;
	bzero(data, MAX_DATA);
	dataLength = strlen(c->dataP)+strlen(c->dataS)+strlen(user.name)+5;
	if(dataLength > MAX_DATA){
		sprintf(dataP, "%s:", c->dataP);
		ex = 0;
		i = 0;
		bzero(data, MAX_DATA);
		sprintf(data, dataP);
		while(ex<1){
			for(j = strlen(dataP); j<MAX_DATA; j++){
				a = strlen(c->dataS);
				if(i < a){
					data[j] = c->dataS[i];
				}else{
					data[j] = '\0';
					ex = 1;
				}
				i++;
			}
			data[strlen(data)] = '\0';
			sendMessage(socketFD, user.name, "server", 'T', data);
			read(socketFD, buff, MAX_FRAME);
			if(buff[14] == 'E'){
				bzero(data, MAX_DATA);
				getFrameData(buff, data);
				data[strlen(data)-1] = '\n';
				writeMsg(data);
			}
		}
	}else{
		sprintf(data, "%s:%s", c->dataP, c->dataS);
		sendMessage(socketFD, user.name, "server", 'T', data);	
		read(socketFD, buff, MAX_FRAME);
		if(buff[14] == 'E'){
			bzero(data, MAX_DATA);
			getFrameData(buff, data);
			data[strlen(data)-1] = '\n';
			writeMsg(data);
		}
	}
}

// doCreateRoom: envia trama per demanar la creació d'una sala.
void doCreateRoom(int socketFD, Client user, CommandLine *c){
	char data[MAX_DATA];
	char buff[MAX_FRAME];
	char message[MAX];
	int dataPlemgth;
	dataPlemgth = strlen(c->dataP);
	if(dataPlemgth < 32){
		bzero(data, MAX_DATA);
		sprintf(data, "%s:%s", c->dataP, c->dataS);
		sendMessage(socketFD, user.name, "server", 'Z', data);
		read(socketFD, buff, MAX_FRAME);
		if(buff[14] == 'O'){
			sprintf(message, "Sala %s creada correctament.\n", c->dataP);
			write(1, message, strlen(message));
		}else{
			bzero(data, MAX_DATA);
			getFrameData(buff, data);
			data[strlen(data)-1] = '\n';
			writeMsg(data);
		}
	}else{
		writeMsg("El nom de la sala es massa llarg.\n");
	}

}

// doEnterRoom: envia missatge per entrar a una sala.
void doEnterRoom(int socketFD, Client user, CommandLine *c){
	char data[MAX_DATA];
	char buff[MAX_FRAME];
	char message[MAX];
	bzero(data, MAX_DATA);
	bzero(message, MAX);
	sprintf(data, "%s", c->dataP);
	sendMessage(socketFD, user.name, "server", 'N', data);
	read(socketFD, buff, MAX_FRAME);
	if(buff[14] == 'O'){
		sprintf(message, "Has entrat a la sala %s.\n", c->dataP);
		write(1, message, strlen(message));
	}else{
		bzero(data, MAX_DATA);
		getFrameData(buff, data);
		data[strlen(data)-1] = '\n';
		writeMsg(data);
	}
}

// doModifyRoom: envia missatge per demanar modificar una sala.
void doModifyRoom(int socketFD, Client user, CommandLine *c){
	char data[MAX_DATA];
	char buff[MAX_FRAME];
	char message[MAX];
	sprintf(data, "%s:%s", c->dataP, c->dataS);
	sendMessage(socketFD, user.name, "server", 'M', data);
	read(socketFD, buff, MAX_FRAME);
	if(buff[14] == 'O'){
		sprintf(message, "Sala %s modificada correctament.\n", c->dataP);
		write(1, message, strlen(message));
	}else{
		bzero(data, MAX_DATA);
		getFrameData(buff, data);
		data[strlen(data)-1] = '\n';
		writeMsg(data);
	}
}

// doExirRoom: envia missatge per sortir d'una sala.
void doExitRoom(int socketFD, Client user, CommandLine *c){
	char data[MAX_DATA];
	char buff[MAX_FRAME];
	char message[MAX];
	bzero(data, MAX_DATA);
	bzero(message, MAX);
	sprintf(data, "%s", c->dataP);
	sendMessage(socketFD, user.name, "server", 'X', data);
	read(socketFD, buff, MAX_FRAME);
	if(buff[14] == 'O'){
		sprintf(message, "Has sortit de la sala %s.\n", c->dataP);
		write(1, message, strlen(message));
	}else{
		bzero(data, MAX_DATA);
		getFrameData(buff, data);
		data[strlen(data)-1] = '\n';
		writeMsg(data);
	}
}

// doShowRooms: envia missatge per demanar la informació de les sales.
void doShowRooms(int socketFD, Client user){
	char buff[MAX_FRAME];
	char message[MAX];
	char data[MAX_DATA];
	sendMessage(socketFD, user.name, "server", 'S', "Mostra sales\0");
	bzero(buff, MAX_FRAME);
	read(socketFD, buff, MAX_FRAME);
	writeMsg("Sales dispoibles:\n");
	while(buff[14] == 'S'){
		getFrameData(buff, data);
		sprintf(message, "\t%s\n", data);
		writeMsg(message);
		bzero(buff, MAX_FRAME);
		read(socketFD, buff, MAX_FRAME);
	}
}

// doDeleteRoom: envia missatge per eliminar una sala.
void doDeletRoom(int socketFD, Client user, CommandLine *c){
	char data[MAX_DATA];
	char buff[MAX_FRAME];
	char message[MAX];
	bzero(data, MAX_DATA);
	sprintf(data, "%s", c->dataP);
	sendMessage(socketFD, user.name, "server", 'D', data);
	read(socketFD, buff, MAX_FRAME);
	if(buff[14] == 'O'){
		sprintf(message, "Sala %s eliminada correctament.\n", c->dataP);
		write(1, message, strlen(message));
	}else{
		bzero(data, MAX_DATA);
		getFrameData(buff, data);
		data[strlen(data)-1] = '\n';
		writeMsg(data);
	}
}

// doCommand: s'executa la comanda introduida per linea de 
// comanda.
void doCommand(CommandLine *c, Client user, int socketFD){
	if(strcmp(c->commandName, "talk") == 0){
		//writeMsg("Comanda T\n");
		if(c->dataP[0] != '\0' && c->dataS[0] != '\0'){
			doTalk(socketFD, user, c);
		}else{
			writeMsg("Comanda error\n");
		}
	}else if(strcmp(c->commandName, "show_rooms") == 0){
		if(c->dataP[0] != '\0' && c->dataS[0] != '\0'){
			writeMsg("Comanda error\n");
		}else{
			doShowRooms(socketFD, user);
			//writeMsg("Comanda S\n");
		}
	}else if(strcmp(c->commandName, "create_room") == 0){
		if(c->dataP[0] != '\0' && c->dataS[0] != '\0'){
			doCreateRoom(socketFD, user, c);
			//writeMsg("Comanda Z\n");
		}else{
			writeMsg("Comanda error\n");
		}
	}else if(strcmp(c->commandName, "modify_room") == 0){
		if(c->dataP[0] != '\0' && c->dataS[0] != '\0'){
			doModifyRoom(socketFD, user, c);
			//writeMsg("Comanda M\n");
		}else{
			writeMsg("Comanda error\n");
		}
	}else if(strcmp(c->commandName, "enter_room") == 0){
		if(c->dataP[0] != '\0' && c->dataS[0] == '\0'){
			doEnterRoom(socketFD, user, c);
			//writeMsg("Comanda N\n");
		}else{
			writeMsg("Comanda error\n");
		}
	}else if(strcmp(c->commandName, "exit_room") == 0){
		if(c->dataP[0] != '\0' && c->dataS[0] == '\0'){
			doExitRoom(socketFD, user, c);
			//writeMsg("Comanda X\n");
		}else{
			writeMsg("Comanda error\n");
		}
	}else if(strcmp(c->commandName, "delete_room") == 0){
		if(c->dataP[0] != '\0' && c->dataS[0] == '\0'){
			doDeletRoom(socketFD, user, c);
			//writeMsg("Comanda D\n");
		}else{
			writeMsg("Comanda error\n");
		}
	}else if(strcmp(c->commandName, "disconnect") == 0){	// En l'enunciat de la pràctica no la demanen, però creiem que és útil per tancar la sessió del usuari
		if(c->dataP[0] != '\0' && c->dataS[0] != '\0'){
			writeMsg("Comanda error\n");
		}else{
			sendMessage(socketFD, user.name, "server", 'B', "Desconectar");
			writeMsg("Desconectat del servidor\n");
			close(socketFD);
			exit(1);
		}
	}else{
		writeMsg("Comanda no existent\n");
	}
	memset(c->commandName, 0, 12);
	if(c->dataP[0] != '\0'){
		free(c->dataP);
	}
	if(c->dataS[0] != '\0'){
		free(c->dataS);
	}
}

// countUsers: compte el nombre de clients que hi ha en la sala.
int countUsers(RoomList rooms){
	int i;
	int count = 0;
	for(i = 0; i < rooms.pdi->maxUsers; i++){
		if(rooms.pdi->users[i] > -1){
			count++;
		}
	}
	return count;
}

// serverTalk: s'envia el missatge de la comanda talk a la resta d'usuaris que hi són a la sala.
int serverTalk(char name[8], char data[MAX_DATA], RoomList rooms, int socketClient){
	char dataP[MAX_DATA];
	char dataS[MAX_DATA];
	int i;
	int cool = 0;
	Room room;
	selectData(data, dataP, dataS);
	roomsGetRoom(rooms, dataP, &room);
	for(i = 0; i < room.maxUsers; i++){
		if(room.users[i] == socketClient){
			cool = 1;
		}
	}
	if(cool == 0){
		return ERROR;
	}
	updateLog(room.HTMLfd, room.name, name, 'T', dataS);
	for(i = 0; i < room.maxUsers; i++){
		if(room.users[i] != -1 && room.users[i] != socketClient){
			sendMessage(room.users[i], name, "client", 'T', data);
		}
	}
	return OK;
}

// serverCommands: fa les commandes que li arriben.
int serverCommands(int socketClient, char buff[MAX_FRAME], RoomList rooms, pthread_mutex_t mtxCom){
	char data[MAX_DATA];
	char name[8];
	char admin[8];
	char dataP[MAX_DATA];
	char dataS[MAX_DATA];
	char message[256];
	int usersCount;
	Room room;
	bzero(name, 8);
	bzero(dataP, MAX_DATA);
	bzero(dataS, MAX_DATA);
	bzero(data, MAX_DATA);
	getName(name, buff);
	getFrameData(buff, data);
	selectData(data, dataP, dataS);
	sprintf(message, "\nOrigen:%s\nTipus:%c\nData1:%s\nData2:%s\n", name, buff[14], dataP, dataS);
	writeMsg(message);
	//mutex lock
	if(pthread_mutex_lock(&mtxCom) != 0){
		writeMsg("Mutex error\n");
		return ERROR;
	}
	if(buff[14] == 'S'){	//show_rooms
		roomsGoToFirst(&rooms);
		roomsNextNode(&rooms);
		while(rooms.pdi != NULL){
			usersCount = countUsers(rooms);
			bzero(data, MAX_DATA);
			sprintf(data, "[%s]-[%d/%d]", rooms.pdi->name, usersCount, rooms.pdi->maxUsers);
			sendMessage(socketClient, "server", name, 'S', data);
			roomsNextNode(&rooms);
		}
		sendMessage(socketClient, "server", name, 'O', "End");
	}else if(buff[14] == 'Z'){	//create_room
		if(roomsSearchName(rooms, dataP) != OK){
			strcpy(room.name, dataP);
			strcpy(room.admin, name);
			room.maxUsers = atoi(dataS);
			room.HTMLfd = initLog(dataP);
			if(roomsInsertNode(&rooms, room) == OK){
				sprintf(data, "Creacio de la sala %s.", room.name);
				updateLog(room.HTMLfd, room.name, name, 'Z', data);
				saveRooms(rooms);
				sendMessage(socketClient, "server", name, 'O', "Creat");
			}else{
				sendMessage(socketClient, "server", name, 'E', "Error al crear sala.\n");
			}
		}else{
			sendMessage(socketClient, "server", name, 'E', "Error al crear sala: sala ja existent.\n");
		}
	}else if(buff[14] == 'M'){	//modify_room
		if(roomGetAdmin(rooms, dataP, admin) == OK){
			if(strcmp(name, admin) == 0){
				roomsGetRoom(rooms, dataP, &room);
				updateLog(room.HTMLfd, room.name, admin, 'M', "Sala modificada.");
				roomsModifyRoomByName(&rooms, dataP, atoi(dataS));
				saveRooms(rooms);
				sendMessage(socketClient, "server", name, 'O', "Modify");
			}else{
				bzero(data, MAX_DATA);
				sprintf(data, "Error: No ets l’administrador de la sala %s.\n", dataP);
				sendMessage(socketClient, "server", name, 'E', data);
			}
		}else{
			bzero(data, MAX_DATA);
			sprintf(data, "Error: La sala %s no existeix.", dataP);
			sendMessage(socketClient, "server", name, 'E', data);
		}
	}else if(buff[14] == 'N'){	//enter_room
		roomsGetRoom(rooms, dataP, &room);
		switch(roomsSetUser(rooms, dataP, socketClient)){
			case OK:
				sprintf(data, "%s ha entrat a la sala.", name);
				updateLog(room.HTMLfd, room.name, name, 'N', data);
				sendMessage(socketClient, "server", name, 'O', "Enter");
				break;
			case -1:
				sendMessage(socketClient, "server", name, 'E', "Error: la sala no existeix.\n");
				break;
			case -2:
				sendMessage(socketClient, "server", name, 'E', "Erros: ja estas connectat.\n");
				break;
			case -3:
				sendMessage(socketClient, "server", name, 'E', "Error: sala plena.\n");
				break;
			default:
				sendMessage(socketClient, "server", name, 'E', "Error: no s'ha pogut entrar.\n");
				break;
		}
	}else if(buff[14] == 'T'){	//talk
		if(serverTalk(name, data, rooms, socketClient) < 0){
			bzero(data, MAX_DATA);
			sprintf(data, "No ets a la sala.\n");
			sendMessage(socketClient, "server", name, 'E', data);
		}else{
			bzero(data, MAX_DATA);
			sprintf(data, "OK");
			sendMessage(socketClient, "server", name, 'O', data);
		}
	}else if(buff[14] == 'X'){	//exit_room
		roomsGetRoom(rooms, dataP, &room);
		switch(roomsResetUser(rooms, dataP, socketClient)){
			case OK:
				sprintf(data, "%s ha sortit a la sala.", name);
				updateLog(room.HTMLfd, room.name, name, 'X', data);
				sendMessage(socketClient, "server", name, 'O', "Exit");
				break;
			case ERROR:
				sendMessage(socketClient, "server", name, 'E', "Error: la sala no existeix.\n");
				break;
			case -2:
				sendMessage(socketClient, "server", name, 'E', "Erros: no estas connectat.\n");
				break;
			default:
				sendMessage(socketClient, "server", name, 'E', "Error: no s'ha pogut entrar.\n");
				break;
		}
	}else if(buff[14] == 'D'){	//delete_room
		bzero(admin, 8);
		roomsGetRoom(rooms, dataP, &room);
		if(roomGetAdmin(rooms, dataP, admin) == OK){
			if(strcmp(name, admin) == 0){
				updateLog(room.HTMLfd, room.name, name, 'D', "Sala eliminada");
				roomsDeleteNodeByName(&rooms, dataP);
				saveRooms(rooms);
				sendMessage(socketClient, "server", name, 'O', "Elimina sala\n");
			}else{
				bzero(data, MAX_DATA);
				sprintf(data, "Error: No ets l’administrador de la sala %s.\n", dataP);
				sendMessage(socketClient, "server", name, 'E', data);
			}
		}else{
			bzero(data, MAX_DATA);
			sprintf(data, "Error: La sala %s no existeix.\n", dataP);
			sendMessage(socketClient, "server", name, 'E', data);
		}
	}else if(buff[14] == 'B'){	//disconnect
		return 1;
	}
	//mutex unlock
	if(pthread_mutex_unlock(&mtxCom) != 0){
		writeMsg("Mutex error");
		return ERROR;
	}
	return 0;
}
