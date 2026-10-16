#ifndef CREATORVERSE_H
#define CREATORVERSE_H

#include "abb.h"

/**
 * Crea el arbol, lee el archivo y lo almacena en el arbol
 * @param A Arbol.
 * @param nparam numero de argumentos de main en linea de comandos
 * @param args seran los argumentos de main: "-f" y nombrearchivo
 */
void inicializarCreadores(TABB *A, int nparam, char **args);

/**
 * Anhade un creador a la base de datos
 * @param A Arbol.
 */
void anhadirCreador(TABB *A);

/**
 * Listado de creadores por orden alfabetico
 * @param A Arbol.
 */
void listarCreadores(TABB A);

/**
 * Elimina un creador de la base de datos
 * @param A Arbol.
 */
void eliminarCreador(TABB *A);

/**
 * @brief Inicializa el árbol de creadores cargando los datos desde un archivo de texto.
 * @param A Puntero al Árbol Binario de Búsqueda (ABB) que se va a inicializar.
 * @param nparam Número de argumentos recibidos por la línea de comandos.
 * @param args Array de cadenas con los argumentos de la línea de comandos.
 */
void inicializarCreadores(TABB *A, int nparam, char **args);

/**
 * @brief Recorre y muestra por pantalla todos los creadores almacenados en el árbol.
 * @param A Árbol Binario de Búsqueda pasado por valor.
 */
void listarCreadores(TABB A);

/**
 * @brief Solicita los datos por teclado y añade un nuevo creador al árbol si su alias no existe.
 * @param A Puntero al Árbol Binario de Búsqueda donde se insertará el creador.
 */
void anhadirCreador(TABB *A);

/**
 * @brief Solicita un alias por teclado y elimina al creador correspondiente del árbol si existe.
 * @param A Puntero al Árbol Binario de Búsqueda del que se eliminará el creador.
 */
void eliminarCreador(TABB *A);

/**
 * @brief Solicita un alias por teclado y añade nuevos retos a la cola de dicho creador.
 * @param A Puntero al Árbol Binario de Búsqueda para actualizar el nodo del creador.
 */
void nuevosRetos(TABB *A);

/**
 * @brief Extrae, muestra y elimina el primer reto pendiente de la cola de un creador específico.
 * @param A Puntero al Árbol Binario de Búsqueda para actualizar el nodo del creador.
 */
void completarReto(TABB *A);

/**
 * @brief Guarda la información de todos los creadores en un archivo de texto siguiendo un recorrido preorden.
 * @param A Árbol Binario de Búsqueda pasado por valor.
 * @param argc Número de argumentos de consola para comprobar si hay un archivo de salida predeterminado.
 * @param argv Array de argumentos de consola con el posible nombre del archivo de salida.
 */
void guardarBaseDatos(TABB A, int argc, char **argv);

/**
 * @brief Solicita un alias por teclado, busca al creador en el árbol e imprime toda su información.
 * @param A Árbol Binario de Búsqueda pasado por valor.
 */
void buscarCreador(TABB A);

/**
 * @brief Recorre el árbol y las colas para encontrar e imprimir el reto o retos con la dificultad máxima.
 * @param A Árbol Binario de Búsqueda pasado por valor.
 */
void buscarRetoMasDificil(TABB A);

/**
 * @brief Recorre el árbol evaluando las colas para encontrar e imprimir los creadores con más retos pendientes.
 * @param A Árbol Binario de Búsqueda pasado por valor.
 */
void buscarCreadoresMasRetos(TABB A);

/**
 * @brief Despliega un submenú interactivo para modificar los campos de un creador existente, incluyendo su recolocación si cambia el alias.
 * @param A Puntero al Árbol Binario de Búsqueda para aplicar los cambios en la estructura.
 */
void modificarCreador(TABB *A);

#endif /* CREATORVERSE_H */

