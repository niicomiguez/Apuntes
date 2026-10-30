#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <sys/wait.h>
// PRUEBA 1

// int var_global = 10;

// // ÚNICA función para todos los hilos
// void *rutina_hilo(void *arg)
// {
//     int id_hilo = *(int *)arg;

//     if (id_hilo == 1)
//     {
//         var_global = 50; // El hilo 1 modifica
//         printf("[Hilo 1] Variable global cambiada a %d\n", var_global);
//     }
//     else if (id_hilo == 2)
//     {
//         sleep(1);
//         printf("[Hilo 2] Leyendo la variable global: %d\n", var_global);
//     }

//     pthread_exit(NULL);
// }

// int main()
// {
//     pthread_t hilo1, hilo2;
//     int id1 = 1, id2 = 2;

//     // Ambos hilos llaman a 'rutina_hilo', pero reciben distinto ID
//     // rutina_hilo es lo que el hilo ejecuta nada más nacer
//     pthread_create(&hilo1, NULL, rutina_hilo, &id1);
//     pthread_create(&hilo2, NULL, rutina_hilo, &id2);

//     pthread_join(hilo1, NULL);
//     pthread_join(hilo2, NULL);

//     return 0;
// }

// PRUEBA 2

// pthread_t hilo1;
// void *rutina_hilo(void *arg)
// {
//     int id_hilo = *(int *)arg;

//     int min = 1;
//     int max = 5;
//     int numeroRandom = min + rand() % (max - min + 1);
//     sleep(numeroRandom);
//     if (id_hilo == 1)
//     {
//         printf("[Hilo 1] Despierta después de: %d segundos\n", numeroRandom);
//     }
//     else if (id_hilo == 2)
//     {
//         pthread_join(hilo1, NULL);
//         printf("[Hilo 2] Despierta después de: %d segundos\n", numeroRandom);
//     }

//     pthread_exit(NULL);
// }

// int main()
// {
//     pthread_t hilo2;
//     int id1 = 1, id2 = 2;

//     // Ambos hilos llaman a 'rutina_hilo', pero reciben distinto ID
//     // rutina_hilo es lo que el hilo ejecuta nada más nacer
//     pthread_create(&hilo1, NULL, rutina_hilo, &id1);
//     pthread_create(&hilo2, NULL, rutina_hilo, &id2);

//     pthread_join(hilo2, NULL);

//     return 0;
// }

// PRUEBA 3

// ÚNICA función para los tres hilos
// void *rutina_hilo(void *arg)
// {
//     int id = *(int *)arg;

//     if (id == 1 || id == 2)
//     {
//         printf("[Hilo %d] Iniciado y durmiendo 15 segundos...\n", id);
//         sleep(15);
//         printf("[Hilo %d] Terminando.\n", id);
//     }
//     else if (id == 3)
//     {
//         // Damos 1 segundo de margen para garantizar que los hilos 1 y 2 ya están bloqueados en su sleep
//         sleep(1);
//         printf("[Hilo 3] Voy a ejecutar fork()...\n");
//         pid_t pid = fork();

//         if (pid == 0)
//         {
//             // --- ZONA DEL PROCESO HIJO ---
//             printf("\n---> [PROCESO HIJO] Nací. Mi PID es: %d\n", getpid());
//             printf("---> [PROCESO HIJO] Abre OTRA terminal rápido y ejecuta: ps -T -p %d\n\n", getpid());

//             sleep(10); // Damos tiempo para que ejecutes el comando 'ps'

//             printf("---> [PROCESO HIJO] Terminando ejecución.\n");
//             exit(0);
//         }
//         else if (pid > 0)
//         {
//             // --- ZONA DEL HILO 3 ---
//             printf("[Hilo 3] He creado al hijo con PID %d. Ahora YO termino inmediatamente.\n", pid);
//             pthread_exit(NULL); // El hilo 3 muere de forma prematura
//         }
//     }

//     pthread_exit(NULL);
// }

// int main()
// {
//     pthread_t h1, h2, h3;
//     int id1 = 1, id2 = 2, id3 = 3;

//     // Creamos los tres hilos apuntando a la misma función
//     pthread_create(&h1, NULL, rutina_hilo, &id1);
//     pthread_create(&h2, NULL, rutina_hilo, &id2);
//     pthread_create(&h3, NULL, rutina_hilo, &id3);

//     // Esperamos a los hilos 1 y 2
//     pthread_join(h1, NULL);
//     pthread_join(h2, NULL);

//     // Esperamos al proceso hijo para limpiar su estado zombi
//     wait(NULL);

//     return 0;
// }

// PRUEBA 4
void *rutina_hilo(void *arg)
{
    int id = *(int *)arg;

    if (id == 1 || id == 2)
    {
        printf("[Hilo %d] Iniciado. Durmiendo 10 segundos para no terminar antes de tiempo...\n", id);
        // Garantizamos que los hilos no terminen antes de la ejecución del exit del hilo 3
        sleep(10);
        printf("[Hilo %d] Despierto y terminando.\n", id); // Esto jamás llegará a ejecutarse
    }
    else if (id == 3)
    {
        // Damos 2 segundos para asegurar que los hilos 1 y 2 ya están bloqueados en el sleep
        sleep(2);
        printf("[Hilo 3] Ejecutando exit() de forma prematura...\n");

        // Usamos exit en lugar de pthread_exit
        exit(0);
    }

    pthread_exit(NULL);
}

int main()
{
    pthread_t h1, h2, h3;
    int id1 = 1, id2 = 2, id3 = 3;

    // Creamos los tres hilos apuntando a la misma función
    pthread_create(&h1, NULL, rutina_hilo, &id1);
    pthread_create(&h2, NULL, rutina_hilo, &id2);
    pthread_create(&h3, NULL, rutina_hilo, &id3);

    // El hilo principal espera a que terminen
    pthread_join(h1, NULL);
    pthread_join(h2, NULL);
    pthread_join(h3, NULL);

    printf("[Main] Fin del programa (jamás se imprimirá).\n");
    return 0;
}
