#ifndef TABLA_HASH_RECOLOCACION_H
#define TABLA_HASH_RECOLOCACION_H
//////////////////////////////////////////////////////////////////////////////////////////
//////MODIFICACIONES/////////////////////////////////////////////////////////////////////
//1. TAMA�O DE LA TABLA (CONSTANTE N): probar con distintos tama�os
//2. FUNCI�N InsertarHash(): convertirla en una funci�n que devuelva 0 (no colisi�n) o 1 (colisi�n)
//   y a�adir par�metro nPasosExtraI
//3. FUNCI�N BuscarHash(): a�adir el par�metro nPasosExtraB
//4. FUNCI�N FuncionHash(): distintas funciones, para cadenas y para enteros 
//////////////////////////////////////////////////////////////////////////////////////////

/* 
 * Tabla hash con recolocaci�n
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define N 18001 /*DEFINIR NUMERO de entradas en la tabla.*/
//El factor de carga es L=n/N, donde n=datos (9000)
//Se recomienda L<=0,5 para recolocaci�n, es decir, N>=2n
//Se recomienda L<=0,75 para encadenamiento, es decir, N>=4n/3
//Debemos probar con N y el primo siguiente mayor que N

///////////CONSTANTES PARA MARCAR LAS CELDAS BORRADAS O VAC�AS
#define VACIO '\0'
#define BORRADO ' '
/////////CONSTANTES PARA DEFINIR LOS CAMPOS DEL TIPOELEMENTO
#define L 15

typedef struct{
    char nombre[3*L];
    char id[L];
    char telefono[L];
}TIPOELEMENTO;

//DEFINICI�N DEL TAD TablaHash
typedef TIPOELEMENTO TablaHash[N];

/**
 * Inicializa cada elemento de la tabla a un contenido vac�o (constante VACIO)
 * Marcamos solo el campo id, suficiente para identificar la celda vac�a
 * @param t tabla hash.
 */
void InicializarTablaHash(TablaHash t);

/**
 * Funciones hash: 3 tipos
 * @param elemento para calcular la posici�n en la tabla hash
 * @tipoFH=1-2-3, las tres funciones hash implementadas
 * @return la posici�n del elemento en la tabla hash
 */
int FuncionHash(TIPOELEMENTO e, unsigned int tipoFH);

/**
 * Mira si el elemento est� en la tabla hash
 * @param t es la tabla hash en la que buscamos la clave
 * @e: es elemento que buscamos, a partir de su clave
 * @tipoclave: indica si queremos usar el id (1) o el tel�fono (2) como clave
 * @tipoFH es la funci�n hash utilizada (1,2,3)
 * @tipoR es la recolocaci�n utilizada (1=lineal, usa el par�metro @a, 2=cuadr�tica)
 * @a es el par�metro a de la recolocaci�n lineal
 * @nPasosExtraB son los pasos que tengo que realizar en cada lista hasta encontrar e
 * @return si la clave est� en la tabla
 */
int EsMiembroHash(TablaHash t, TIPOELEMENTO e, int tipoclave, unsigned int tipoFH, unsigned int tipoR, unsigned int a,int *nPasosExtraB);

/**
 * Buscar una clave en la tabla hash
 * @param t es la tabla hash en la que buscamos la clave
 * @buscado: es el elemento que buscamos
 * @e es donde almacenamos el elemento encontrado
 * @tipoclave: indica si queremos usar el id (1) o el tel�fono (2) como clave
 * @tipoFH es la funci�n hash utilizada (1,2,3)
 * @tipoR es la recolocaci�n utilizada (1=lineal, usa el par�metro @a, 2=cuadr�tica)
 * @a es el par�metro a de la recolocaci�n lineal
 * @nPasosExtraB son los pasos que tengo que realizar en cada lista hasta encontrar e
 * @return si la b�squeda tuvo �xito
 */
/////////////////////Hay que a�adir nPasosExtraB como par�metro por referencia
int BuscarHash(TablaHash t, TIPOELEMENTO buscado, TIPOELEMENTO *e, int tipoclave, unsigned int tipoFH, unsigned int tipoR, unsigned int a,int *nPasosExtraB);

/**
 * Inserta un elemento en una tabla hash al principio de la lista
 * @param t es la tabla hash
 * @e es el elemento que queremos guardar en la tabla
 * @tipoclave: indica si queremos usar el id (1) o el tel�fono (2) como clave
 * @tipoFH es la funci�n hash utilizada (1,2,3)
 * @tipoR es la recolocaci�n utilizada (1=lineal, usa el par�metro @a, 2=cuadr�tica)
 * @a es el par�metro a de la recolocaci�n lineal
 * @nPasosExtraI son los pasos que tengo que realizar hasta encontrar una posici�n libre
 * @return devuelve 1 si se produce colisi�n y 0 en caso contrario
 */
///HACER QUE ESTA FUNCI�N DEVUELVA UN ENTERO QUE INDIQUE SI HUBO O NO COLISI�N
///POR TANTO NO SER� UNA FUNCI�N void
int InsertarHash(TablaHash t, TIPOELEMENTO e, int tipoclave, unsigned int tipoFH, unsigned int tipoR, unsigned int a, int *nPasosExtraI);

/**
 * Borra un elemento en una tabla hash
 * @param t es la tabla hash
 * @e es el elemento que queremos borrar en la tabla
 * @tipoFH es la funci�n hash utilizada (1,2,3)
 * @tipoR es la recolocaci�n utilizada (1=lineal, usa el par�metro @a, 2=cuadr�tica)
 * @a es el par�metro a de la recolocaci�n lineal
 * @nPasosExtraE son los pasos que tengo que realizar hasta encontrar el elemento a eliminar
 */
void BorrarHash(TablaHash t, TIPOELEMENTO e, int tipoclave, unsigned int tipoFH, unsigned int tipoR, unsigned int a, int *nPasosExtraE);

#endif	// TABLA_HASH_RECOLOCACION_H