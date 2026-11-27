#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>

int main(int argc, char const *argv[])
{
    // Comprobar que se reciben los tres parámetros necesarios por línea de comandos
    if (argc < 4)
    {
        printf("Uso: <M>(enteros) <N>(iteraciones) <P>(procesos)\n");
        return 1;
    }

    // Convertir los argumentos de texto a enteros (M: tamaño, N: iteraciones, P: procesos)
    int valorM = atoi(argv[1]);
    int valorN = atoi(argv[2]);
    int valorP = atoi(argv[3]);

    // Validar que los tres parámetros sean mayores que cero
    if (valorM <= 0 || valorN <= 0 || valorP <= 0)
    {
        printf("Error: M, N y P deben ser enteros mayores que 0.\n");
        return 1;
    }

    // Crear o truncar el fichero vector.txt en modo lectura/escritura
    int fd = open("vector.txt", O_RDWR | O_CREAT | O_TRUNC, 0666);
    if (fd == -1)
    {
        perror("open");
        return 1;
    }

    // Inicializar el fichero escribiendo M enteros con valor 0
    int cero = 0;
    for (int i = 0; i < valorM; i++)
    {
        if (write(fd, &cero, sizeof(int)) != sizeof(int))
        {
            perror("write");
            close(fd);
            return 1;
        }
    }

    // Guardar instante de inicio antes de creación de hijos
    struct timespec inicio, fin;
    if (clock_gettime(CLOCK_MONOTONIC, &inicio) == -1)
    {
        perror("clock_gettime inicio");
        close(fd);
        return 1;
    }

    // Bucle para crear los P procesos hijos trabajadores
    for (int i = 0; i < valorP; i++)
    {
        pid_t pid = fork();
        if (pid == -1)
        {
            perror("Fork fallido");
            close(fd);
            return 1;
        }
        else if (pid == 0)
        {
            // Inicializar la semilla aleatoria usando el PID único de cada hijo
            srand(getpid());

            // Lazo de N iteraciones sobre el vector en fichero
            for (int j = 0; j < valorN; j++)
            {
                // Seleccionar una posición aleatoria y calcular su desplazamiento en bytes
                int pos = rand() % valorM;
                off_t offset = (off_t)pos * sizeof(int);
                int valor = 0;

                // Leer el entero situado en el desplazamiento 'offset' de forma atómica
                if (pread(fd, &valor, sizeof(int), offset) != sizeof(int))
                {
                    perror("pread");
                    close(fd);
                    exit(1);
                }

                // Incrementar su valor en una unidad
                valor++;

                // Escribir el valor actualizado de vuelta en la misma posición
                if (pwrite(fd, &valor, sizeof(int), offset) != sizeof(int))
                {
                    perror("pwrite");
                    close(fd);
                    exit(1);
                }
            }

            // Cerrar el descriptor en el hijo y finalizar para no continuar el bucle del padre
            close(fd);
            exit(0);
        }
    }

    // Padre espera a que terminen todos los hijos
    for (int i = 0; i < valorP; i++)
    {
        wait(NULL);
    }

    if (clock_gettime(CLOCK_MONOTONIC, &fin) == -1)
    {
        perror("clock_gettime fin");
        close(fd);
        return 1;
    }

    // Convertir la diferencia de segundos y nanosegundos a segundos con decimales
    double tiempo = (fin.tv_sec - inicio.tv_sec) + (fin.tv_nsec - inicio.tv_nsec) / 1e9;

    long sumaTotal = 0;

    for (int i = 0; i < valorM; i++)
    {
        int elem = 0;
        if (pread(fd, &elem, sizeof(int), (off_t)i * sizeof(int)) != sizeof(int))
        {
            perror("pread suma");
            close(fd);
            return 1;
        }
        sumaTotal += elem;
    }

    // Cerrar el descriptor en el padre
    if (close(fd) == -1)
    {
        perror("close");
        return 1;
    }

    long esperados = (long)valorP * valorN;
    printf("  Tamano del vector (M) : %d\n", valorM);
    printf("  Iteraciones/hijo  (N) : %d\n", valorN);
    printf("  Procesos hijos    (P) : %d\n", valorP);
    printf("  Tiempo de ejecucion   : %.6f s\n", tiempo);
    printf("------------------------------------------\n");
    printf("  Incrementos esperados : %ld\n", esperados);
    printf("  Incrementos obtenidos : %ld\n", sumaTotal);

    if (sumaTotal == esperados)
    {
        printf("  Estado: [OK] Todos los incrementos realizados.\n");
    }
    else
    {
        printf("  Estado: [ERROR] Carrera critica (-%ld incrementos).\n", esperados - sumaTotal);
    }

    return 0;
}