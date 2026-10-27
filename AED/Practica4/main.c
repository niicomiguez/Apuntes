#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "creatornet.h"
#include <string.h>


int main(int argc, char **argv)
{
    TGRAFO G;
    crearGrafo(&G);
    char opcion;
    char nombreFichero[100] = "";

    if (argc >= 3 && strcmp(argv[1], "-f") == 0) {
        strncpy(nombreFichero, argv[2], sizeof(nombreFichero) - 1);
        cargarRed(&G, nombreFichero);
    } else {
        printf("Iniciando con una red vacia.\n");
    }

    inicializar(&G, argc, argv);

    do
    {
        printf("\n--- CREATORNET ---\n");
        printf("A/a. Anhadir creador/a\n");
        printf("E/e. Eliminar creador/a\n");
        printf("C/c. Crear colaboracion\n");
        printf("B/b. Eliminar colaboracion\n");
        printf("I/i. Mostrar red\n");
        printf("L/l. Consultar creador/a\n");
        printf("T/t. Mostrar estadísticas/a\n");
        printf("P/p. Red de colaboracion coste minimo/a\n");
        printf("F/f. Camino minimo por coste/a\n");
        printf("D/d. Camino minimo por duración/a\n");
        printf("S/s. Salir\n");

        printf("Opcion: ");
        scanf(" %c", &opcion);
        opcion = tolower(opcion);

        switch (opcion)
        {
        case 'a':
            anhadirCreador(&G);
            break;
        case 'e':
            eliminarCreador(&G);
            break;
        case 'c':
            crearColaboracion(&G);
            break;
        case 'b':
            eliminarColaboracion(&G);
            break;
        case 'i':
            mostrarRed(G);
            break;
        case 'l':
            consultarCreador(G);
            break;
        case 't':
            mostrarEstadisticas(G);
            break;
        case 'p':
            redDeCosteMinimo(G);
        case 'f':
            caminoMinimo(G,1);
            break;
        case 'd':
            caminoMinimo(G,2);
            break;
        case 's':
            break;
        default:
            printf("Opcion equivocada\n");
        }
    } while (opcion != 's');

    if (strlen(nombreFichero) == 0) {
        printf("\nIntroduce el nombre del fichero para guardar la red (ej: datos.csv): ");
        scanf(" %99s", nombreFichero);
    }

    guardarRed(G, nombreFichero);
    destruirGrafo(&G);

    printf("Saliendo de CreatorNet. ¡Hasta pronto!\n");
    return 0;
}