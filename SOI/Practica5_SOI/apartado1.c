#include <stdio.h>
#include <unistd.h>

int global_init = 10;
int global_no_init;
int array_global[10];

int main(int argc, char const *argv[])
{
    int local1, local2;
    int array3d[10][10][10];

    printf("PID del proceso: %d\n", getpid());
    printf("Dirección main: %p\n\n", (void *)main);

    printf("--- GLOBALES ---\n");
    printf("Direccion global_init:    %p\n", (void *)&global_init);
    printf("Direccion global_no_init: %p\n", (void *)&global_no_init);
    printf("Direccion array_global:   %p\n\n", (void *)array_global);

    printf("--- LOCALES (PILA) ---\n");
    printf("Direccion local1:         %p\n", (void *)&local1);
    printf("Direccion local2:         %p\n", (void *)&local2);
    printf("Direccion array3d:        %p\n\n", (void *)array3d);

    printf("--- ORDEN EN ARRAY 3D [10][10][10] ---\n");
    printf("array3d[0][0][0]: %p\n", (void *)&array3d[0][0][0]);
    printf("array3d[0][0][1]: %p\n", (void *)&array3d[0][0][1]);
    printf("array3d[0][1][0]: %p\n", (void *)&array3d[0][1][0]);
    printf("array3d[1][0][0]: %p\n\n", (void *)&array3d[1][0][0]);

    printf("Pulsa ENTER para terminar...");
    getchar();
    return 0;
}
