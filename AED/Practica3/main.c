#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include "creatorverse.h"

void nuevosRetos(TABB *A);

void ejecutarMenu(TABB *arbol) {
    bool activo = true;
    char entrada;
    
    while (activo) {
        printf("Bienvenid@ a CreatorVerse\n");
        printf("A/a. Anhadir creador\n");
        printf("L/l. Listar creadores\n");
        printf("E/e. Eliminar creador\n");
        printf("N/n. Anhadir nuevo reto\n");
        printf("C/c. Completar siguiente reto\n");
        printf("S/s. Salir\n");
        printf("Seleccione una opcion: ");
        
        scanf(" %c", &entrada);

        switch (tolower(entrada))
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
            break;
        
        default:
            break;
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

    // Destrucción al acabar ejecución
    destruirAbb(&miArbol);

    return 0;
}