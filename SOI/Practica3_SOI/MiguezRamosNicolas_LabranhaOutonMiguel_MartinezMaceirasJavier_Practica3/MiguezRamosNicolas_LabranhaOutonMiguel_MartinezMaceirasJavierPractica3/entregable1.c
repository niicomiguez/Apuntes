#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

// sig_atomic = variable no se lea nunca con datos corruptos
// en variable normal el SO puede interrumpir la ejecución corrompiendo los datos
// volatile = compilador no optimiza variable haciendo que se lea siempre de la RAM directamente
volatile sig_atomic_t flag_sigusr1 = 0;
volatile sig_atomic_t flag_alarm = 0;

// alarm = temporizador del sistema que despierta al padre para que imprima el valor del numero de procesos
/* Manejador de señales */
void manejador(int numero_senhal)
{
    if (numero_senhal == SIGUSR1)
    {
        flag_sigusr1 = 1;
    }
    else if (numero_senhal == SIGALRM)
    {
        flag_alarm = 1;
    }
}

int main(int argc, char const *argv[])
{
    int num_procesos = 0;
    struct sigaction sa;

    /* Configuración de la estructura sigaction */
    sa.sa_handler = manejador;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;

    /* Registro de los manejadores con sigaction */
    if (sigaction(SIGUSR1, &sa, NULL) == -1)
    {
        perror("Error al registrar SIGUSR1");
        exit(1);
    }
    if (sigaction(SIGALRM, &sa, NULL) == -1)
    {
        perror("Error al registrar SIGALRM");
        exit(1);
    }

    printf("Programa iniciado (PID padre: %d). Esperando señales SIGUSR1...\n", getpid());
    fflush(stdout);

    /* Iniciar el temporizador de 1 segundo */
    alarm(1);

    /* Bucle principal: se ejecuta mientras no superemos los 5 procesos */
    while (num_procesos <= 5)
    {
        // Bloquear padre
        pause();

        /* 1. Atender el paso de 1 segundo (SIGALRM) */
        if (flag_alarm == 1)
        {
            flag_alarm = 0;
            printf("Número de procesos creados = %d\n", num_procesos);
            fflush(stdout);
            alarm(1); /* Rearmar el reloj para el siguiente segundo */
        }

        /* 2. Atender la petición de creación de hijo (SIGUSR1) */
        if (flag_sigusr1 == 1)
        {
            flag_sigusr1 = 0;
            num_procesos++;

            if (num_procesos > 5)
            {
                break;
            }

            pid_t pid_hijo = fork();

            if (pid_hijo < 0)
            {
                perror("Error al ejecutar fork");
            }
            else if (pid_hijo == 0)
            {
                /* --- CÓDIGO DEL HIJO --- */
                /* El hijo se suspende inmediatamente a sí mismo con SIGSTOP */
                raise(SIGSTOP);

                /* Cuando el padre lo reanude al final, ejecutará exit y terminará */
                exit(0);
            }
        }
    }

    /* Cancelar cualquier alarma pendiente para que no vuelva a saltar */
    alarm(0);

    printf("\nLímite superado (%d procesos creados). Finalizando programa y limpiando hijos...\n", num_procesos);
    fflush(stdout);

    // si el padre recibe señal SIGTERM, debe ignorarla (SIG_IGN)
    // si no se ignora SIGTERM el padre se mata a sí mismo
    signal(SIGTERM, SIG_IGN);

    // Matar a todos los procesos del grupo 0
    // Como los hijos están congelados, esta orden se queda en la pila de procesos pendientes
    kill(0, SIGTERM);

    // Indicar que todos los hijos del grupo 0 continuen su ejecución
    // Como ya han despertado se ejecuta el sigterm pendiente
    kill(0, SIGCONT);

    // El padre espera a que TODOS sus hijos hayan finalizado
    while (wait(NULL) > 0)
        ;

    printf("Todos los hijos han finalizado correctamente. Padre termina de forma limpia.\n");

    return 0;
}