/*
*
* Nom: Joan Guillem Castell Ros-Zanet login:ls23841
* Nom: Arnau Perramon login:ls26027
*
* File: sockets.c
* Funcions per els sockets.
*
*/

// pasiveSocket: fa tota la rutina de connexió del servidor.
// Retorna el file descriptor del socket, en cas d'error retorna un -1.
int pasiveSocket(Connection c);

// activeSocket: funció que crea el socket del client i fa el protocol de connexió.
// Retorna ERROR o el file descriptor del socket si tot ha sortit bé.
int activeSocket(Connection c, Client u);

// serverProtocol: funció encarregada del protocol de conexió del servidor.
int serverProtocol(UserList l, int socketClient);

// sendMessage: funció encarregada d'enviar els missatges entre les maquines.
void sendMessage(int socketFD, char origin[], char destination[], char type, char data[]);

// getName: recupera el nom del origen de la trama.
void getName(char name[8], char buff[MAX_FRAME]);

//getFrameData: recupera el camp de dada d'una trama.
void getFrameData(char buff[MAX_FRAME], char data[MAX_DATA]);
