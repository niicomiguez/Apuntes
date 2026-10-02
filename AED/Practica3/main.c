#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>

void ejecutarMenu() {
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
            break;
         case 'a':
            break;
         case 'l':
            break;
         case 'e':
            break;
         case 'n':
            break;
         case 'c':
            break;
        
        default:
            break;
        }
        
    }
}

int main() {
    ejecutarMenu();
    return 0;
}