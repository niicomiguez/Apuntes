#ifndef CREATORNET_H
#define CREATORNET_H

#include "grafo.h"

/**
 * Inicializar el grafo leyendo datos de un archivo o iniciando vacio.
 * @param G Puntero al grafo.
 * @param argc numero de cadenas por linea de comandos (argumento de main)
 * @param argv cadenas por linea de comandos: ejecutable -f archivo
 */
void inicializar(TGRAFO *G, int argc, char **argv);

//TODO: FUNCIONES PUBLICAS QUE SE LLAMAN DESDE MAIN

/**
 * Solicita por teclado los datos de un/a nuevo/a creador/a (alias, categoría y colectivo)
 * e intenta insertarlo/a en la red controlando duplicados y límites.
 * @param G Puntero al grafo.
 */
void anhadirCreador(TGRAFO *G);

/**
 * Solicita el alias de un/a creador/a existente y lo/a elimina de la red 
 * junto con todas sus colaboraciones asociadas.
 * @param G Puntero al grafo.
 */
void eliminarCreador(TGRAFO *G);

/**
 * Solicita los datos de una nueva colaboración entre dos creadores/as (ambos alias, 
 * coste, duración y modalidad) e inserta el arco de forma simétrica si cumple las validaciones.
 * @param G Puntero al grafo.
 */
void crearColaboracion(TGRAFO *G);

/**
 * Solicita los alias de dos creadores/as y elimina la colaboración 
 * existente entre ellos/as en ambos sentidos.
 * @param G Puntero al grafo.
 */
void eliminarColaboracion(TGRAFO *G);

/**
 * Muestra por pantalla la lista completa de todos/as los/as creadores/as 
 * registrados/as junto con sus respectivas listas de colaboraciones.
 * @param G Grafo que contiene la red.
 */
void mostrarRed(TGRAFO G);

/**
 * Solicita el alias de un/a creador/a y muestra únicamente sus colaboraciones 
 * asociadas con el formato detallado de coste, duración y modalidad.
 * @param G Grafo que contiene la red.
 */
void consultarCreador(TGRAFO G);

#endif	/* CREATORNET_H */

