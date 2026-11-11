#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>
#include <time.h>

int H_GLOBAL;
// Estructura donde los hilos guardan id, inicio, fin y la suma de su bloque
typedef struct
{
    int id;
    long inicio;
    long fin;
    double suma_parcial;
} DatosHilo;

void *calcular_basilea(void *arg)
{
    DatosHilo *datos = (DatosHilo *)arg;
    double suma = 0.0;

    for (long i = datos->inicio; i <= datos->fin; i++)
    {
        // a) Calcular la potencia con pow
        suma += 1.0 / pow((double)i, 2);

        // Comprobación apartado (c): sustituir temporalmente por:
        // suma += 1.0; // Al final el resultado global debe dar exactamente n
    }

    datos->suma_parcial = suma;
    pthread_exit(NULL);
}

// e) Función que ejecuta el hilo que crea el hilo principal para leer las sumas parciales
void *calcular_suma_global(void *arg)
{
    DatosHilo *datos = (DatosHilo *)arg;

    double *suma_total = malloc(sizeof(double));
    *suma_total = 0.0;

    for (int j = 0; j < H_GLOBAL; j++)
    {
        *suma_total += datos[j].suma_parcial;
    }

    pthread_exit((void *)suma_total);
}

// f) Calcular el tiempo transcurrido entre el inicio y fin de las operaciones
double calcular_tiempo(struct timespec inicio, struct timespec fin)
{
    return (fin.tv_sec - inicio.tv_sec) + (fin.tv_nsec - inicio.tv_nsec) / 1e9;
}

int main(int argc, char const *argv[])
{
    if (argc != 3)
    {
        printf("Error de formato. Uso correcto: %s <n(Iteraciones)> <H(Hilos)> \n", argv[0]);
        return 1;
    }

    // b) Número de iteracciones n e hilos H son argumentos
    long n = atol(argv[1]);
    H_GLOBAL = atoi(argv[2]);

    // Validación de entradas
    if (n < 1 || H_GLOBAL < 1)
    {
        printf("n y H deben ser mayores que 0.\n");
        return 1;
    }

    // c) Calcular tamaño de bloque para cada hilo en funcion de n y H
    long bloque = n / H_GLOBAL;
    long resto = n % H_GLOBAL;

    // Inicialización de los datos e hilos
    DatosHilo datos[H_GLOBAL];
    pthread_t hilos[H_GLOBAL];

    long iteraciones_acumuladas = 0;

    for (int j = 0; j < H_GLOBAL; j++)
    {
        // c) Determinamos cuántas iteraciones le tocan al hilo j repartiendo el resto
        long iteraciones_hilo = bloque + (j < resto ? 1 : 0);

        // Inicializar los datos del hilo con el: id, inicio y fin
        datos[j].id = j;
        datos[j].inicio = iteraciones_acumuladas + 1;
        datos[j].fin = datos[j].inicio + iteraciones_hilo - 1;

        iteraciones_acumuladas += iteraciones_hilo;

        printf("[Hilo %d] Hace %ld iteraciones (Rango: %ld a %ld)\n", j, iteraciones_hilo, datos[j].inicio, datos[j].fin);
    }

    // g) Medir el comienzo del cálculo de la ejecución concurrente
    struct timespec inicio_conc, fin_conc;
    clock_gettime(CLOCK_MONOTONIC, &inicio_conc);

    // d) Se lanzan los hilos j a calcular sumatorio en segundo plano y lo guardan en sus datos
    for (int j = 0; j < H_GLOBAL; j++)
    {
        pthread_create(&hilos[j], NULL, calcular_basilea, (void *)&datos[j]);
    }

    // e) Hilo principal espera a que todos los n hilos finalicen su ejecución
    for (int j = 0; j < H_GLOBAL; j++)
    {
        pthread_join(hilos[j], NULL);
    }

    // e) Hilo principal crea un hilo nuevo para calcular el resultado final a partir de todas las contribuciones
    pthread_t hilo_final;
    pthread_create(&hilo_final, NULL, calcular_suma_global, (void *)datos);

    double *suma_total_ptr;
    pthread_join(hilo_final, (void **)&suma_total_ptr);

    double suma_total = *suma_total_ptr;
    free(suma_total_ptr);

    printf("Resultado global calculada por Hilo H+1: %.15f\n", suma_total);

    // g) Medir el final del cálculo de la ejecución concurrente
    clock_gettime(CLOCK_MONOTONIC, &fin_conc);
    double tiempo_conc = calcular_tiempo(inicio_conc, fin_conc);

    // g) Medir el incio del cálculo de la ejecución secuencial
    struct timespec inicio_sec, fin_sec;
    clock_gettime(CLOCK_MONOTONIC, &inicio_sec);

    // f) Hilo principal realiza el cálculo completo y compara
    // f.1) Cálculo secuencial completo por el hilo principal
    double suma_secuencial = 0.0;
    for (long i = 1; i <= n; i++)
    {
        suma_secuencial += 1.0 / pow((double)i, 2);
    }

    // g) Medir el final del cálculo de la ejecución secuencial y calcular speedup
    clock_gettime(CLOCK_MONOTONIC, &fin_sec);
    double tiempo_sec = calcular_tiempo(inicio_sec, fin_sec);
    double speedup = tiempo_sec / tiempo_conc;

    // 2. Valor exacto teórico (pi^2 / 6)
    double valor_exacto = (M_PI * M_PI) / 6.0;

    // 3. Diferencias y errores
    double dif_paralelo_secuencial = fabs(suma_total - suma_secuencial);
    double error_paralelo = fabs(suma_total - valor_exacto);
    double error_secuencial = fabs(suma_secuencial - valor_exacto);

    // 4. Muestra de resultados
    printf("\n=== RESULTADOS Y COMPARATIVA (APARTADO F) ===\n");
    printf("Resultado Concurrente (Paralelo): %.15f\n", suma_total);
    printf("Resultado Secuencial:            %.15f\n", suma_secuencial);
    printf("Valor Exacto Teórico (pi^2/6):   %.15f\n", valor_exacto);
    printf("--------------------------------------------------\n");
    printf("Diferencia (Paralelo vs Secuencial): %.15f\n", dif_paralelo_secuencial);
    printf("Error Absoluto versión Concurrente:  %e\n", error_paralelo);
    printf("Error Absoluto versión Secuencial:   %e\n", error_secuencial);

    // g) Speedup entre secuencial y concurrente
    printf("\n=== TIEMPOS Y SPEEDUP (APARTADO G) ===\n");
    printf("Tiempo Concurrente: %.6f s\n", tiempo_conc);
    printf("Tiempo Secuencial:  %.6f s\n", tiempo_sec);
    printf("Speedup:            %.2fx\n", speedup);

    return 0;
}
