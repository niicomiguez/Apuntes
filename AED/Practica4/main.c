#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "creatornet.h"

int main(int argc, char **argv)
{
    TGRAFO G;
    char opcion;

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
        case 's':
            break;
        default:
            printf("Opcion equivocada\n");
        }
    } while (opcion != 's');

    destruirGrafo(&G);

    return (EXIT_SUCCESS);
}