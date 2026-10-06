#ifndef COLA_H
#define COLA_H

///////////////////////////////////////INICIO PARTE MODIFICABLE
//si hace falta definicion de constantes se ponen aqui
#define MAX_RETO 20
typedef struct
{
    char titulo[MAX_RETO];
    int dificultad;
}TIPOELEMENTOCOLA;
//////////////////////////////////////////FIN PARTE MODIFICABLE

//definicion del tipo opaco
typedef struct cabeceraCola *TCOLA;

//Funciones de creacion/destruccion
/**
 * Crea la cola vacia. 
 * @param C Puntero a la cola. Debe estar inicializada.
 */
void crearCola(TCOLA *C);

/**
 * Destruye la cola
 * @param C puntero a la cola que queremos destruir
 */
void destruirCola(TCOLA *C);

//Funciones de informacion
/**
 * Comprueba si la cola esta vacia
 * @param C cola
 */
unsigned esVaciaCola(TCOLA C);

/*
 * Recupera la informacion del primer elemento de la cola
 * @param C cola
 * 
*/
TIPOELEMENTOCOLA primeroCola(TCOLA C);

//Funciones de insercion/eliminacion
/**
 * Inserta un nuevo nodo en la cola para el elemento E
 * al final de la cola
 * @param C puntero a la cola
 * @param E Informacion del nuevo nodo. 
 */
void insertarCola(TCOLA *C, TIPOELEMENTOCOLA E);

/**
 * Suprime el primer elemento de la cola
 * @param C puntero a la cola
 */
void suprimirCola(TCOLA *C);

#endif	// COLA_H