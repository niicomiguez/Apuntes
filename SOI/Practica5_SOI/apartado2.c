#include <stdio.h>
#include <unistd.h>

void f2(int param2)
{
    int local2 = 20;
    printf("--- DENTRO DE f2 ---\n");
    printf("Direccion param2:  %p\n", (void *)&param2);
    printf("Direccion local2:  %p\n\n", (void *)&local2);
}

void f1(int param1)
{
    int local1 = 10;
    printf("--- DENTRO DE f1 ---\n");
    printf("Direccion param1:  %p\n", (void *)&param1);
    printf("Direccion local1:  %p\n\n", (void *)&local1);
}

int main()
{
    int local_main = 5;

    printf("PID del proceso: %d\n\n", getpid());

    printf("--- DIRECCIONES DE CODIGO (FUNCIONES) ---\n");
    printf("Direccion main: %p\n", (void *)main);
    printf("Direccion f1:   %p\n", (void *)f1);
    printf("Direccion f2:   %p\n\n", (void *)f2);

    printf("--- DENTRO DE main ---\n");
    printf("Direccion local_main: %p\n\n", (void *)&local_main);

    f1(100);
    f2(200);

    printf("Pulsa ENTER para terminar...");
    getchar();

    return 0;
}