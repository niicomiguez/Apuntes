#ifndef FUNCIONES_H
#define FUNCIONES_H

#include "grafo.h"

//FUNCIONES DEL PROGRAMA DE PRUEBA DE GRAFOS
//Opción a del menú, introducir un vertice en el grafo
void anhadirVertice(TGRAFO *G);

//Opción b del menú, eliminar un vértice del grafo
void eliminarVertice(TGRAFO *G);

//Opción c del menú, crear una relación entre dos vértices
void anhadirArco(TGRAFO *G);

//Opción d del menú, eliminar una relación entre dos vértices
void eliminarArco(TGRAFO *G);

//Opción i del menú, imprimir el grafo
void imprimirGrafo(TGRAFO G);


#endif	/* FUNCIONES_H */

