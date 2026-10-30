#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>

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
    datos->suma_parcial = 0.0;

    for (long i = datos->inicio; i <= datos->fin; i++)
    {
        // Calculaar la potencia con pow
        datos->suma_parcial += 1.0 / pow((double)i, 2);
    }

    pthread_exit(NULL);
}

int main(int argc, char const *argv[])
{
    if (argc != 3)
    {
        printf("Error de formato. Uso correcto: %s <n(Iteraciones)> <H(Hilos)> \n", argv[0]);
        return 1;
    }

    long n = atol(argv[1]);
    int H = atoi(argv[2]);

    // Calcular tamaño de bloque para cada hilo en funcion de n y H
    long bloque = n / H;
    long resto = n % H;

    // Inicialización de los datos e hilos
    DatosHilo datos[H];
    pthread_t hilos[H];

    long iteraciones_acumuladas = 0;
    for (int j = 0; j < H; j++)
    {
        // Determinamos cuántas iteraciones le tocan al hilo j
        long iteraciones_hilo = bloque + (j < resto ? 1 : 0);

        // Inicializar los datos del hilo con el: id, inicio y fin
        datos[j].id = j;
        datos[j].inicio = iteraciones_acumuladas + 1;
        datos[j].fin = datos[j].inicio + iteraciones_hilo - 1;

        iteraciones_acumuladas += iteraciones_hilo;

        printf("[Hilo %d] Hace %ld iteraciones (Rango: %ld a %ld)\n", j, iteraciones_hilo, datos[j].inicio, datos[j].fin);

        // Se lanza el hilo j a calcular en segundo plano
        pthread_create(&hilos[j], NULL, calcular_basilea, (void *)&datos[j]);
    }

    // Hilo principal espera a que todos los n hilos finalicen su ejecución
    for (int j = 0; j < H; j++)
    {
        pthread_join(hilos[j], NULL);
    }

    // Hilo principal muestra el resultado de sumar todas las sumas parciales de nos hijos
    double suma_total = 0.0;
    for (int j = 0; j < H; j++)
    {
        suma_total += datos[j].suma_parcial;
    }

    printf("Resultado global de la serie de Basilea: %.15f\n", suma_total);

    // Hilo principal realiza el cálculo completo y compara
    // 1. Cálculo secuencial completo por el hilo principal
    double suma_secuencial = 0.0;
    for (long i = 1; i <= n; i++)
    {
        suma_secuencial += 1.0 / pow((double)i, 2);
    }

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
    printf("Error Relativo versión Concurrente:  %e\n", error_paralelo);
    printf("Error Relativo versión Secuencial:   %e\n", error_secuencial);

    return 0;
}
