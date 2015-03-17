/*
*
* Nom: Joan Guillem Castell Ros-Zanet login:ls23841
* Nom: Arnau Perramon login:ls26027
*
* File: UserList.c
* En aquest .C tindrem implementades totes les funcions que haurà de fer
* la nostra LinkedList amb punt d'interés.
*
*/

/*Includes*/
#include "lib.h"

// goToFirst: funció que fica el pdi al primer lloc de la llista.
void goToFirst(UserList *list){
	list->pdi = list->first;
}

// nextNode: funció que mou el punt d'interes al següent node.
void nextNode(UserList *list){
	list->pdi = list->pdi->next;
}

// insertList: funció on inserim un nou node. Retorna -1 si no hi ha suficient memoria i 
// retorna 1 si s'ha inserit correctament el node.
int insertNode(UserList *list, User newNode){
	User *aux; // aux: node auxiliar on demanarem memoria per al nou node.
	aux = (User*) malloc(sizeof(User));
	if(aux == NULL){
		writeMsg("Error al demanar memoria per el nou node \n");
		return ERROR;
	}
	strcpy(aux->name, newNode.name);
	strcpy(aux->passwordMD5, newNode.passwordMD5);
	aux->id = newNode.id;
	aux->socket = -1;
	aux->next = list->first->next;
	list->first->next = aux;
	list->pdi = list->first->next;
	return OK;
}

// createList: funció per demanar memoria per la llista, i crea el primer node,
// que serà el node fantasma. Retorna -1 si no hi ha suficient memoria i retorna 1 
// si s'ha creat correctament la llista.
int createList(UserList *list){
	list->first = (User*) malloc(sizeof(User)); // Demanem memòria per el node fantasma.
	if(list->first == NULL){
		writeMsg("Error al demanar memoria per el node fantasma\n");
		return ERROR;
	}
	// Omplim les dades del node fantasma.
	strcpy(list->first->name, "FANTAS\0");
	strcpy(list->first->passwordMD5, "FANTASMA\0");
	list->first->id = -1;
	list->first->next = NULL;
	list->first->socket = -1;
	// Fiquem de moment pdi a NULL.
	list->pdi = NULL;
	return OK;	
}

// searchId: selecciona la informació de la llista amb el mateix ID i la volca al punter del node.
// Retorna 1 si ha trobat informació i si retorna -1 no ha trobat res en la llista.
int searchId(UserList list, int id, User *node){
	goToFirst(&list);
	while(list.pdi != NULL){
		if(list.pdi->id == id){
			memset(node->name, 0, 8);
			memset(node->passwordMD5, 0, 32);
			strcpy(node->name, list.pdi->name);
			strcpy(node->passwordMD5, list.pdi->passwordMD5);
			node->id = id;
			node->socket = list.pdi->socket;
			node->next = NULL;
			return OK;
		}	
		nextNode(&list);
	}
	return ERROR;
}

// searchName: selecciona la informació de la llista amb el mateix nom i la volca al punter del node.
// Retorna 1 si ha trobat informació i si retorna -1 no ha trobat res en la llista.
int searchName(UserList list, char name[8], User *node){
	goToFirst(&list);
	while(list.pdi != NULL){
		if(strcmp(list.pdi->name, name) == 0){
			memset(node->name, 0, 8);
			memset(node->passwordMD5, 0, 32);
			strcpy(node->name, list.pdi->name);
			strcpy(node->passwordMD5, list.pdi->passwordMD5);
			node->id = list.pdi->id;
			node->socket = list.pdi->socket;
			node->next = NULL;
			return OK;
		}
		nextNode(&list);
	}
	return ERROR;
}

// getNameBySocket: busca el nom del usuari a partir del seu socket.
int getNameBySocket(UserList list, int socket, char name[8]){
	goToFirst(&list);
	while(list.pdi != NULL){
		if(list.pdi->socket == socket){
			strcpy(name, list.pdi->name);
			return OK;
		}
		nextNode(&list);
	}
	return ERROR;
}

// getIDByName: busca l'ID del usuari  a partir del seu nom.
int getIDByName(UserList list, char name[8]){
	goToFirst(&list);
	while(list.pdi != NULL){
		if(strcmp(list.pdi->name, name) == 0){
			return list.pdi->id;
		}
		nextNode(&list);
	}
	return ERROR;
}

// getSocket: busca el socket del usuari.
// Retorna el socket del usuari a partir de seu nom.
int getSocket(UserList list, char name[8]){
	goToFirst(&list);
	while(list.pdi != NULL){
		if(strcmp(list.pdi->name, name) == 0){
			return list.pdi->socket;
		}
		nextNode(&list);
	}
	return ERROR;
}

// setSocket: inicialitza el socket del usuari desitjat.
void setSocket(UserList list, char name[8], int socket){
	goToFirst(&list);
	while(list.pdi != NULL){
		if(strcmp(list.pdi->name, name) == 0){
			list.pdi->socket = socket;
		}
		nextNode(&list);
	}
}

// deleteNode: elimina el node que apunta el punter next del pdi.
// Rep la LinketList apuntant al node abans del node que volem eliminar.
void deleteNode(UserList *list){
	User *temp; // temp: node temporal
	temp = list->pdi->next;
	list->pdi->next = temp->next;
	free(temp);
}

// deleteNodeByName: busca el node que volem eliminar pel nom.
// Rep la LinkedList i el nom del node que volem eliminar.
// Retorna 1 si ha trobat i eliminat el node, retorna -1 si no ha trobat el node.
int deleteNodeByName(UserList *list, char name[8]){
	goToFirst(list);
	while(list->pdi != NULL){
		if(strcmp(list->pdi->next->name, name) == 0){
			deleteNode(list);
			list->pdi = list->first;
			return OK;
		}
		list->pdi = list->pdi->next;
	}
	list->pdi = list->first;
	return ERROR;
}

// deleteNodeById: busca el node que volem eliminar pel numero identificador.
// Rep la LinkedList i el numero identificador del node que volem eliminar.
// Retorna 1 si ha trobat i eliminat el node, retorna -1 si no ha trobat el node.
int deleteNodeById(UserList *list, int id){
	goToFirst(list);
	while(list->pdi != NULL){
		if(list->pdi->next->id == id){
			deleteNode(list);
			list->pdi = list->first;
			return OK;
		}
		list->pdi = list->pdi->next;
	}
	list->pdi = list->first;
	return ERROR;
}

// deleteList: funció per esborrar tota la llista.
// Rep la LinkedList que volem eliminar.
void deleteList(UserList *list){
	User *temp; // temp: node temporal
	while(list->first != NULL){
		if(list->first->socket > 0){
			close(list->first->socket);
		}
		temp = list->first;
		list->first = list->first->next;
		free(temp);
	}
	list->first = NULL;
	list->pdi = NULL;
}
