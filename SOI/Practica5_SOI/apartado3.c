#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
    printf("PID del proceso: %d\n\n", getpid());

    printf("Estado INICIAL (antes de malloc). Pulsa ENTER...");
    getchar();

    // Caso A: Reserva pequeña (20 bytes -> cabe en el [heap] ya creado por printf)
    int *ptr_peq = (int *)malloc(5 * sizeof(int));
    printf("\n2. Tras malloc PEQUEÑO (20 B) -> Direccion: %p\n", (void *)ptr_peq);
    printf("Pulsa ENTER...");
    getchar();

    // Caso B: Reserva grande (10 MB -> supera el umbral de brk y usa mmap)
    char *ptr_grande = (char *)malloc(10 * 1024 * 1024);
    printf("\n3. Tras malloc GRANDE (10 MB) -> Direccion: %p\n", (void *)ptr_grande);
    printf("Mira maps ahora (aparece una region nueva de ~10MB). Pulsa ENTER...");
    getchar();

    // Liberamos ambos
    free(ptr_peq);
    free(ptr_grande);
    printf("\n4. Tras FREE de ambos. Mira maps (la de 10MB desaparece, [heap] sigue igual). Pulsa ENTER...");
    getchar();

    // Nuevo malloc pequeño posterior
    int *ptr_nuevo = (int *)malloc(10 * sizeof(int));
    printf("\n5. Tras NUEVO malloc (40 B) -> Direccion: %p (reutiliza ptr_peq)\n", (void *)ptr_nuevo);
    printf("Pulsa ENTER para salir...");
    getchar();

    free(ptr_nuevo);
    return 0;
}