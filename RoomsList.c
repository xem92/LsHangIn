/*
*
* Nom: Joan Guillem Castell Ros-Zanet login:ls23841
* Nom: Arnau Perramon login:ls26027
*
* File: RoomsList.c
* En aquest .C tindrem implementades totes les funcions que haurà de fer
* la nostra LinkedList amb punt d'interés.
*
*/

/*Includes*/
#include "lib.h"

// roomsGoToFirst: funció que fica el pdi al primer lloc de la llista.
void roomsGoToFirst(RoomList *list){
	list->pdi = list->first;
}

// roomsNextNode: funció que mou el punt d'interes al següent node.
void roomsNextNode(RoomList *list){
	list->pdi = list->pdi->next;
}

// roomsInsertNode: funció on inserim un nou node. Retorna -1 si no hi ha suficient memoria i 
// retorna 1 si s'ha inserit correctament el node.
int roomsInsertNode(RoomList *list, Room newNode){
	Room *aux; // aux: node auxiliar on demanarem memoria per al nou node.
	aux = (Room*) malloc(sizeof(Room));
	if(aux == NULL){
		writeMsg("Error al demanar memoria per el nou node \n");
		return ERROR;
	}
	strcpy(aux->name, newNode.name);
	strcpy(aux->admin, newNode.admin);
	aux->maxUsers = newNode.maxUsers;
	aux->users = (int*) malloc(sizeof(int)*newNode.maxUsers);
	aux->HTMLfd = newNode.HTMLfd;
	int i;
	for(i = 0; i < aux->maxUsers; i++){
		aux->users[i] = -1;
	}
	aux->next = list->first->next;
	list->first->next = aux;
	list->pdi = list->first->next;
	return OK;
}

// roomsCreateList: funció per demanar memoria per la llista, i crea el primer node,
// que serà el node fantasma. Retorna -1 si no hi ha suficient memoria i retorna 1 
// si s'ha creat correctament la llista.
int roomsCreateList(RoomList *list){
	list->first = (Room*) malloc(sizeof(Room)); // Demanem memòria per el node fantasma.
	if(list->first == NULL){
		writeMsg("Error al demanar memoria per el node fantasma\n");
		return ERROR;
	}
	// Omplim les dades del node fantasma.
	strcpy(list->first->name, "FANTAS\0");
	strcpy(list->first->admin, "FANTAS\0");
	list->first->maxUsers = 0;
	list->first->HTMLfd = -1;
	list->first->users = NULL;
	list->first->next = NULL;
	// Fiquem de moment pdi a NULL.
	list->pdi = NULL;
	return OK;	
}

// roomsSearchName: selecciona la informació de la llista amb el mateix nom.
// Retorna 1 si ha trobat informació i si retorna -1 no ha trobat res en la llista.
int roomsSearchName(RoomList list, char name[32]){
	roomsGoToFirst(&list);
	while(list.pdi != NULL){
		if(strcmp(list.pdi->name, name) == 0){
			return OK;
		}
		roomsNextNode(&list);
	}
	return ERROR;
}

// roomSetUser: introdueix un nou usuari a l'array' d'enters.
int roomsSetUser(RoomList list, char name[32], int socket){
	roomsGoToFirst(&list);
	int i;
	while(list.pdi != NULL){
		if(strcmp(list.pdi->name, name) == 0){
			for(i = 0; i < list.pdi->maxUsers; i++){
				if(list.pdi->users[i] == socket){
					return -2; // ja està connectat.
				}
			}
			for(i = 0; i < list.pdi->maxUsers; i++){
				if(list.pdi->users[i] == -1){
					list.pdi->users[i] = socket;
					return OK;
				}
			}
			return -3; // està ple.
		}
		roomsNextNode(&list);
	}
	return ERROR; // no existeix la sala.
}

// roomsResetUser: reseteja el usuari a l'array d'enters ficant -1.
int roomsResetUser(RoomList list, char name[32], int socket){
	roomsGoToFirst(&list);
	int i;
	while(list.pdi != NULL){
		if(strcmp(list.pdi->name, name) == 0){
			for(i = 0; i < list.pdi->maxUsers; i++){
				if(list.pdi->users[i] == socket){
					list.pdi->users[i] = -1;
					return OK;
				}
			}
			return -2; // no està connectat a la sala.
		}
		roomsNextNode(&list);
	}
	return ERROR; //no existeix la sala.
}

