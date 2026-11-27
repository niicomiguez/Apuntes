#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

int global = 2;

void* manejadorHilo(void* arg){
    int local_recibida = *(int*)arg;
    int nueva_local = local_recibida * global;
    int *ptr_hilo = (int *)malloc(sizeof(int));
    *ptr_hilo = nueva_local;

    printf("--- HILO %lu ---\n", (unsigned long)pthread_self());
    printf("global         : %d  | dir: %p\n", global, (void *)&global);
    printf("arg (apuntada) : %d  | dir: %p\n", *(int *)arg, arg);
    printf("&arg (pila)    : -  | dir: %p\n", (void *)&arg);
    printf("local_recibida : %d  | dir: %p\n", local_recibida, (void *)&local_recibida);
    printf("nueva_local    : %d | dir: %p\n", nueva_local, (void *)&nueva_local);
    printf("&ptr_hilo      : -  | dir: %p\n", (void *)&ptr_hilo);
    printf("malloc (heap)  : %d | dir: %p\n\n", *ptr_hilo, (void *)ptr_hilo);

    sleep(30);

    free(ptr_hilo);
    return NULL;
}

int main(int argc, char const *argv[])
{
    int local = 5;
    pthread_t hilo1, hilo2;

    printf("HILO PRINCIPAL (PID: %d)\n", getpid());
    printf("global         : %d  | dir: %p\n", global, (void *)&global);
    printf("local          : %d  | dir: %p\n\n", local, (void *)&local);

    pthread_create(&hilo1, NULL, manejadorHilo, &local);
    sleep(1);
    pthread_create(&hilo2, NULL, manejadorHilo, &local);

    pthread_join(hilo1, NULL);
    pthread_join(hilo2, NULL);
    return 0;
}