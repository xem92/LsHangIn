/*
*
* Nom: Joan Guillem Castell Ros-Zanet login:ls23841
* Nom: Arnau Perramon login:ls26027
*
* File: auxiliar.c
* Funcions auxiliars.
*
*/

// writeMsg: escriu un missatge per pantalla.
void writeMsg(char buff[MAX]);

// getFD: funció on obtenim el file descriptor del fitxer que volem obrir.
int getFD(char fileName[11]);

// saveRooms: guarda les dades de les sales en el fitxer "rooms.dat".
int saveRooms(RoomList rooms);
