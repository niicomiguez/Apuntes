#include <stdlib.h>
#include <stdio.h>

#include "cola.h"

//celda que contiene los elementos de la cola
struct celdaCola{
    TIPOELEMENTOCOLA elemento;
    struct celdaCola * sig;
};
typedef struct celdaCola * puntero;

/////////////////////ESTRUCTURAS DE DATOS
//celda cabecera: contiene informacion de frente y final de la cola
struct cabeceraCola{
	puntero frente;
	puntero final;
};


///////////////////////FUNCIONES

//Crea la cola vacia, con cabecera de la cola y 
//punteros nulos a frente y final
void crearCola(TCOLA *C)
{
    *C=(TCOLA)malloc(sizeof(struct cabeceraCola));
    if (C == NULL)
      printf("Error: Memoria insuficiente\n");
    else{
        (*C)->frente=(puntero)malloc(sizeof(struct celdaCola));
        if ((*C)->frente == NULL)
             printf("Error: Memoria insuficiente\n");
        else{
            (*C)->final=(*C)->frente;
            (*C)->frente->sig=NULL;
        }
    }
}

//Destruye la cola, recorriendo sus elementos
void destruirCola(TCOLA *C) {
    puntero p, r;
    p = (*C)->frente;
    while (p != NULL) {
        r = p;
        p = p->sig;
        free(r);
    }
    free(*C);
}

//Informa si la cola está vacia
unsigned esVaciaCola(TCOLA C)
{
    return (C->frente==C->final);
}

//Devuelve el elemento el elemento en el frente de la cola
TIPOELEMENTOCOLA primeroCola(TCOLA C)
{
    if (!esVaciaCola(C))
	return (C->frente)->sig->elemento;
    
}

//Inserta un elemento al final de la cola
void insertarCola(TCOLA *C, TIPOELEMENTOCOLA E)
{
    ((*C)->final)->sig=(puntero)malloc(sizeof(struct celdaCola));
    if ((*C)->final->sig == NULL)
        printf("Error: Memoria insuficiente\n");
    else{
        (*C)->final=((*C)->final)->sig;
        ((*C)->final)->elemento=E;
        ((*C)->final)->sig=NULL;
    }
}

//Suprime el primer elemento de la cola
void suprimirCola(TCOLA *C)
{
    puntero p;
    if (!esVaciaCola(*C)){
        p=(*C)->frente;
	(*C)->frente=(*C)->frente->sig;
	free(p);
    }
}
