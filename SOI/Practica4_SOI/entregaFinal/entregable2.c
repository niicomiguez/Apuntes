#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sched.h>
#include <time.h>

// Cada hilo genera 1.000 millones de puntos Montecarlo por defecto
#ifndef N_ITERACIONES
#define N_ITERACIONES 1000000000L
#endif

// Estructura para almacenar los parámetros de entrada y los resultados de cada hilo
typedef struct
{
    int id;
    int grupo;
    double tiempo;
    double aproximacion_pi;
} DatosHilo;

// Rutina que ejecuta cada hilo: aproximación de Pi por Montecarlo y prueba de sched_yield
void *calcular_pi(void *arg)
{
    DatosHilo *datos = (DatosHilo *)arg;

    // Asignación de grupo según la regla h % 4 (1 a 4)
    datos->grupo = (datos->id % 4) + 1;

    // Generación de semilla única por hilo
    // para garantizar secuencias completamente independientes al usar rand_r
    unsigned int semilla = (unsigned int)time(NULL) ^
                           ((unsigned int)datos->id + 1u) * 2654435761u;

    long dentro_circulo = 0;
    struct timespec inicio, fin;

    // Medición de tiempo individual de la fase de cómputo del hilo
    clock_gettime(CLOCK_MONOTONIC, &inicio);

    for (long i = 1; i <= N_ITERACIONES; i++)
    {
        // Generación de coordenadas (x, y) en el cuadrado [0, 1] x [0, 1]
        double x = (double)rand_r(&semilla) / RAND_MAX;
        double y = (double)rand_r(&semilla) / RAND_MAX;

        // Comprobación de inclusión dentro del círculo unidad (x^2 + y^2 <= 1)
        if (x * x + y * y <= 1.0)
        {
            dentro_circulo++;
        }

        // Aplicación del comportamiento de cesión de CPU según el grupo de pertenencia
        switch (datos->grupo)
        {
        case 1: // Grupo 1 (h%4 == 0): No llama a sched_yield (Competitivo / Acaparador)
            break;
        case 2: // Grupo 2 (h%4 == 1): Llama a sched_yield cada 1000 iteraciones
            if (i % 1000 == 0)
                sched_yield();
            break;
        case 3: // Grupo 3 (h%4 == 2): Llama a sched_yield cada 100 iteraciones
            if (i % 100 == 0)
                sched_yield();
            break;
        case 4: // Grupo 4 (h%4 == 3): Llama a sched_yield en todas las iteraciones (Generoso)
            sched_yield();
            break;
        }
    }

    // Fin de la medición de tiempo de ejecución del hilo
    clock_gettime(CLOCK_MONOTONIC, &fin);

    datos->tiempo = (fin.tv_sec - inicio.tv_sec) +
                    (fin.tv_nsec - inicio.tv_nsec) / 1e9;

    // Cálculo de Pi (4 * proporción de puntos en el círculo)
    datos->aproximacion_pi = 4.0 * ((double)dentro_circulo / N_ITERACIONES);

    pthread_exit(NULL);
}

// Función de comparación para qsort (ordenar de menor a mayor tiempo de ejecución)
int comparar_tiempos(const void *a, const void *b)
{
    const DatosHilo *hiloA = (const DatosHilo *)a;
    const DatosHilo *hiloB = (const DatosHilo *)b;

    if (hiloA->tiempo < hiloB->tiempo)
        return -1;
    if (hiloA->tiempo > hiloB->tiempo)
        return 1;
    return 0;
}

int main(int argc, char *argv[])
{
    int H;
    char sobrante;

    // Validación estricta de argumentos de entrada
    if (argc != 2 || sscanf(argv[1], "%d %c", &H, &sobrante) != 1 || H < 1)
    {
        printf("Error de formato. Uso correcto: %s <Número_de_Hilos (H)>\n", argv[0]);
        return 1;
    }

    // Reserva dinámica de memoria en heap (evita desbordamientos de pila / stack overflow)
    pthread_t *hilos = malloc((size_t)H * sizeof(pthread_t));
    DatosHilo *datos = malloc((size_t)H * sizeof(DatosHilo));

    if (hilos == NULL || datos == NULL)
    {
        fprintf(stderr, "Error crítico: no se pudo reservar memoria.\n");
        free(hilos);
        free(datos);
        return 1;
    }

    printf("Iniciando %d hilos con %ld iteraciones Montecarlo cada uno...\n\n", H, (long)N_ITERACIONES);

    // Creación y puesta en marcha de los H hilos
    int creados = 0;
    for (int i = 0; i < H; i++)
    {
        datos[i].id = i;
        if (pthread_create(&hilos[i], NULL, calcular_pi, &datos[i]) != 0)
        {
            fprintf(stderr, "Error al crear el hilo %d.\n", i);
            break;
        }
        creados++;
    }

    // Esperar la finalización de los hilos que se han creado correctamente
    for (int i = 0; i < creados; i++)
    {
        pthread_join(hilos[i], NULL);
    }

    // Si falló la creación de algún hilo, liberamos recursos y salimos
    if (creados != H)
    {
        free(hilos);
        free(datos);
        return 1;
    }

    // Ordenar los resultados en el array de menor a mayor según el tiempo transcurrido
    qsort(datos, (size_t)H, sizeof(DatosHilo), comparar_tiempos);

    // Impresión formateada de la tabla final exigida
    printf("======================================================================================\n");
    printf("                 TABLA RESULTADOS ENTREGABLE 2 (ORDENADO POR TIEMPO)\n");
    printf("======================================================================================\n");
    printf(" ID Hilo  |  Grupo  | Frecuencia sched_yield | Tiempo Ejecución (s) | Aproximación Pi \n");
    printf("----------+---------+------------------------+----------------------+-----------------\n");

    for (int i = 0; i < H; i++)
    {
        const char *desc_yield;
        switch (datos[i].grupo)
        {
        case 1:
            desc_yield = "Sin yield";
            break;
        case 2:
            desc_yield = "Cada 1000 iteraciones";
            break;
        case 3:
            desc_yield = "Cada 100 iteraciones";
            break;
        case 4:
            desc_yield = "Todas las iteraciones";
            break;
        default:
            desc_yield = "Desconocido";
            break;
        }

        printf("  %-7d | Grupo %d | %-22s | %20.6f | %.15f\n",
               datos[i].id, datos[i].grupo, desc_yield, datos[i].tiempo, datos[i].aproximacion_pi);
    }
    printf("======================================================================================\n");

    // Liberación limpia de memoria asignada dinámicamente
    free(hilos);
    free(datos);

    return 0;
}