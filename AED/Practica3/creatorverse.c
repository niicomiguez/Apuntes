#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "creatorverse.h"

//FUNCIONES PRIVADAS: su prototipo no esta en creatorverse.h
//Listado de los prototipos de las funciones privadas
void _imprimirCreador(TIPOELEMENTOABB creador);
void _imprimirCola(TCOLA *cola);
void _imprimirReto(TIPOELEMENTOCOLA reto, int numero);

//Elimina el cambio de linea final si uso gets() o fgets()
void _strip_line(char *linea); 

//Separa cada reto de un creador y lo inserta en la cola de creadores
void _splitByComma(char*field, TCOLA *cola);


//FUNCIONES PUBLICAS: su prototipo está en creatorverse.h

////////////////////////////////////////////////////////////////////////////////////////
// PARA COMPLETAR POR EL/LA ESTUDIANTE:
// COMPLETA TODOS LOS COMENTARIOS INDICADOS EN MAYUSCULAS EN ESTA FUNCION 
// Y EN LA FUNCION PRIVADA _splitByComma()
////////////////////////////////////////////////////////////////////////////////////////
void inicializarCreadores(TABB *A, int nparam, char **args) {
    TIPOELEMENTOABB creador;
    char linea[1000]; //cada linea del archivo tiene toda la informacion de un creador
    char *token; //variable que va leyendo cada campo del creador

    //ANHADE EL CODIGO PARA CREAR EL ARBOL X
    crearAbb(A);

    if (nparam < 3 || strcmp(args[1], "-f") != 0) {
        fprintf(stderr, "Para cargar un archivo usa: %s -f <filename>\n", args[0]);
        return;
    }
    FILE *fp = fopen(args[2], "rt");
    if (fp == NULL) {
        perror("Error abriendo el archivo\n");
        return;
    }
    while (fgets(linea, sizeof (linea), fp)) {
        _strip_line(linea); //elimino el cambio de linea final
        token = strtok(linea, "|"); //primer token
        
        int n = 0; //tengo 7 campos, contador de campos
        while (n < 7) {
            n++;
            switch (n) {
                case 1: //alias
                    // ANHADE EL CODIGO PARA COPIAR EL token EN EL CAMPO alias DEL CREADOR CON strncpyç X
                    strncpy(creador.alias,token,MAX_ALIAS);
                    break;
                case 2: //categoria
                    // ANHADE EL CODIGO PARA COPIAR EL token EN EL CAMPO categoria DEL CREADOR CON atoi X
                    creador.categoria = atoi(token);
                    break;
                case 3: //colectivo
                    // ANHADE EL CODIGO PARA COPIAR EL token EN EL CAMPO colectivo DEL CREADOR CON strncpy X
                    strncpy(creador.colectivo,token,MAX_COLEC);
                    break;
                case 4: //seguidores
                    // ANHADE EL CODIGO PARA COPIAR EL token EN EL CAMPO seguidores DEL CREADOR CON atoi X
                    creador.seguidores=atoi(token);
                    break;
                case 5: //verificado
                    // ANHADE EL CODIGO PARA COPIAR EL token EN EL CAMPO verificado DEL CREADOR CON atoi X
                    creador.verificado=atoi(token);
                    break;
                case 6: //cola de retos separados por comas
                    // ANHADE EL CODIGO PARA CREAR LA COLA DE RETOS DEL CREADOR ACTUAL X
                    crearCola(&creador.retos);
                    // ANHADE EL CODIGO SIGUIENTE: SI token ES DISTINTO DE "-", LLAMAS A LA FUNCIÓN
                    if (strcmp(token,"-"))
                    {
                        _splitByComma(token,&creador.retos);
                    }
                    // _splitByComma MODIFICANDO LA SIGUIENTE LINEA SUSTITUYENDO EL DATO ENTRE <> POR LA VARIABLE QUE SE INDICA
                         //_splitByComma(token, <cola de retos del creador>)
                    break;
                case 7: //descripcion
                    // ANHADE EL CODIGO PARA COPIAR EL token EN EL CAMPO descripcion DEL CREADOR CON strncpy
                    strncpy(creador.descripcion,token,MAX_DESC);
                    break;
            }
            token = strtok(NULL, "|");

        }
        //ANHADE EL CODIGO PARA INSERTAR EL CREADOR EN EL ARBOL SOLO SI NO ES MIEMBRO DEL ARBOL
        if (!esMiembroAbb(*A,creador))
        {
            insertarElementoAbb(A,creador);
        }
        
    }
    fclose(fp);
}

void anhadirCreador(TABB *A){
    
}

void listarCreadores(TABB A) {
    TIPOELEMENTOABB creador;

    if (!esAbbVacio(A)) {
        // Recorrer los menores (izquierda)
        listarCreadores(izqAbb(A));

        // Leer el nodo actual y llamar a la función en cadena
        leerElementoAbb(A, &creador);
        _imprimirCreador(creador);

        // Recorrer los mayores (derecha)
        listarCreadores(derAbb(A));
    }
}

