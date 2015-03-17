/*
*
* Nom: Joan Guillem Castell Ros-Zanet login:ls23841
* Nom: Arnau Perramon login:ls26027
*
* File: config.h
* Estructures de la informació dels fitxers.
*
*/

// Tipus Client: guardem les dades del client.
// name: nom del usuari
// password: contrasenya del usuari
typedef struct client{
	char name[8];
	char passwordMD5[33];
} Client;

// Tipus Connection: guardem les dades del server.
// ip: cadena de caracter amb la ip.
// port: enter amb el numero del port.
typedef struct connection{
	char ip[256];
	int port;
} Connection;
