#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "creatorverse.h"

//FUNCIONES PRIVADAS: su prototipo no esta en creatorverse.h
//Listado de los prototipos de las funciones privadas

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
                    crearCola(creador.retos);
                    // ANHADE EL CODIGO SIGUIENTE: SI token ES DISTINTO DE "-", LLAMAS A LA FUNCIÓN
                    if (strcmp(token,"-"))
                    {
                        _splitByComma(token,creador.retos);
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

    }
    fclose(fp);
}

void anhadirCreador(TABB *A){
    
}

void listarCreadores(TABB A){
    
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
            // sscanf(cadenareto, " %[^:]:%d", <campo titulo de la variable reto>, <campos dificultad de la variable reto>);
            
            //ANHADE EL CODIGO PARA INSERTAR EL RETO EN LA COLA


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

//Funcion para eliminar el cambio de linea si uso gets() o fgets()
void _strip_line(char *linea) {
    linea[strcspn(linea, "\r\n")] = 0;
}