// roomsGetRoom: recuera la informació d'una sala.
// Retorna -1 si la sala no existeix.
int roomsGetRoom(RoomList list, char name[32], Room *room){
	roomsGoToFirst(&list);
	int i;
	while(list.pdi != NULL){
		if(strcmp(list.pdi->name, name) == 0){
			strcpy(room->admin, list.pdi->admin);
			strcpy(room->name, list.pdi->name);
			room->maxUsers = list.pdi->maxUsers;
			room->HTMLfd = list.pdi->HTMLfd;
			room->users = (int*) malloc(sizeof(int)*list.pdi->maxUsers);
			for(i = 0; i < room->maxUsers; i++){
				room->users[i] = list.pdi->users[i];
			}
			room->next = NULL;
			return OK;
		}
		roomsNextNode(&list);
	}
	return ERROR;
}

// roomGetAdmin: comprova si existeix la sala y retorna el seu administrador.
int roomGetAdmin(RoomList list, char name[32], char admin[8]){
	roomsGoToFirst(&list);
	while(list.pdi != NULL){
		if(strcmp(list.pdi->name, name) == 0){
			strcpy(admin, list.pdi->admin);
			return OK;
		}
		roomsNextNode(&list);
	}
	return ERROR;
}

// roomsModifyRoomByName: modifica el màxim numero d'usuaris de la sala escollida.
int roomsModifyRoomByName(RoomList *list, char name[32], int newMax){
	int i;
	roomsGoToFirst(list);
	while(list->pdi != NULL){
		if(strcmp(list->pdi->name, name) == 0){
			free(list->pdi->users);
			list->pdi->users = (int*) malloc(sizeof(int)*newMax);
			for(i = 0; i < newMax; i++){
				list->pdi->users[i] = -1;
			}
			list->pdi->maxUsers = newMax;
			return OK;
		}
		roomsNextNode(list);
	}
	return ERROR;
}

// roomsDeleteNode: elimina el node que apunta el punter next del pdi.
// Rep la LinketList apuntant al node abans del node que volem eliminar.
void roomsDeleteNode(RoomList *list){
	Room *temp; // temp: node temporal
	int i;
	temp = list->pdi->next;
	list->pdi->next = temp->next;
	//Tanco tots els files descriptors dels sockets dels usuaris.
	for(i = 0; i< temp->maxUsers; i++){
		if(temp->users[i] != -1){
			close(temp->users[i]);
		}
	}
	close(temp->HTMLfd);
	free(temp->users);
	free(temp);
}

// roomsDeleteNodeNoClose: elimina el node que apunta el punter next del pdi.
// Rep la LinketList apuntant al node abans del node que volem eliminar. No tanca 
// els file descriptors dels sockets dels usuaris.
void roomsDeleteNodeNoClose(RoomList *list){
	Room *temp; // temp: node temporal
	temp = list->pdi->next;
	list->pdi->next = temp->next;
	close(temp->HTMLfd);
	free(temp->users);
	free(temp);
}

// roomsDeleteNodeByName: busca el node que volem eliminar pel nom.
// Rep la LinkedList i el nom del node que volem eliminar.
// Retorna 1 si ha trobat i eliminat el node, retorna -1 si no ha trobat el node.
int roomsDeleteNodeByName(RoomList *list, char name[32]){
	roomsGoToFirst(list);
	while(list->pdi != NULL){
		if(strcmp(list->pdi->next->name, name) == 0){
			roomsDeleteNodeNoClose(list);
			list->pdi = list->first;
			return OK;
		}
		list->pdi = list->pdi->next;
	}
	list->pdi = list->first;
	return ERROR;
}

// roomsDeleteList: funció per esborrar tota la llista.
// Rep la LinkedList que volem eliminar.
void roomsDeleteList(RoomList *list){
	Room *temp; // temp: node temporal
	while(list->first != NULL){
		temp = list->first;
		list->first = list->first->next;
		free(temp);
	}
	list->first = NULL;
	list->pdi = NULL;
}
