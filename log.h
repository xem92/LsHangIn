/*
*
* Nom: Joan Guillem Castell Ros-Zanet login:ls23841
* Nom: Arnau Perramon login:ls26027
*
* File: log.h
* Arxiu .H amb les funcions del log.
*/

// initLog: crea/obra un fitxer .html per al diari de la sala.
// Retorna el file descriptor.
int initLog(char name[40]);

// updateLog: actualitza el fitxer amb la nova informació.
void updateLog(int fd, char name[32], char user[8], char type, char data[100]);
