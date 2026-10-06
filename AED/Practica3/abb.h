#ifndef ABB_H
#define ABB_H

///////////////////////////////////////INICIO PARTE MODIFICABLE
//si hay que poner definiciones de constantes se ponen aqui
//si hay que poner algun include se pone aqui
#include "cola.h"

#define MAX_ALIAS 50
#define MAX_COLEC 50
#define MAX_DESC 200

typedef char *TIPOCLAVE;

typedef struct {
    char alias[MAX_ALIAS];
    int categoria;
    char colectivo[MAX_COLEC];
    long seguidores;
    int verificado;
    TCOLA retos;
    char descripcion[MAX_DESC];
} TIPOELEMENTOABB;
//////////////////////////////////////////FIN PARTE MODIFICABLE
		
typedef struct celda * TABB;//tipo opaco

/////////////////////////////// FUNCIONES

//FUNCIONES DE CREACION Y DESTRUCCION DEL ARBOL
/**
 * Crea el arbol vacio.
 * @param A Puntero al arbol. Debe estar inicializado.
 */
void crearAbb(TABB *A);

/**
 * Destruye el arbol recursivamente
 * @param A El arbol que queremos destruir
 */
void destruirAbb(TABB *A);

//FUNCIONES DE INFORMACION
/**
 * Comprueba si el arbol esta vacio
 * @param A El arbol binario
 */
unsigned esAbbVacio(TABB A);

/**
 * Devuelve el subarbol izquierdo de A
 * @param A - Arbol original
 */
TABB izqAbb(TABB A);
/**
 * Devuelve el subarbol derecho de A
 * @param A - Arbol original
 */
TABB derAbb(TABB A);
/**
 * Recupera la informacion de la celda de la raiz del arbol
 * @param A
 * @param E Puntero al nodo que apuntara a la informacion
 */
void leerElementoAbb(TABB A, TIPOELEMENTOABB *E);

/**
 * Comprueba si el elemento <E> existe en el arbol <A>
 * @param A
 * @param E
 */
unsigned esMiembroAbb(TABB A, TIPOELEMENTOABB E);

/**
 * Busca un nodo con clave <cl> en el arbol y, si existe
 * modifica el puntero <nodo> para que lo apunte.
 * @param A Arbol binario en el que se busca el nodo.
 * @param cl Clave que se buscara.
 * @param nodo Puntero a un tipoelem.
 */
void buscarNodoAbb(TABB A, TIPOCLAVE cl, TIPOELEMENTOABB *nodo);

//FUNCIONES DE MODIFICACION
/**
 * Inserta un nuevo nodo en el arbol para el elemento E
 * del que toma su clave. Esta clave no debe existir en
 * el arbol.
 * @param A Arbol binario
 * @param E Informacion del nuevo nodo.
 */
void insertarElementoAbb(TABB *A, TIPOELEMENTOABB E);
/**
 * Suprime el nodo del arbol <A>
 * @param A
 * @param E
 */
void suprimirElementoAbb(TABB *A,TIPOELEMENTOABB E);
/**
 * Busca el nodo con clave la del nodo y lo modifica.
 * @param A
 * @param nodo
 */
void modificarElementoAbb(TABB A, TIPOELEMENTOABB nodo);

#endif	// ABB_H