void eliminarCreador(TABB *A){
    
}


///////////////////////////////////////////////////////
//FUNCIONES PRIVADAS
//////////////////////////////////////////////
//lee una cadena de retos y los almacena en la cola
//el formato de cadena es "titulo1:dificultad1,titulo2:dificultad2,titulo3:dificultad3,...
//el titulo es texto y la dificultad es un entero
void _splitByComma(char *cadena, TCOLA *cola) {
    TIPOELEMENTOCOLA reto; //va a tener los campos titulo y dificultad

     //REVISA SI MAX_RETO EXISTE EN cola.h O LE HAS DADO OTRO NOMBRE A LA LONGITUD MAXIMA DEL TITULO DEL RETO
    char cadenareto[MAX_RETO]; //es cada reto con los campos titulo y dificultad, leo su longitud de la longitud maxima del titulo en cola.h

    char *start = cadena;
    char *end = cadena;
    while (*end != '\0') {
        if (*end == ',') {
            *end = '\0';
            //cadenareto va a contener titulo:dificultad del reto leido
            
            //REVISA SI MAX_RETO EXISTE EN cola.h O LE HAS DADO OTRO NOMBRE
            strncpy(cadenareto, start, MAX_RETO); //MAX_RETO se lee de cola.h

            // podemos separar los campos de cada reto de forma facil con sscanf

            // MODIFICA LA SIGUIENTE LINEA SUSTITUYENDO LOS DATOS ENTRE <> POR LAS VARIABLES QUE SE INDICAN
            sscanf(cadenareto, " %[^:]:%d",reto.titulo, &reto.dificultad);
            
            //ANHADE EL CODIGO PARA INSERTAR EL RETO EN LA COLA
            insertarCola(cola, reto);

            start = end + 1;
        }
        end++;
    }
    //Copia el ultimo token
    
    // REVISA SI MAX_RETO EXISTE EN cola.h O LE HAS DADO OTRO NOMBRE
    strncpy(cadenareto, start, MAX_RETO);//MAX_RETO se lee de cola.h

    // MODIFICA LA SIGUIENTE LINEA SUSTITUYENDO LOS DATOS ENTRE <> POR LAS VARIABLES QUE SE INDICAN
    //sscanf(cadenareto, " %[^:]:%d", <campo titulo de la variable reto>, <campos dificultad de la variable reto>);

    
    //ANHADE EL CODIGO PARA INSERTAR EL RETO EN LA COLA
    
    
    
}
void _imprimirCreador(TIPOELEMENTOABB creador) {
    // Alias pegado al borde izquierdo
    // %s = Imprimir Strings
    // \n = salto de línea
    printf("Alias: %s\n", creador.alias);
    
    // \t para aplicar la sangría
    // %d = imprimir ints
    printf("\tCategoria: %d\n", creador.categoria);
    
    if (strcmp(creador.colectivo, "-") != 0) {
        printf("\tColectivo: %s\n", creador.colectivo);
    }
    
    // %ld = imprimir longs
    printf("\tSeguidores: %ld\n", creador.seguidores);
    
    if (creador.verificado == 1) {
        printf("\tVerificado: Si\n");
    }
    
    if (!esVaciaCola(creador.retos)) {
        printf("\tRetos:\n");
        _imprimirCola(&creador.retos);
    }
    
    printf("\tDescripcion: %s\n", creador.descripcion);
}

void _imprimirCola(TCOLA *cola) {
    // Código para crear la cola auxiliar, el bucle de desencolado/encolado y la restauración
    TCOLA colaAuxiliar;
    crearCola(&colaAuxiliar);
    int contador=1;
    TIPOELEMENTOCOLA elementoActual;
    while (!esVaciaCola(*cola))
    {
        elementoActual=primeroCola(*cola);
        _imprimirReto(elementoActual,contador);

        insertarCola(&colaAuxiliar,elementoActual);
        suprimirCola(cola);
        contador+=1;
    }
    while (!esVaciaCola(colaAuxiliar))
    {
        elementoActual=primeroCola(colaAuxiliar);
        insertarCola(cola,elementoActual);
        suprimirCola(&colaAuxiliar);
    }
    destruirCola(&colaAuxiliar);
    
}

void _imprimirReto(TIPOELEMENTOCOLA reto, int numero) {
    // Código con el printf para mostrar el número, título y dificultad del reto
    printf("\t%d. %s [dificultad: %d] \n",numero,reto.titulo,reto.dificultad);
}

//Funcion para eliminar el cambio de linea si uso gets() o fgets()
void _strip_line(char *linea) {
    linea[strcspn(linea, "\r\n")] = 0;
}
