#ifndef VUELTA_ATRAS_H
#define VUELTA_ATRAS_H

/* Ejecuta el algoritmo de vuelta atrás sin vector usadas. */
int vueltaAtrasSinUsadas(int n, const int B[][n], int soa[],
                         long *nodosVisitados, long *pasosCriterio,
                         long *pasosGenerar, long *pasosSolucion,
                         long *pasosMasHermanos, long *pasosRetroceder);

/* Ejecuta el algoritmo de vuelta atrás con vector usadas. */
int vueltaAtrasConUsadas(int n, const int B[][n], int soa[],
                         long *nodosVisitados, long *pasosCriterio,
                         long *pasosGenerar, long *pasosSolucion,
                         long *pasosMasHermanos, long *pasosRetroceder);

#endif /* VUELTA_ATRAS_H */