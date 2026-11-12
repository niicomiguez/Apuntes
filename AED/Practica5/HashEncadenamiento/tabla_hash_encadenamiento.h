#ifndef TABLA_HASH_ENCADENAMIENTO_H
#define TABLA_HASH_ENCADENAMIENTO_H
//////////////////////////////////////////////////////////////////////////////////////////
//////MODIFICACIONES/////////////////////////////////////////////////////////////////////
//1. TAMAÑO DE LA TABLA (CONSTANTE N): probar con distintos tamaños
//2. FUNCIÓN InsertarHash(): convertirla en una función que devuelva 0 (no colisión) o 1 (colisión)
//3. FUNCIÓN BuscarHash(): añadir el parámetro nPasosExtraB
//4. FUNCIÓN FuncionHash(): distintas funciones, para cadenas y para enteros
//////////////////////////////////////////////////////////////////////////////////////////

/* 
 * Tabla hash con encadenamiento
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lista.h"

#define N 18001 /*DEFINIR NUMERO de entradas en la tabla.*/
//El factor de carga es L=n/N, donde n=datos (9000)
//Se recomienda L<=0,5 para recolocación, es decir, N>=2n
//Se recomienda L<=0,75 para encadenamiento, es decir, N>=4n/3
//Debemos probar con N y el primo siguiente mayor que N

typedef TLISTA TablaHash[N];

/**
 * Inicializa cada elemento de la tabla a una lista vacÃ­a
 * @param t tabla hash.
 */
void InicializarTablaHash (TablaHash t);

/**
 * Destruye la lista que corresponde a cada elemento de la tabla
 * @param t tabla hash.
 */
void DestruirTablaHash (TablaHash t);

/**
 * Funciones hash: 3 tipos
 * @param elemento para calcular la posición en la tabla hash
 * @tipoFH=1-2-3, las tres funciones hash implementadas
 * @return la posición del elemento en la tabla hash
 */
int FuncionHash(TIPOELEMENTO e, unsigned int tipoFH);

/**
 * Buscar una clave en la tabla hash
 * @param t es la tabla hash en la que buscamos la clave
 * @param buscado: es el elemento que buscamos
 * @param e es donde almacenamos el elemento encontrado
 * @param tipoclave indica si queremos usar el id (1) o el teléfono (2) como clave 
 * @param tipoFH es la función hash utilizada (1,2,3)
 * @param nPasosExtraB son los pasos que tengo que realizar en cada lista hasta encontrar e
 * @return si la búsqueda tuvo éxito
 */
/////////////////////Hay que añadir nPasosExtraB//////////////////////////
int BuscarHash(TablaHash t, TIPOELEMENTO buscado, TIPOELEMENTO *e, int tipoclave, unsigned int tipoFH, int *nPasosExtraB);

/**
 * Mira si e está en la tabla hash
 * @param t es la tabla hash en la que buscamos la clave
 * @param e: elemento que buscamos
 * @param tipoclave indica si queremos usar el id (1) o el teléfono (2) como clave 
 * @param tipoFH es la función hash utilizada (1,2,3)
 * @nPasosExtraB son los pasos que tengo que realizar en cada lista hasta encontrar e
 * @return si la clave estÃ¡ en la tabla
 */
int EsMiembroHash(TablaHash t, TIPOELEMENTO e, int tipoclave, unsigned int tipoFH, int *nPasosExtraB);

/**
 * Inserta un elemento en una tabla hash al principio de la lista
 * @param t es la tabla hash
 * @param elemento es el elemento que queremos guardar en la tabla
 * @param tipoFH es la función hash utilizada (1,2,3)
 * @return devuelve 1 si se produce colisión y 0 en caso contrario
 */
///HACER QUE ESTA FUNCIÓN DEVUELVA UN ENTERO QUE INDIQUE SI HUBO O NO COLISIÓN
///POR TANTO NO SERÁ UNA FUNCIÓN void
int InsertarHash (TablaHash *t, TIPOELEMENTO elemento, unsigned int tipoFH);

/**
 * Borra un elemento en una tabla hash
 * @param t es la tabla hash
 * @param e es el elemento que queremos borrar en la tabla
 * @param tipoclave indica si queremos usar el id (1) o el teléfono (2) como clave 
 * @param tipoFH es la función hash utilizada (1,2)
*/
void BorrarHash(TablaHash *t, TIPOELEMENTO e, int tipoclave, unsigned int tipoFH, int *nPasosExtraE);
#endif	// TABLA_HASH_ENCADENAMIENTO_H

