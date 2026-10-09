#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "creatorverse.h"

//FUNCIONES PRIVADAS: su prototipo no esta en creatorverse.h
//Listado de los prototipos de las funciones privadas
void _imprimirCreador(TIPOELEMENTOABB creador);
void _imprimirCola(TCOLA *cola);
void _imprimirReto(TIPOELEMENTOCOLA reto, int numero);
void _introducirRetosCola(TCOLA *cola);
void _nuevosRetos(TABB *A);
void _completarReto(TABB *A,TCOLA *cola);
void guardarBaseDatos(TABB A, int argc, char **argv);
void _guardarPreorden(TABB A, FILE *fp);
void _guardarRetosCola(TCOLA *cola, FILE *fp);

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
                    // ANHADE EL CODIGO PARA COPIAR EL token EN EL CAMPO alias DEL CREADOR CON strncpy X
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

void anhadirCreador(TABB *A) {
    TIPOELEMENTOABB creador;
    char respuesta[100];
    TIPOELEMENTOCOLA reto;

    printf("Introduce los datos del nuevo creador:\n");
    
    // Alias
    printf("Alias: ");
    scanf("%s", creador.alias); // o fgets si quieres admitir espacios, pero ten cuidado con el buffer

    // Categoría (1 a 5)
    printf("Categoria (1-5): ");
    scanf("%d", &creador.categoria);

    // Colectivo (- si desconocido)
    printf("Colectivo (- si desconocido): ");
    scanf("%s", creador.colectivo);

    // Seguidores
    printf("Seguidores: ");
    scanf("%ld", &creador.seguidores);

    // Verificado (0/1)
    printf("Verificado (0/1): ");
    scanf("%d", &creador.verificado);

    // Retos pendientes con bucle hasta "fin"
    crearCola(&creador.retos);
    _introducirRetosCola(&creador.retos);

    // Descripción
    printf("Descripcion: ");
    scanf(" %[^\n]", creador.descripcion); 

    // Comprobar si ya existe en el árbol antes de insertar
    if (!esMiembroAbb(*A, creador)) {
        insertarElementoAbb(A, creador);
        printf("Creador anhadido correctamente a la base de datos.\n");
    } else {
        printf("Error: Ya existe un creador con ese alias.\n");
        // Si no se inserta, destruir su cola
        destruirCola(&creador.retos);
    }
}
void eliminarCreador(TABB *A){
    char aliasBuscar[MAX_ALIAS];
    TIPOELEMENTOABB creadorEncontrado;

    printf("Introduce el alias del creador a eliminar: ");
    scanf("%s", aliasBuscar);

    buscarNodoAbb(*A,aliasBuscar,&creadorEncontrado);

    // Borrar si existe en bd
    if (esMiembroAbb(*A, creadorEncontrado)) { 
        suprimirElementoAbb(A, creadorEncontrado);
        printf("El creador %s ha sido eliminado de la base de datos\n", aliasBuscar);
    } else {
        printf("El creador con alias %s no existe en la base de datos.\n", aliasBuscar);
    }
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
    sscanf(cadenareto, " %[^:]:%d", reto.titulo, &reto.dificultad);

    
    //ANHADE EL CODIGO PARA INSERTAR EL RETO EN LA COLA
    insertarCola(cola,reto);
    
}
void nuevosRetos(TABB *A) {
    char aliasBuscar[MAX_ALIAS];
    TIPOELEMENTOABB creador;

    printf("Introduce el alias del creador para añadir nuevos retos: ");
    scanf("%s", aliasBuscar);

    buscarNodoAbb(*A, aliasBuscar, &creador);
    // Buscar si creador existe en árbol

    if (esMiembroAbb(*A, creador)) {
        
        printf("Creador encontrado: %s\n", creador.alias);
        
        // Añadir retos
        _introducirRetosCola(&creador.retos);

        // Actualizar nodo en el árbol con la nueva cola modificada
        modificarElementoAbb(*A, creador);

        printf("Nuevos retos añadidos correctamente.\n");
    } else {
        printf("Error: No existe ningún creador con el alias '%s'.\n", aliasBuscar);
    }
}
void _introducirRetosCola(TCOLA *cola) {
    TIPOELEMENTOCOLA reto;
    printf("Introduce los retos (escribe 'fin' en el titulo para terminar):\n");
    
    while (1) {
        printf("Titulo del reto (fin para finalizar): ");
        scanf("%s", reto.titulo);
        
        if (strcmp(reto.titulo, "fin") == 0) {
            break;
        }
        
        printf("Dificultad (1-5): ");
        scanf("%d", &reto.dificultad);

        insertarCola(cola, reto);
    }
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
    }else{
        printf("\tNo hay retos disponibles\n");
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
    
    printf("\t\t%d. %s [dificultad: %d] \n", numero, reto.titulo, reto.dificultad);
    
}

