/*
*
* Nom: Joan Guillem Castell Ros-Zanet login:ls23841
* Nom: Arnau Perramon login:ls26027
*
* File: UserList.h
* Arxiu .H on guardem els nous tipus per crear la llista i
* les funcions públiques del nostre TAD LinkedList.
*/

//Tipus User: guardem la informació del usuari.
//name: nom del usuari.
//passwordMD5: password encriptat a MD5.
//id: identificador numèric del usuari.
// socket: socket del usuari, si no està connectat valdrà -1.
//*next: punter al següent node, és a dir, punter a la informació del següent usuari.
typedef struct user {
	char name[8];
	char passwordMD5[33];
	int id;
	int socket;
	struct user *next;
}User;

//Tipus LinkedList: conté els punters als nodes d'informació.
//*first: punter al primer node d'informació.
//*pdi: punt d'interés, punter que apunta a un node d'informació.
typedef struct list {
	User *first;
	User *pdi;
}UserList;

/*Funcions de la LinkedList*/

// goToFirst: funció que fica el pdi al primer lloc de la llista.
void goToFirst(UserList *list);

// insertList: funció on inserim un nou node. Retorna -1 si no hi ha suficient memoria i 
// retorna 1 si s'ha inserit correctament el node.
int createList(UserList *list);

// createList: funció per demanar memoria per la llista, i crea el primer node,
// que serà el node fantasma. Retorna -1 si no hi ha suficient memoria i retorna 1 
// si s'ha creat correctament la llista.
int insertNode(UserList *list, User newNode);

// searchId: selecciona la informació de la llista amb el mateix ID i la volca al punter del node. 
// Retorna 1 si ha trobat informació i si retorna -1 no ha trobat res en la llista. 
int searchId(UserList list, int id, User *node);

// searchName: selecciona la informació de la llista amb el mateix nom i la volca al punter del node. 
// Retorna 1 si ha trobat informació i si retorna -1 no ha trobat res en la llista. 
int searchName(UserList list, char name[8], User *node);

// getIDByName: busca l'ID del usuari  a partir del seu nom.
int getIDByName(UserList list, char name[8]);

// getNameBySocket: busca el nom del usuari a partir del seu socket.
int getNameBySocket(UserList list, int socket, char name[8]);

// deleteNodeByName: busca el node que volem eliminar pel nom.
// Rep la LinkedList i el nom del node que volem eliminar.
// Retorna 1 si ha trobat i eliminat el node, retorna -1 si no ha trobat el node.
int deleteNodeByName(UserList *list, char name[8]);

// getSocket: busca el socket del usuari.
// Retorna el socket del usuari a partir de seu nom.
int getSocket(UserList list, char name[8]);

// setSocket: actualitza el socket del usuari.
void setSocket(UserList list, char name[8], int socket);

// deleteNodeById: busca el node que volem eliminar pel numero identificador.
// Rep la LinkedList i el numero identificador del node que volem eliminar.
// Retorna 1 si ha trobat i eliminat el node, retorna -1 si no ha trobat el node.
int deleteNodeById(UserList *list, int id);

// deleteList: funció per esborrar la llista.
void deleteList(UserList *list);

// nextNode: funció que mou el punt d'interes al següent node.
void nextNode(UserList *list);
