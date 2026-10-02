#include <stdio.h>
#include <stdlib.h> 
#include <unistd.h>  
#include <math.h>    
#include <sys/time.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    // Comprobación formato
    if (argc != 4) {
        printf("Error de formato. Uso correcto: %s <N> <P> <fichero>\n", argv[0]);
        return 1; 
    }

    // Convertir a enteros con atoi
    // argv[0] = ejecutable | argv[1] = N | argv[2] = P
    int N = atoi(argv[1]);
    int P = atoi(argv[2]);
    char *nombre_fichero = argv[3];

    if (N <= 0 || P <= 0) {
        printf("Error: N y P deben ser números naturales mayores que 0.\n");
        return 1;
    }

    // pid en vez de int para tener portabilidad
    pid_t pid;

    int bloque = N / P; 
    int resto = N % P; 
    
    // Primer numero natural es 1
    int inicio_actual = 1;

    // Abrir fichero en modo escritura
    FILE *fichero = fopen(nombre_fichero, "w");
    if (fichero == NULL) {
        printf("Error al crear o abrir el fichero.\n");
        return 1;
    }

    //---- ZONA HIJOS P ----
    for (int i = 0; i < P; i++) {    
        // Cálculo de números que le tocan a cada hijo
        int cantidad = bloque;

        //Primer hijo hace cálculo para posible resto
        if (i == 0) {
            cantidad = cantidad + resto;
        }
        
        // Calculamos el número final
        int fin_actual = inicio_actual + cantidad - 1;

        pid = fork();

        if (pid < 0) {
            printf("Error al crear el proceso hijo.\n");
            exit(1);
        } else if (pid == 0) {

            printf("Hijo %d (PID %d): calcula desde el %d hasta el %d\n", i, getpid(), inicio_actual, fin_actual);
            
            // Variables para medir el tiempo
            struct timeval tiempo_inicio, tiempo_fin;

            // Inicio cronómetro
            gettimeofday(&tiempo_inicio, NULL);

            double suma = 0.0;
            
            for (int j = inicio_actual; j <= fin_actual; j++) {
                // Cálculo de la tangente de la raíz cuadrada en doble precisión
                suma += tan(sqrt((double)j)); 
            }

            // Calculamos la media del bloque
            double media = suma / cantidad;

            // Fin cronómetro
            gettimeofday(&tiempo_fin, NULL);

            // Cálculo de diferencia (ms)
            long tiempo_us = (tiempo_fin.tv_sec - tiempo_inicio.tv_sec) * 1000000 + 
                             (tiempo_fin.tv_usec - tiempo_inicio.tv_usec);

            // Mostrar resultado
            printf("[HIJO %d - PID %d] Rango [%d - %d] | Media: %f | Tiempo: %ld us\n", 
                   i, getpid(), inicio_actual, fin_actual, media, tiempo_us);

            // Escritura en fichero
            fprintf(fichero, "PID_HIJO: %d | Rango: %d al %d | Media: %f | Tiempo: %ld us\n", 
                    getpid(), inicio_actual, fin_actual, media, tiempo_us);

            fflush(fichero);
            exit(0);
        }
        
        // Inicio del siguiente es el fin del anterior +1 
        inicio_actual = fin_actual + 1;
    }
    for (int i = 0; i < P; i++) {
        wait(NULL); 
    }
    fclose(fichero); 
    // ---- FIN ZONA HIJOS P ----

    // ---- ZONA HIJO P+1 ----
    pid = fork();

    if (pid < 0) {
        printf("Error al crear el proceso P+1.\n");
        exit(1);
    } else if (pid == 0) {
        // Inicio cronómetro
        struct timeval tiempo_inicio_p1, tiempo_fin_p1;
        gettimeofday(&tiempo_inicio_p1, NULL);

        // Abrir fichero modo lectura
        FILE *fichero_lectura = fopen(nombre_fichero, "r");
        if (fichero_lectura == NULL) {
            printf("Error al abrir el fichero para leer.\n");
            exit(1);
        }

        double suma_total_reconstruida = 0.0;
        
        int pid_leido, inicio_leido, fin_leido;
        double media_leida;
        long tiempo_leido;

        // Leer los datos de los P hijos anteriores
        for (int i = 0; i < P; i++) {

            fscanf(fichero_lectura, "PID_HIJO: %d | Rango: %d al %d | Media: %lf | Tiempo: %ld us\n", 
                   &pid_leido, &inicio_leido, &fin_leido, &media_leida, &tiempo_leido);
            
            int cantidad_calculada = (fin_leido - inicio_leido) + 1;
            suma_total_reconstruida += (media_leida * cantidad_calculada);
        }
        fclose(fichero_lectura);

        // Cálculo media final
        double media_final = suma_total_reconstruida / N;

        // --- Fin cronómetro ---
        gettimeofday(&tiempo_fin_p1, NULL);
        long tiempo_us_p1 = (tiempo_fin_p1.tv_sec - tiempo_inicio_p1.tv_sec) * 1000000 + 
                            (tiempo_fin_p1.tv_usec - tiempo_inicio_p1.tv_usec);
        
        // Mostrar resultado con tiempo
        printf("[HIJO P+1 - PID %d] Resultado global calculado: %f | Tiempo: %ld us\n", getpid(), media_final, tiempo_us_p1);

        // Añadir resultado final y tiempo en el archivo
        FILE *fichero_escritura = fopen(nombre_fichero, "a");
        fprintf(fichero_escritura, "\n---> [RESULTADO GLOBAL CONCURRENTE] Hijo P+1 (PID %d) | Media final: %f | Tiempo: %ld us\n\n", 
                getpid(), media_final, tiempo_us_p1);
        fclose(fichero_escritura);

        exit(0);
    }

    // Padre espera a que hijo P+1 acabe
    wait(NULL);

    // ---- FIN ZONA HIJO P+1 ----
    
    // ---- ZONA HIJO P+2  ----
    pid = fork();

    if (pid < 0) {
        printf("Error al crear el proceso P+2.\n");
        exit(1);
    } else if (pid == 0) {
        struct timeval tiempo_inicio_seq, tiempo_fin_seq;
        gettimeofday(&tiempo_inicio_seq, NULL); 

        double suma_secuencial = 0.0;
        
        // Un solo bucle para calcular desde el 1 hasta N
        for (int j = 1; j <= N; j++) {
            suma_secuencial += tan(sqrt((double)j)); 
        }

        double media_secuencial = suma_secuencial / N;

        gettimeofday(&tiempo_fin_seq, NULL); 
        
        long tiempo_us_seq = (tiempo_fin_seq.tv_sec - tiempo_inicio_seq.tv_sec) * 1000000 + 
                             (tiempo_fin_seq.tv_usec - tiempo_inicio_seq.tv_usec);

        printf("[HIJO P+2 - PID %d] Resultado SECUENCIAL: %f | Tiempo: %ld us\n", 
               getpid(), media_secuencial, tiempo_us_seq);

        FILE *fichero_seq = fopen(nombre_fichero, "a");
        fprintf(fichero_seq, "[CÁLCULO SECUENCIAL] Hijo P+2 (PID %d) | Media final: %f | Tiempo: %ld us\n", 
                getpid(), media_secuencial, tiempo_us_seq);
        fclose(fichero_seq);

        exit(0); 
    }

    // Padre espera a que hijo P+2 acabe
    wait(NULL);

    // ---- FIN ZONA HIJO P+2  ----

    // ---- ZONA DEL PADRE ----
    
    FILE *fichero_final = fopen(nombre_fichero, "r");
    if (fichero_final == NULL) {
        printf("Error al abrir el fichero para la lectura final.\n");
        return 1;
    }

    printf("\n=== RESUMEN DE TIEMPOS Y DIFERENCIA ===\n");

    int pid_temp, inicio_temp, fin_temp;
    double media_temp, media_concurrente, media_secuencial;
    long tiempo_temp;

    // Mostrar tiempo hijos P
    for (int i = 0; i < P; i++) {
        fscanf(fichero_final, "PID_HIJO: %d | Rango: %d al %d | Media: %lf | Tiempo: %ld us\n", 
               &pid_temp, &inicio_temp, &fin_temp, &media_temp, &tiempo_temp);
        printf("Tiempo del Hijo %d (PID %d): %ld us\n", i, pid_temp, tiempo_temp);
    }

    // Mostrar resultados y tiempo P+1
    fscanf(fichero_final, "\n---> [RESULTADO GLOBAL CONCURRENTE] Hijo P+1 (PID %d) | Media final: %lf | Tiempo: %ld us\n\n", 
           &pid_temp, &media_concurrente, &tiempo_temp);
    printf("Tiempo del Hijo P+1 (PID %d, Concurrente): %ld us\n", pid_temp, tiempo_temp);

    // Mostrar resultados P+2
    fscanf(fichero_final, "[CÁLCULO SECUENCIAL] Hijo P+2 (PID %d) | Media final: %lf | Tiempo: %ld us\n", 
           &pid_temp, &media_secuencial, &tiempo_temp);
    printf("Tiempo del Hijo P+2 (PID %d, Secuencial): %ld us\n", pid_temp, tiempo_temp);

    fclose(fichero_final);

    // Mostrar diferencia entre ambos
    double diferencia = fabs(media_concurrente - media_secuencial);
    printf("\nDIFERENCIA MATEMÁTICA ENTRE RESULTADOS: %f\n\n", diferencia);

    // ---- FIN ZONA PADRE ----
    return 0;
}