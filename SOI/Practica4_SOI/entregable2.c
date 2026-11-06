#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sched.h>
#include <time.h>

#define N_ITERACIONES 1000000000L // 1e9 iteraciones por hilo

// Estructura para pasar parámetros y recoger resultados de cada hilo
typedef struct
{
    int id;
    int grupo;
    double tiempo_ejecucion;
    double pi_aproximado;
} DatosHilo;

// Función auxiliar para calcular tiempo transcurrido en segundos
double calcular_tiempo(struct timespec inicio, struct timespec fin)
{
    return (fin.tv_sec - inicio.tv_sec) + (fin.tv_nsec - inicio.tv_nsec) / 1e9;
}

// Rutina que ejecutará cada hilo
void *montecarlo_pi(void *arg)
{
    DatosHilo *datos = (DatosHilo *)arg;

    // Determinamos el grupo según el id del hilo
    datos->grupo = (datos->id % 4) + 1;

    // Inicializamos una semilla local única por hilo para rand_r
    unsigned int semilla = time(NULL) ^ (datos->id + 1);

    long dentro_circulo = 0;
    struct timespec inicio, fin;

    // Inicio de la medición de tiempo del hilo
    clock_gettime(CLOCK_MONOTONIC, &inicio);

    for (long i = 1; i <= N_ITERACIONES; i++)
    {
        // Generar puntos (x, y) aleatorios en [0, 1] usando rand_r (thread-safe)
        double x = (double)rand_r(&semilla) / RAND_MAX;
        double y = (double)rand_r(&semilla) / RAND_MAX;

        // Comprobar condición del círculo de radio 1: x^2 + y^2 <= 1
        if (x * x + y * y <= 1.0)
        {
            dentro_circulo++;
        }

        // Aplicar la política de sched_yield según el grupo del hilo
        switch (datos->grupo)
        {
        case 1:
            // No llama a sched_yield
            break;
        case 2:
            if (i % 1000 == 0)
                sched_yield();
            break;
        case 3:
            if (i % 100 == 0)
                sched_yield();
            break;
        case 4:
            sched_yield();
            break;
        }
    }

    // Fin de la medición de tiempo
    clock_gettime(CLOCK_MONOTONIC, &fin);

    datos->tiempo_ejecucion = calcular_tiempo(inicio, fin);
    datos->pi_aproximado = 4.0 * ((double)dentro_circulo / N_ITERACIONES);

    pthread_exit(NULL);
}

// Función de comparación para qsort (ordenar de menor a mayor tiempo)
int comparar_tiempos(const void *a, const void *b)
{
    DatosHilo *hiloA = (DatosHilo *)a;
    DatosHilo *hiloB = (DatosHilo *)b;

    if (hiloA->tiempo_ejecucion < hiloB->tiempo_ejecucion)
        return -1;
    else if (hiloA->tiempo_ejecucion > hiloB->tiempo_ejecucion)
        return 1;
    else
        return 0;
}

int main(int argc, char const *argv[])
{
    if (argc != 2)
    {
        printf("Error de formato. Uso correcto: %s <Número_de_Hilos (H)>\n", argv[0]);
        return 1;
    }

    int H = atoi(argv[1]);
    if (H <= 0)
    {
        printf("El número de hilos debe ser mayor que 0.\n");
        return 1;
    }

    pthread_t hilos[H];
    DatosHilo datos[H];

    printf("Creando %d hilos de cálculo Montecarlo con 1e9 iteraciones cada uno...\n\n", H);

    // Crear los H hilos
    for (int i = 0; i < H; i++)
    {
        datos[i].id = i;
        pthread_create(&hilos[i], NULL, montecarlo_pi, (void *)&datos[i]);
    }

    // Esperar a que terminen todos los hilos
    for (int i = 0; i < H; i++)
    {
        pthread_join(hilos[i], NULL);
    }

    // Ordenar el array de datos de menor a mayor tiempo de ejecución
    qsort(datos, H, sizeof(DatosHilo), comparar_tiempos);

    // Mostrar tabla final de resultados
    printf("====================================================================================\n");
    printf("              TABLA RESULTADOS ENTREGABLE 2 (ORDENADO POR TIEMPO)\n");
    printf("====================================================================================\n");
    printf(" ID Hilo  |  Grupo  | Frecuencia yield     | Tiempo Ejecución (s) | Aproximación Pi \n");
    printf("----------+---------+----------------------+----------------------+-----------------\n");

    for (int i = 0; i < H; i++)
    {
        char *desc_yield;
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
        }

        printf("  %-7d | Grupo %d | %-20s | %20.6f | %.15f\n",
               datos[i].id, datos[i].grupo, desc_yield, datos[i].tiempo_ejecucion, datos[i].pi_aproximado);
    }
    printf("====================================================================================\n");

    return 0;
}