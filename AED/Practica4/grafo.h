#ifndef GRAFO_H
#define GRAFO_H

#define MAXVERTICES 200 /*maximo numero de vertices*/

/*
 * Implementacion estatica del TAD grafo con una matriz
 * de adyacencia con maximo numero de vertices MAXVERTICES 
 */

/////////////////////////////////////////////////////////// TIPOS DE DATOS
#define TAM_ALIAS 30
#define TAM_COLECTIVO 50

//Informacion que se almacena en cada vertice
typedef struct {
    char alias[TAM_ALIAS];
    int categoria;
    char colectivo[TAM_COLECTIVO];
} TIPOVERTICE;

//Informacion que se almacena en cada arco
typedef struct {
    float coste;
    float duracion;
    char modalidad; // 'D' (directa) o 'R' (remota)
    int existe;     // 1 si la colaboracion existe, 0 si no hay relacion
} TIPOARCO;
//Tipo opaco TGRAFO
typedef struct tipografo * TGRAFO;

//////////////////////////////////////////////////////////////// FUNCIONES
/**
 * Creación del grafo vacío
 * @param G Puntero al grafo.
 */
void crearGrafo(TGRAFO *G);

/**
 * Destrucción del grafo
 * @param G Puntero al grafo.
 */
void destruirGrafo(TGRAFO *G);

/**
 * Devuelve 1 si el grafo G existe y 0 en caso contrario
 * @param G grafo.
 */
int existeGrafo(TGRAFO G);

/**
 * Devuelve 1 si el vertice Vert existe en el grafo G
 * @param G grafo.
 * @param Vert vertice
 */
int existeVertice(TGRAFO G, TIPOVERTICE Vert);

/**
 * Devuelve 1 si los vertices en las posiciones pos1 y pos2 son adyacentes
 * @param G grafo.
 * @param pos1 posicion del vertice origen
 * @param pos2 posicion del vertice destino
 */
int existeArco(TGRAFO G, int pos1, int pos2);

/**
 * Inserta un vertice en el grafo, devolviendo su posicion o -1 si no se inserta
 * @param G Puntero a grafo.
 * @param Vert vertice
 */
int crearVertice(TGRAFO *G, TIPOVERTICE Vert);

/**
 * Elimina un vertice del grafo
 * @param G Puntero a grafo.
 * @param Vert vertice
 */
void borrarVertice(TGRAFO *G, TIPOVERTICE Vert);

/**
 * Crea el arco entre los vertices con posiciones pos1 y pos2
 * @param G Puntero al grafo
 * @param pos1 posicion del vertice origen
 * @param pos2 posicion del vertice destino
 */
void crearArco(TGRAFO *G, int pos1, int pos2,TIPOARCO arco);

/**
 * Elimina el arco entre los vertices con posiciones pos1 y pos2
 * @param G Puntero al grafo
 * @param pos1 posicion del vertice origen
 * @param pos2 posicion del vertice destino
 */
void borrarArco(TGRAFO *G, int pos1, int pos2);


/**
 * Devuelve la posicion del vertice Vert en el vector VERTICES o -1 si no esta
 * @param G grafo
 * @param Vert vertice
 */
int posicionVertice(TGRAFO G, TIPOVERTICE Vert);

/**
 * Devuelve el número de vértices del grafo G
 * @param G grafo
 * @param Vert vertice
 */
int leerN(TGRAFO G);

/**
 * Devuelve el vector de vértices VERTICES del grafo G
 * @param G grafo
 */
TIPOVERTICE* leerVertices(TGRAFO G);

/**
 * Devuelve el contenido del arco de pos1 a pos2
 * @param G grafo
 * @param pos1 posicion del vertice origen
 * @param pos2 posicion del vertice destino
 */
TIPOARCO leerArco(TGRAFO G, int pos1, int pos2);

#endif	/* GRAFO_H */