//Funcion para eliminar el cambio de linea si uso gets() o fgets()
void _strip_line(char *linea) {
    linea[strcspn(linea, "\r\n")] = 0;
}

void completarReto(TABB *A) {
    char aliasBuscar[MAX_ALIAS];
    TIPOELEMENTOABB creador;

    printf("Alias del creador que quiere completar el reto : ");
    scanf("%s", aliasBuscar);

    buscarNodoAbb(*A, aliasBuscar, &creador);

    if (esMiembroAbb(*A, creador)) {
        

        if (!esVaciaCola(creador.retos)) {
            TIPOELEMENTOCOLA retoActual = primeroCola(creador.retos);
            printf("\nReto completado : %s [dificultad: %d]\n", retoActual.titulo, retoActual.dificultad);
            
            suprimirCola(&creador.retos);

            if (!esVaciaCola(creador.retos)) {
                TIPOELEMENTOCOLA retoSiguiente = primeroCola(creador.retos);
                printf("Siguiente reto : %s [dificultad: %d]\n", retoSiguiente.titulo, retoSiguiente.dificultad);
            } else {
                printf("Este creador no tiene mas retos\n");
            }

            modificarElementoAbb(*A, creador);
        } else {
            printf("\nEste creador no tiene retos pendientes.\n");
        }
    } else {
        printf("\nError: No existe ningun creador con el alias '%s'.\n", aliasBuscar);
    }
}

// Función auxiliar para escribir los retos en formato "titulo:dif,titulo:dif|"
void _guardarRetosCola(TCOLA *cola, FILE *fp) {
    if (esVaciaCola(*cola)) {
        fprintf(fp, "-|");
        return;
    }
    
    TCOLA colaAux;
    crearCola(&colaAux);
    TIPOELEMENTOCOLA reto;
    int primero = 1;

    while (!esVaciaCola(*cola)) {
        reto = primeroCola(*cola);
        
        // Si no es el primero, se imprime coma separadora
        if (!primero) {
            fprintf(fp, ",");
        }
        fprintf(fp, "%s:%d", reto.titulo, reto.dificultad);
        
        primero = 0;
        insertarCola(&colaAux, reto);
        suprimirCola(cola);
    }
    fprintf(fp, "|"); 

    // Restaurar cola original
    while (!esVaciaCola(colaAux)) {
        insertarCola(cola, primeroCola(colaAux));
        suprimirCola(&colaAux);
    }
    destruirCola(&colaAux);
}

// Recorrido PREORDEN para guardar los datos (Raíz, Izquierda, Derecha)
void _guardarPreorden(TABB A, FILE *fp) {
    if (!esAbbVacio(A)) {
        TIPOELEMENTOABB creador;
        leerElementoAbb(A, &creador);
        
        // Escribir la raíz (el creador actual)
        fprintf(fp, "%s|%d|%s|%ld|%d|", creador.alias, creador.categoria, creador.colectivo, creador.seguidores, creador.verificado);
        _guardarRetosCola(&creador.retos, fp);
        fprintf(fp, "%s|\n", creador.descripcion);
        
        // Recorrer subárbol izquierdo y luego derecho
        _guardarPreorden(izqAbb(A), fp);
        _guardarPreorden(derAbb(A), fp);
    }
}

// Función guardado
void guardarBaseDatos(TABB A, int argc, char **argv) {
    char nombreFichero[100];
    
    // Comprobación si se indica archivo
    if (argc >= 3 && strcmp(argv[1], "-f") == 0) {
        strcpy(nombreFichero, argv[2]);
    } else {
        printf("\nNo se detecta archivo inicial. Introduce el nombre del fichero para guardar (ej: salida.txt): ");
        scanf("%s", nombreFichero);
    }

    FILE *fp = fopen(nombreFichero, "wt"); // "wt" = Write Text
    if (fp == NULL) {
        printf("Error al abrir el archivo %s para escritura.\n", nombreFichero);
        return;
    }

    _guardarPreorden(A, fp);
    fclose(fp);
    printf("\nDatos guardados correctamente en '%s' usando recorrido preorden.\n", nombreFichero);
}