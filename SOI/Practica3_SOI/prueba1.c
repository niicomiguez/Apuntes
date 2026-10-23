// ### ZONA PUNTO 2 ###

// #include <stdio.h>
// #include <unistd.h>

// int main(){
//     int dato;
//     printf("Programa iniciado, PID = %d\n",getpid());

//     printf("Esperando por entrada en teclado :" );

//     scanf("%d",&dato);

//     // Abrir nueva terminal y poner kill -9 pid (este ultimo será el pid que se imprime)
//     // no se debe de llegar a ejecutar
//     printf("Dato introducido correctamente: %d\n",dato);

//     return 0;
// }

// ### FIN ZONA PUNTO 2 ###

// ### ZONA PUNTO 3 ###
/* Incluye las librerias necesarias*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
static void gestion(int); /*Declaracion de la funcion de gestion de señales recibidas*/
int main()
{
    /* Declara las variables necesarias*/
    pid_t padre, hijo;
    if (signal(SIGUSR1, gestion) == SIG_ERR)
        printf("Error al crear gestor 1\n");
    if (signal(SIGUSR2, gestion) == SIG_ERR)
        printf("Error al crear gestor 2\n");
    padre = getpid();
    if ((hijo = fork()) == 0)
    {                         /*Trabajo del hijo*/
        kill(padre, SIGUSR1); /*Envia señal al padre*/
        for (;;)
            ; /*Espera señal del padre indefinidamente*/
    }
    else
    { /*Trabajo del padre*/
        /* Completa el código del padre para enviar las señales SIGUSR2 y SIGTERM al hijo,
        y espera a que acabe con wait */
        sleep(2);
        kill(hijo, SIGUSR2);
        sleep(2);
        kill(hijo, SIGTERM);
        wait(NULL);
    }
    return 0;
}
static void gestion(int numero_de_senhal)
{ /* Funcion de gestion de señales*/
    switch (numero_de_senhal)
    {
    case SIGUSR1: /*Entra señal SIGUSR1*/
        printf("Señal tipo 1 recibida.Soy % d\n", getpid());
        break;
    case SIGUSR2:
        printf("Señal tipo 2 recibida.Soy % d\n", getpid());
        break;
    }
    return;
}
// ### FIN ZONA PUNTO 3 ###

// ### ZONA PUNTO 4 ###
// #include <stdio.h>
// #include <stdlib.h>
// #include <unistd.h>
// #include <signal.h>
// #include <ctype.h>

// void manejador_sigint(int numero_senhal)
// {
//     static int contador = 0;
//     contador++;

//     if (contador <= 3)
//     {
//         printf("\n Señal SIGINT atrapada y desactivada \n");
//     }
//     else
//     {
//         char respuesta;
//         printf("\n¿Deseas recuperar el funcionamiento normal de Ctrl+C? (S/s)");
//         scanf(" %c", &respuesta);

//         if (tolower(respuesta) == 's')
//         {
//             signal(SIGINT, SIG_DFL);
//             printf("\nComportamiento normal restaurado.");
//         }
//     }
// }

// int main()
// {
//     // Registramos la señal SIGINT (Ctrl+C)
//     if (signal(SIGINT, manejador_sigint) == SIG_ERR)
//     {
//         printf("Error al configurar el manejador de SIGINT\n");
//         return 1;
//     }

//     printf("Programa iniciado (PID: %d). Prueba a pulsar Ctrl+C varias veces...\n", getpid());

//     while (1)
//     {
//         // Proceso suspendido hasta señal nueva
//         pause();
//     }

//     return 0;
// }
// ### FIN ZONA PUNTO 4 ###
// ### ZONA PUNTO 5 ###
// #include <stdio.h>
// #include <stdlib.h>
// #include <unistd.h>
// #include <signal.h>
// #include <sys/wait.h>

// int main(int argc, char const *argv[])
// {
//     pid_t padre, hijo1, hijo2;

//     hijo1 = fork();
//     if (hijo1 < 0)
//     {
//         printf("Error al crear H1\n");
//         return 1;
//     }
//     else if (hijo1 == 0)
//     {
//         printf("[H1 - PID %d] Creado. Esperando señal para morir...\n", getpid());
//         while (1)
//         {
//             pause(); // Se queda bloqueado hasta que H2 lo mate
//         }
//         exit(0);
//     }

//     hijo2 = fork();
//     if (hijo2 < 0)
//     {
//         printf("Error al crear H2\n");
//         return 1;
//     }
//     else if (hijo2 == 0)
//     {
//         // H2 conoce 'hijo1' porque heredó la variable en el fork()
//         printf("[H2 - PID %d] Voy a finalizar a mi hermano H1 (PID %d)\n", getpid(), hijo1);

//         sleep(1); // Pequeña pausa para ver los mensajes ordenados

//         kill(hijo1, SIGKILL);
//         printf("[H2] Señal enviada a H1. Terminando normalmente...\n");

//         exit(0); // H2 termina de forma limpia
//     }

//     printf("[PADRE - PID %d] Esperando la finalización de H1 con waitpid...\n", getpid());
//     waitpid(hijo1, NULL, 0);
//     printf("[PADRE] H1 ha finalizado.\n");

//     waitpid(hijo2, NULL, 0);
//     printf("[PADRE] H2 ha finalizado. Padre termina normalmente.\n");
//     return 0;
// }
// ### FIN ZONA PUNTO 5 ###
// ### ZONA PUNTO 6 ###
// #include <stdio.h>
// #include <stdlib.h>
// #include <unistd.h>
// #include <signal.h>
// #include <sys/wait.h>

// void manejador(int numero_senhal)
// {
//     printf("\nSeñal SIGINT atrapada y recibida\n");
// }

// int main(int argc, char const *argv[])
// {
//     struct sigaction sa;

//     // Asignamos la función manejadora
//     sa.sa_handler = manejador;

//     // Limpiamos la máscara de señales a bloquear
//     sigemptyset(&sa.sa_mask);

//     // SA_RESETHAND: restaura automáticamente la acción a SIG_DFL tras el primer uso
//     sa.sa_flags = SA_RESETHAND;

//     // 2. Registramos la configuración para SIGINT con la llamada sigaction
//     if (sigaction(SIGINT, &sa, NULL) == -1)
//     {
//         perror("Error al configurar sigaction");
//         return 1;
//     }

//     printf("Programa iniciado (PID: %d). Pulsa Ctrl+C dos veces...\n", getpid());

//     // 3. Mantenemos el proceso suspendido esperando las señales
//     while (1)
//     {
//         pause();
//     }

//     return 0;
// }