#include <stdio.h>
#include <stdlib.h> // para exit
#include <unistd.h> // fork, getpid, sleep
#include <sys/types.h> // pid_t
#include <sys/wait.h> // wait, WEXITSTATUS

int main(void) {
    pid_t pid1 = fork(); // hijo 1

    if (pid1 < 0) { // caso error
        perror("fork");
        exit(1); // 1 para indicar fallo
    }
    if (pid1 == 0) { // el hijo
        printf("(hijo 1) PID = %d, termina\n", getpid());
        exit(5); // numero arbitrario para identificar al hijo
    }

    pid_t pid2 = fork(); // hijo 2 (huerfano)
    // solo el padre llega aqui, el hijo 1 ya murio

    if (pid2 < 0) { // caso error
        perror("fork");
        exit(1);
    }
    else if (pid2 == 0) { // el hijo
        printf("(hijo 2) PID = %d, PPID = %d, vivo\n", getpid(), getppid());
        sleep(60); // como vive mas que el padre (30s) quedará huerfano
        printf("(hijo 2) PPID = %d\n", getppid()); // ahora sera el 1 porque es adoptado
        execl("/bin/sleep", "sleep", "20", NULL); // mantendrá el mismo PID, tiempo para comprobar en ps
        printf("(hijo 2) execl fail\n");
        exit(0); // si execl falla
    }

    printf("(padre) PID = %d, hijo 1 = %d\n", getpid(), pid1);
    sleep(30); // tiempo para mirar el comando ps, tiempo zombie

    int status;
    pid_t hijo = wait(&status); // espera a que termine un hijo y recoge el 5, se borra el zombie
    printf("(padre) wait devuelve PID = %d, exit = %d\n", hijo, WEXITSTATUS(status));
    // wexitstatus es el codigo de exit (5 para el hijo1)
    return 0;

}