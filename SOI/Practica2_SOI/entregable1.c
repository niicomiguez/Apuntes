#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>

// variable global
int variable_global = 100;

// escribe una cadena completa en el fichero usando su longitud real en bytes
static void escribir(int fd, const char *msg) {
    if (write(fd, msg, strlen(msg)) < 0)
        perror("Error en write");
}

int main(void) {
    pid_t pid;
    char *env_var = getenv("USER"); // obtiene la variable de entorno USER
    int fd;

    // variable local
    int variable_local = 200;

    // variable dinámica
    int *variable_dinamica = malloc(sizeof(int));
    if (variable_dinamica == NULL) {
        perror("Error al reservar memoria dinámica");
        exit(EXIT_FAILURE);
    }
    *variable_dinamica = 300;

    // el padre abre un fichero antes del fork
    fd = open("fichero_compartido.txt", O_CREAT | O_RDWR | O_TRUNC, 0644);
    if (fd < 0) {
        perror("Error al abrir el fichero en el proceso padre");
        free(variable_dinamica);
        exit(EXIT_FAILURE);
    }

    escribir(fd, "Línea inicial escrita por el proceso padre.\n");

    // estado antes del fork
    printf("ANTES DEL FORK:\n");
    // el offset indica en que posición se realizará la siguiente operación sobre el fichero
    printf("Fichero abierto con descriptor (fd): %d | Offset: %ld\n", fd, (long)lseek(fd, 0, SEEK_CUR));
    printf("Variable global:   %d | Dirección virtual: %p\n", variable_global, (void *)&variable_global);
    printf("Variable local:    %d | Dirección virtual: %p\n", variable_local, (void *)&variable_local);
    printf("Variable dinámica: %d | Dirección virtual: %p\n", *variable_dinamica, (void *)variable_dinamica);

    // vaciar el buffer de salida para que el hijo no duplique estas líneas
    fflush(stdout);

    pid = fork();

    if (pid < 0) {
        perror("Error al ejecutar fork");
        close(fd);
        free(variable_dinamica);
        exit(EXIT_FAILURE);
    }
    else if (pid == 0) { // proceso hijo
        int lectura_hijo = 0;

        printf("\nPROCESO HIJO:\n");
        printf("PID: %d | PPID: %d | UID: %d | EUID: %d | GID: %d | USER: %s\n", getpid(), getppid(), getuid(), geteuid(), getgid(), env_var ? env_var : "No definida");
        printf("Fichero abierto con descriptor (fd): %d | Offset al empezar: %ld\n", fd, (long)lseek(fd, 0, SEEK_CUR));

        escribir(fd, "Texto añadido por el proceso hijo.\n");
        printf("Offset tras escribir el hijo: %ld\n", (long)lseek(fd, 0, SEEK_CUR));

        variable_global = 400;
        variable_local = 500;
        *variable_dinamica = 600;

        printf("Variable global modificada (hijo):   %d | Dirección virtual: %p\n", variable_global, (void *)&variable_global);
        printf("Variable local modificada (hijo):    %d | Dirección virtual: %p\n", variable_local, (void *)&variable_local);
        printf("Variable dinámica modificada (hijo): %d | Dirección virtual: %p\n", *variable_dinamica, (void *)variable_dinamica);

        printf("Introduce un número por teclado (hijo): ");
        fflush(stdout);
        if (scanf("%d", &lectura_hijo) == 1)
            printf("Leído correctamente el valor (hijo): %d\n", lectura_hijo);
        else
            printf("Error en la lectura (hijo).\n");

        sleep(2); // tiempo para comprobar con ps desde otro terminal

        close(fd);
        free(variable_dinamica);
        exit(EXIT_SUCCESS);
    }
    else { // proceso padre
        int lectura_padre = 0;

        printf("\nPROCESO PADRE:\n");
        printf("PID: %d | PPID: %d | UID: %d | EUID: %d | GID: %d | USER: %s\n", getpid(), getppid(), getuid(), geteuid(), getgid(), env_var ? env_var : "No definida");
        printf("Fichero abierto con descriptor (fd): %d\n", fd);

        // ambos procesos llegan al scanf a la vez y compiten por el teclado
        printf("Introduce un número por teclado (padre): ");
        fflush(stdout);
        if (scanf("%d", &lectura_padre) == 1)
            printf("Leído correctamente el valor (padre): %d\n", lectura_padre);
        else
            printf("Error en la lectura (padre).\n");

        // esperar a que termine el hijo para una salida ordenada
        wait(NULL);

        // el offset incluye lo escrito por el hijo: la descripción de fichero es compartida
        printf("Offset en el padre tras terminar el hijo: %ld\n", (long)lseek(fd, 0, SEEK_CUR));
        escribir(fd, "Texto final añadido por el proceso padre.\n");

        printf("Variable global (padre):   %d | Dirección virtual: %p\n", variable_global, (void *)&variable_global);
        printf("Variable local (padre):    %d | Dirección virtual: %p\n", variable_local, (void *)&variable_local);
        printf("Variable dinámica (padre): %d | Dirección virtual: %p\n", *variable_dinamica, (void *)variable_dinamica);

        close(fd);
        free(variable_dinamica);
    }

    return 0;
}