#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include "creatorverse.h"
#include <string.h>

// Se referencian aquí para no modificar creatorverse.h
void nuevosRetos(TABB *A);
void completarReto(TABB *A);
void guardarBaseDatos(TABB A, int argc, char **argv);

void ejecutarMenu(TABB *arbol) {
    bool activo = true;
    char entrada[20];
    
    while (activo) {
        printf("Bienvenid@ a CreatorVerse\n");
        printf("A/a. Anhadir creador\n");
        printf("L/l. Listar creadores\n");
        printf("E/e. Eliminar creador\n");
        printf("N/n. Anhadir nuevo reto\n");
        printf("C/c. Completar siguiente reto\n");
        printf("S/s. Salir\n");
        printf("Seleccione una opcion: ");
        
        scanf(" %s", &entrada);

        if (strlen(entrada) == 1) {
            switch (tolower(entrada[0])) 
            {
            case 's':
                activo = false;
                printf("\nGracias por usar nuestro programa\n");
                break;
             case 'a':
                anhadirCreador(arbol);
                break;
             case 'l':
                listarCreadores(*arbol);
                break;
             case 'e':
                eliminarCreador(arbol);
                break;
             case 'n':
                nuevosRetos(arbol);
                break;
             case 'c':
                completarReto(arbol);
                break;
            
            default:
                printf("\nSeleccione una opción disponible\n");
                break;
            }
        } else {
            printf("\nSeleccione una opción disponible\n");
        }
    }
}

int main(int argc, char *argv[]) {
    // Declarar el árbol
    TABB miArbol; 
    
    // Iniciamos creando el arbol y leyendo el archivo si se pasa por parámetro
    inicializarCreadores(&miArbol, argc, argv);

    // Ejecutar el menú pasándole la dirección de nuestro árbol
    ejecutarMenu(&miArbol);

    guardarBaseDatos(miArbol, argc, argv);

    // Destrucción al acabar ejecución
    destruirAbb(&miArbol);

    return 0;
}