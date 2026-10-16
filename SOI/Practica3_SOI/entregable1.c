#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

/* Banderas globales modificables únicamente dentro de los manejadores (Nota 1) */
volatile sig_atomic_t flag_sigusr1 = 0;
volatile sig_atomic_t flag_alarm = 0;

// alarm nos vale para contador el número de procesos creador por segundo
// ya que el padre está en pausa y no los puede contar
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
        pause(); /* El padre se bloquea */

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

            /* --- CÓDIGO DEL PADRE --- */
            /* Si ya hemos creado más de 5 procesos, salimos del bucle */
            if (num_procesos > 5)
            {
                break;
            }
        }
    }

    /* Cancelar cualquier alarma pendiente para que no vuelva a saltar */
    alarm(0);

    printf("\nLímite superado (%d procesos creados). Finalizando programa y limpiando hijos...\n", num_procesos);
    fflush(stdout);

    /*
     * Limpieza de hijos sin almacenar sus PIDs:
     * Enviamos la señal SIGCONT a todo nuestro grupo de procesos (pasando 0 a kill).
     * Esto reanuda a todos los hijos que estaban detenidos con SIGSTOP para que
     * alcancen la línea exit(0) y finalicen solos.
     */
    kill(0, SIGCONT);

    /* El padre espera a que absolutamente TODOS sus hijos hayan finalizado */
    while (wait(NULL) > 0)
        ;

    printf("Todos los hijos han finalizado correctamente. Padre termina de forma limpia.\n");

    return 0;
}