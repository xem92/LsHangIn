/*
*
* Nom: Joan Guillem Castell Ros-Zanet login:ls23841
* Nom: Arnau Perramon login:ls26027
*
* File: RoomsList.h
* Arxiu .H on guardem els nous tipus per crear la llista i
* les funcions públiques del nostre TAD LinkedList.
*/

//Tipus Room: guardem la informació de la sala.
//name: nom de la sala.
//admin: nom del administrador.
//maxUsers: nombre màxim d'usuaris.
//HTMLfd: file descriptor del fitxer HTML de la sala.
//users: es guarda els sockets dels usuaris que hi son a la sala.
//*next: punter al següent node, és a dir, punter a la informació de la següent sala.
typedef struct room {
	char name[32];
	char admin[8];
	int maxUsers;
	int HTMLfd;
	int *users;
	struct room *next;
}Room;

//Tipus RoomList: conté els punters als nodes d'informació
//*first: punter al primer node d'informació
//*pdi: punt d'interés, punter que apunta a un node d'informació
typedef struct roomList {
	Room *first;
	Room *pdi;
}RoomList;

/*Funcions de la LinkedList*/

// roomsGoToFirst: funció que fica el pdi al primer lloc de la llista.
void roomsGoToFirst(RoomList *list);

// roomsInsertNode: funció on inserim un nou node. Retorna -1 si no hi ha suficient memoria i 
// retorna 1 si s'ha inserit correctament el node.
int roomsInsertNode(RoomList *list, Room newNode);

// roomsCreateList: funció per demanar memoria per la llista, i crea el primer node,
// que serà el node fantasma. Retorna -1 si no hi ha suficient memoria i retorna 1 
// si s'ha creat correctament la llista.
int roomsCreateList(RoomList *list);

// roomsSearchName: selecciona la informació de la llista amb el mateix nom i la volca al punter del node. 
// Retorna 1 si ha trobat informació i si retorna -1 no ha trobat res en la llista. 
int roomsSearchName(RoomList list, char name[32]);

// roomSetUser: inicialitza el usuari a l'array.
int roomsSetUser(RoomList list, char name[32], int socket);

// roomsResetUser: reseteja el usuari a l'array d'enters
int roomsResetUser(RoomList list, char name[32], int socket);

// roomsGetRoom: recuera la informació d'una sala.
// Retorna -1 si la sala no existeix.
int roomsGetRoom(RoomList list, char name[32], Room *room);

// roomGetAdmin: comprova si existeix la sala y retorna el seu administrador.
int roomGetAdmin(RoomList rooms, char name[32], char admin[8]);

// roomsModifyRoomByName: modifica el màxim numero d'usuaris de la sala escollida.
int roomsModifyRoomByName(RoomList *list, char name[32], int newMax);

// roomsDeleteNodeByName: busca el node que volem eliminar pel nom.
// Rep la LinkedList i el nom del node que volem eliminar.
// Retorna 1 si ha trobat i eliminat el node, retorna -1 si no ha trobat el node.
int roomsDeleteNodeByName(RoomList *list, char name[32]);

// roomsDeleteList: funció per esborrar la llista.
void roomsDeleteList(RoomList *list);

// nextNode: funció que mou el punt d'interes al següent node.
void roomsNextNode(RoomList *list);
