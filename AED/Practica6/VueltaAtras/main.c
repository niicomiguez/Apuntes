#include <stdio.h>
#include "vuelta_atras.h"

static void mostrarResultados(int n, const int B[][n], const int soa[], int voa,
                              int conUsadas, const char *creadores[], const char *actividades[],
                              long nodosVisitados, long pasosCriterio,
                              long pasosGenerar, long pasosSolucion,
                              long pasosMasHermanos, long pasosRetroceder) {
    printf("=== Prueba n = %d (%s vector usadas) ===\n",
           n, conUsadas ? "con" : "sin");
    printf("Beneficio maximo: %d millones de visualizaciones\n", voa);
    printf("Vector s = [");
    for (int i = 0; i < n; i++) {
        printf("%d%s", soa[i], (i < n - 1) ? ", " : "");
    }
    printf("]\n");

    for (int i = 0; i < n; i++) {
        int act = soa[i];
        printf("  Creador %d (%-10s) -> Actividad %d (%-35s) [%d M]\n",
               i + 1, creadores[i], act + 1, actividades[act], B[i][act]);
    }

    printf("\nMetricas (n = %d, usadas = %s):\n", n, conUsadas ? "Si" : "No");
    printf("  Nodos visitados : %ld\n", nodosVisitados);
    printf("  Criterio        : %ld\n", pasosCriterio);
    printf("  Generar         : %ld\n", pasosGenerar);
    printf("  Solucion        : %ld\n", pasosSolucion);
    printf("  Mas hermanos    : %ld\n", pasosMasHermanos);
    printf("  Retroceder      : %ld\n\n", pasosRetroceder);
}

int main(void) {
    long nodosVisitados = 1;
    long pasosCriterio = 0;
    long pasosGenerar = 0;
    long pasosSolucion = 0;
    long pasosMasHermanos = 0;
    long pasosRetroceder = 0;

    const char *creadores[7] = {
        "LunaByte", "KiroBeat", "NekoRush", "AdaLoop",
        "PixelNova", "CodeSpark", "TechnoWave"
    };

    const char *actividades[7] = {
        "Directo cooperativo",
        "Taller de musica digital",
        "Torneo de videojuegos",
        "Podcast tecnologico",
        "Exposicion de arte digital",
        "Taller de desarrollo de videojuegos",
        "Concierto de musica electronica"
    };

    const int matriz3[3][3] = {
        {11, 17,  8},
        { 9,  7,  6},
        {13, 15, 16}
    };

    const int matriz7[7][7] = {
        {11, 17,  8, 16, 20, 14,  9},
        { 9,  7,  6, 12, 15, 18, 18},
        {13, 15, 16, 12, 16, 18, 19},
        {21, 24, 28, 17, 26, 20, 22},
        {10, 14, 12, 11, 15, 13, 16},
        {12, 20, 19, 13, 22, 17, 20},
        { 7, 21, 13,  5, 34,  9, 11}
    };

    int soa3[3];
    int voa3 = vueltaAtrasSinUsadas(3, matriz3, soa3,
                                    &nodosVisitados, &pasosCriterio,
                                    &pasosGenerar, &pasosSolucion,
                                    &pasosMasHermanos, &pasosRetroceder);
    mostrarResultados(3, matriz3, soa3, voa3, 0, creadores, actividades,
                      nodosVisitados, pasosCriterio, pasosGenerar,
                      pasosSolucion, pasosMasHermanos, pasosRetroceder);

    int voa3Usadas = vueltaAtrasConUsadas(3, matriz3, soa3,
                                          &nodosVisitados, &pasosCriterio,
                                          &pasosGenerar, &pasosSolucion,
                                          &pasosMasHermanos, &pasosRetroceder);
    mostrarResultados(3, matriz3, soa3, voa3Usadas, 1, creadores, actividades,
                      nodosVisitados, pasosCriterio, pasosGenerar,
                      pasosSolucion, pasosMasHermanos, pasosRetroceder);

    int soa7[7];
    int voa7 = vueltaAtrasSinUsadas(7, matriz7, soa7,
                                    &nodosVisitados, &pasosCriterio,
                                    &pasosGenerar, &pasosSolucion,
                                    &pasosMasHermanos, &pasosRetroceder);
    mostrarResultados(7, matriz7, soa7, voa7, 0, creadores, actividades,
                      nodosVisitados, pasosCriterio, pasosGenerar,
                      pasosSolucion, pasosMasHermanos, pasosRetroceder);

    int voa7Usadas = vueltaAtrasConUsadas(7, matriz7, soa7,
                                          &nodosVisitados, &pasosCriterio,
                                          &pasosGenerar, &pasosSolucion,
                                          &pasosMasHermanos, &pasosRetroceder);
    mostrarResultados(7, matriz7, soa7, voa7Usadas, 1, creadores, actividades,
                      nodosVisitados, pasosCriterio, pasosGenerar,
                      pasosSolucion, pasosMasHermanos, pasosRetroceder);

    return 0;
}