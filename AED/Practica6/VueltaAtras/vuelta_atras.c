#include "vuelta_atras.h"

static void reiniciarContadores(long *nodosVisitados, long *pasosCriterio,
                                long *pasosGenerar, long *pasosSolucion,
                                long *pasosMasHermanos, long *pasosRetroceder) {
    *nodosVisitados = 1;
    *pasosCriterio = 0;
    *pasosGenerar = 0;
    *pasosSolucion = 0;
    *pasosMasHermanos = 0;
    *pasosRetroceder = 0;
}

// ### FUNCIONES VERSIÓN SIN USADAS ###
// s[1] = 2 -> creador 1 tiene la actividad 2
static void GenerarSinUsadas(int nivel, int s[], int *bact, int n, const int B[n][n], long *pasosGenerar) {
    (*pasosGenerar)++;
    // Evitar que se sumen valores repetidos a bact
    if (s[nivel] != -1) {
        *bact -= B[nivel][s[nivel]];
    }
    s[nivel]++;
    *bact += B[nivel][s[nivel]];
}

// Comprobar que los creadores anteriores a s[nivel] no tienen la misma tarea asignada
static int CriterioSinUsadas(int nivel, const int s[], long *pasosCriterio) {
    (*pasosCriterio)++;
    for (int i = 0; i < nivel; i++) {
        (*pasosCriterio)++;
        if (s[nivel] == s[i]) {
            return 0;
        }
    }
    return 1;
}

// Una solución es válida cando estamos en el nivel final y se cumple el criterio
static int SolucionSinUsadas(int nivel, int n, const int s[], long *pasosSolucion, long *pasosCriterio) {
    (*pasosSolucion)++;
    return (nivel == n - 1) && CriterioSinUsadas(nivel, s, pasosCriterio);
}

// Devuelve 1 si al creador actual le quedan actividades por probar
static int MasHermanos(int nivel, const int s[], int n, long *pasosMasHermanos) {
    (*pasosMasHermanos)++;
    return s[nivel] < n - 1;
}

// Retrocede un nivel, quitando la acumulacion de bact y poniendo el valor del nivel a -1
static void RetrocederSinUsadas(int *nivel, int s[], int *bact, int n, const int B[][n], long *pasosRetroceder) {
    (*pasosRetroceder)++;
    *bact -= B[*nivel][s[*nivel]];
    s[*nivel] = -1;
    (*nivel)--;
}

int vueltaAtrasSinUsadas(int n, const int B[][n], int soa[],
                         long *nodosVisitados, long *pasosCriterio,
                         long *pasosGenerar, long *pasosSolucion,
                         long *pasosMasHermanos, long *pasosRetroceder) {
    reiniciarContadores(nodosVisitados, pasosCriterio, pasosGenerar,
                        pasosSolucion, pasosMasHermanos, pasosRetroceder);

    int s[n];
    for (int i = 0; i < n; i++) {
        s[i] = -1;
        soa[i] = -1;
    }

    int nivel = 0;
    int bact = 0;
    int voa = -1;

    do {
        GenerarSinUsadas(nivel, s, &bact, n, B, pasosGenerar);

        if (SolucionSinUsadas(nivel, n, s, pasosSolucion, pasosCriterio) && (bact > voa)) {
            voa = bact;
            for (int i = 0; i < n; i++) {
                soa[i] = s[i];
            }
        }

        if (CriterioSinUsadas(nivel, s, pasosCriterio)) {
            (*nodosVisitados)++;
            if (nivel < n - 1) {
                nivel++;
            }
        }

        while (nivel >= 0 && !MasHermanos(nivel, s, n, pasosMasHermanos)) {
            RetrocederSinUsadas(&nivel, s, &bact, n, B, pasosRetroceder);
        }
    } while (nivel >= 0);

    return voa;
}

// ### FUNCIONES VERSIÓN CON USADAS ###
// usadas[actividad] = número de creadores; guardar el número de creadores que tienen asignada una activad en un momento determinado
static void GenerarConUsadas(int nivel, int s[], int *bact, int n, const int B[n][n], int usadas[], long *pasosGenerar) {
    (*pasosGenerar)++;
    // Evitar que se sumen valores repetidos a bact y quita uno de usadas
    if (s[nivel] != -1) {
        *bact -= B[nivel][s[nivel]];
        usadas[s[nivel]]--;
    }
    s[nivel]++;
    *bact += B[nivel][s[nivel]];
    usadas[s[nivel]]++;
}

// Devuelve 1 si solo hay un creador por actividad
static int CriterioConUsadas(int nivel, const int s[], const int usadas[], long *pasosCriterio) {
    (*pasosCriterio)++;
    return usadas[s[nivel]] == 1;
}

// Una solución es válida cando estamos en el nivel final y se cumple el criterio
static int SolucionConUsadas(int nivel, int n, const int s[], const int usadas[], long *pasosSolucion, long *pasosCriterio) {
    (*pasosSolucion)++;
    return (nivel == n - 1) && CriterioConUsadas(nivel, s, usadas, pasosCriterio);
}

// Retroceder un nivel, restando el bact del nivel y usadas -1
static void RetrocederConUsadas(int *nivel, int s[], int *bact, int n, const int B[][n], int usadas[], long *pasosRetroceder) {
    (*pasosRetroceder)++;
    *bact -= B[*nivel][s[*nivel]];
    usadas[s[*nivel]]--;
    s[*nivel] = -1;
    (*nivel)--;
}

int vueltaAtrasConUsadas(int n, const int B[][n], int soa[],
                         long *nodosVisitados, long *pasosCriterio,
                         long *pasosGenerar, long *pasosSolucion,
                         long *pasosMasHermanos, long *pasosRetroceder) {
    reiniciarContadores(nodosVisitados, pasosCriterio, pasosGenerar,
                        pasosSolucion, pasosMasHermanos, pasosRetroceder);

    int s[n];
    int usadas[n];
    for (int i = 0; i < n; i++) {
        s[i] = -1;
        soa[i] = -1;
        usadas[i] = 0;
    }

    int nivel = 0;
    int bact = 0;
    int voa = -1;

    do {
        GenerarConUsadas(nivel, s, &bact, n, B, usadas, pasosGenerar);

        if (SolucionConUsadas(nivel, n, s, usadas, pasosSolucion, pasosCriterio) && (bact > voa)) {
            voa = bact;
            for (int i = 0; i < n; i++) {
                soa[i] = s[i];
            }
        }

        if (CriterioConUsadas(nivel, s, usadas, pasosCriterio)) {
            (*nodosVisitados)++;
            if (nivel < n - 1) {
                nivel++;
            }
        }

        while (nivel >= 0 && !MasHermanos(nivel, s, n, pasosMasHermanos)) {
            RetrocederConUsadas(&nivel, s, &bact, n, B, usadas, pasosRetroceder);
        }
    } while (nivel >= 0);

    return voa;
}