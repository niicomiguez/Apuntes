#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <time.h>

void imprimir_con_hora(const char *mensaje)
{
    time_t ahora = time(NULL);
    struct tm *tm_info = localtime(&ahora);
    char hora[9];
    strftime(hora, sizeof(hora), "%H:%M:%S", tm_info);
    printf("[%s] %s", hora, mensaje);
    fflush(stdout);
}

void manejador(int numero_senhal)
{
    char buf[128];
    if (numero_senhal == SIGUSR1)
    {
        sprintf(buf, "[Padre P (PID %d)]: Señal SIGUSR1 recibida\n", getpid());
        imprimir_con_hora(buf);
    }
    else if (numero_senhal == SIGUSR2)
    {
        sprintf(buf, "[Padre P (PID %d)]: Señal SIGUSR2 recibida\n", getpid());
        imprimir_con_hora(buf);
    }
}

int main(int argc, char const *argv[])
{
    struct sigaction sa;
    char buffer[256];

    // sigset_t = variable específica para almacenar conjunto de señalesw
    sigset_t conjunto_bloqueo, pendientes;

    /* Configuración de la estructura sigaction */
    sa.sa_handler = manejador;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;

    if (sigaction(SIGUSR1, &sa, NULL) == -1)
    {
        perror("Error al registrar SIGUSR1");
        exit(1);
    }
    if (sigaction(SIGUSR2, &sa, NULL) == -1)
    {
        perror("Error al registrar SIGUSR2");
        exit(1);
    }

    // Bloquear SIGUSR1 en el padre
    sigemptyset(&conjunto_bloqueo);
    sigaddset(&conjunto_bloqueo, SIGUSR1);
    // Aplicar SIG_BLOCK a todo el conjunto de bloqueo
    sigprocmask(SIG_BLOCK, &conjunto_bloqueo, NULL);

    sprintf(buffer, "[Padre P (PID %d)]: Señal SIGUSR1 bloqueada correctamente.\n", getpid());
    imprimir_con_hora(buffer);

    // Código del hijo h1
    pid_t h1 = fork();
    if (h1 < 0)
    {
        perror("Error al crear hijo h1");
    }
    else if (h1 == 0)
    {
        pid_t pid_abuelo = getppid();

        // Código del nieto
        pid_t n1 = fork();
        if (n1 < 0)
        {
            perror("Error al crear nieto n1");
            exit(1);
        }
        else if (n1 == 0)
        {
            sprintf(buffer, "[N1 (PID %d)]: Enviando SIGUSR2 al abuelo P (PID %d).\n", getpid(), pid_abuelo);
            imprimir_con_hora(buffer);

            kill(pid_abuelo, SIGUSR2);

            sprintf(buffer, "[N1 (PID %d)]: Durmiendo 5 segundos.\n", getpid());
            imprimir_con_hora(buffer);

            sleep(5);

            sprintf(buffer, "[N1 (PID %d)]: Finalizando con exit(20).\n", getpid());
            imprimir_con_hora(buffer);

            exit(20);
        }

        // Continuación H1
        sprintf(buffer, "[H1 (PID %d)]: Enviando SIGUSR1 al padre P (PID %d).\n", getpid(), pid_abuelo);
        imprimir_con_hora(buffer);

        kill(pid_abuelo, SIGUSR1);

        // H1 espera por N1 para hacer exit
        int estado_n1;
        waitpid(n1, &estado_n1, 0);

        if (WIFEXITED(estado_n1))
        {
            sprintf(buffer, "[H1 (PID %d)]: Nieto N1 terminó con código de salida: %d\n", getpid(), WEXITSTATUS(estado_n1));
            imprimir_con_hora(buffer);
        }

        sprintf(buffer, "[H1 (PID %d)]: Finalizando con exit(10).\n", getpid());
        imprimir_con_hora(buffer);

        exit(10);
    }

    // Padre se duerme tres segundos
    sprintf(buffer, "[Padre P (PID %d)]: Durmiendo 3 segundos con SIGUSR1 bloqueada.\n", getpid());
    imprimir_con_hora(buffer);

    sleep(3);

    // Comprobar la lista de llamadas pendientes
    sigpending(&pendientes);

    if (sigismember(&pendientes, SIGUSR1))
    {
        sprintf(buffer, "[Padre P (PID %d)]: Comprobado con sigpending: SIGUSR1 está pendiente de procesar.\n", getpid());
        imprimir_con_hora(buffer);
    }

    // Desbloqueo de la señal SIGUSR1
    sprintf(buffer, "[Padre P (PID %d)]: Desbloqueando SIGUSR1.\n", getpid());
    imprimir_con_hora(buffer);

    sigprocmask(SIG_UNBLOCK, &conjunto_bloqueo, NULL);

    // Padre espera por h1
    int estado_h1;
    waitpid(h1, &estado_h1, 0);

    if (WIFEXITED(estado_h1))
    {
        sprintf(buffer, "[Padre P (PID %d)]: Hijo H1 terminó con código de salida: %d\n", getpid(), WEXITSTATUS(estado_h1));
        imprimir_con_hora(buffer);
    }

    return 0;
}