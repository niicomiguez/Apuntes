#include <stdio.h>
#include <stdlib.h>
#include "misfunciones.h"

/*
 * Programa que muestra el uso del TAD grafo de números enteros
 */

int main(int argc, char** argv) {
    //Grafo de números enteros
    TGRAFO G; //grafo
    char opcion;

    //Creo el grafo
    crearGrafo(&G);
    do {
        printf("\n\na. Insertar nuevo vertice\n");
        printf("b. Eliminar vertice\n");
        printf("c. Crear arco\n");
        printf("d. Eliminar arco\n");
        printf("i. Imprimir grafo\n");
        printf("s. Salir\n");

        printf("Opcion: ");
        scanf(" %c", &opcion); 

        switch (opcion) {
            case 'a':case'A':
                anhadirVertice(&G);
                break;
            case 'b':case 'B':
                eliminarVertice(&G);
                break;
            case 'c': case 'C':
                anhadirArco(&G);
                break;
            case 'd': case 'D':
                eliminarArco(&G);
                break;
            case 'i': case 'I':
                imprimirGrafo(G);
                break;
            case 's': case 'S':
                opcion='s';
                break;
            default:
                printf("Opción equivocada\n");
        }
    } while (opcion != 's');

    //Al salir, liberamos la memoria del TAD, lo destruimos
    destruirGrafo(&G);

    return (EXIT_SUCCESS);
}

