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
void _buscarCreador(TABB *A);
int _maxDificultadArbol(TABB A);
int _maxDificultadCola(TCOLA *cola);
void _buscarRetoMasDificil(TABB A);
void _cambiarAlias(TABB *A, TIPOELEMENTOABB *creador);
void _copiarCola(TCOLA *original, TCOLA *copia);



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
    
    printf("Titulo del reto (fin para finalizar): ");
    scanf(" %[^\n]", reto.titulo);
    
    while (strcmp(reto.titulo, "fin") != 0) {
        
        printf("Dificultad (1-5): ");
        scanf("%d", &reto.dificultad);

        insertarCola(cola, reto);
        
        printf("Titulo del reto (fin para finalizar): ");
        scanf(" %[^\n]", reto.titulo);
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

// ### FUNCIÓN BUSCAR CREADOR
void buscarCreador(TABB A){   
    char aliasBuscar[MAX_ALIAS]; 
    TIPOELEMENTOABB creador;
    printf("\nIntroduce el alias del creador a buscar : ");
    scanf("%s",aliasBuscar);

    buscarNodoAbb(A, aliasBuscar, &creador);

    if (esMiembroAbb(A,creador))
    {
        _imprimirCreador(creador);        
    }else {
        printf("\nError: El creador '%s' no existe en la base de datos.\n", aliasBuscar);
    }
}

// ### FUNCIONES MAX DIFICULTAD ###
int _maxDificultadArbol(TABB A) {
    if (esAbbVacio(A)) return -1;

    int maxIzq = _maxDificultadArbol(izqAbb(A));
    
    TIPOELEMENTOABB creador;
    leerElementoAbb(A, &creador);
    int maxCentro = _maxDificultadCola(&creador.retos);
    
    int maxDer = _maxDificultadArbol(derAbb(A));

    // Comparar dificultad izquierda,derecha y centro
    int max = maxCentro;
    if (maxIzq > max) max = maxIzq;
    if (maxDer > max) max = maxDer;

    return max;
}

int _maxDificultadCola(TCOLA *cola) {
    if (esVaciaCola(*cola)) return -1; 

    TCOLA colaAuxiliar;
    crearCola(&colaAuxiliar);
    TIPOELEMENTOCOLA reto;
    int max = -1;

    while (!esVaciaCola(*cola)) {
        reto = primeroCola(*cola);
        if (strcmp(reto.titulo, "-") != 0 && reto.dificultad > max) {
            max = reto.dificultad;
        }
        insertarCola(&colaAuxiliar, reto);
        suprimirCola(cola);
    }

    while (!esVaciaCola(colaAuxiliar)) {
        insertarCola(cola, primeroCola(colaAuxiliar));
        suprimirCola(&colaAuxiliar);
    }
    destruirCola(&colaAuxiliar);

    return max;
}

// Recorre la cola y solo imprime los retos que coincidan con la dificultad máxima
void _imprimirRetosMaximos(TIPOELEMENTOABB creador, int maxDif) {
    TCOLA colaAuxiliar;
    crearCola(&colaAuxiliar);
    TIPOELEMENTOCOLA reto;
    int impresoNombre = 0; // Bandera para imprimir el nombre del creador solo 1 vez
    int contador = 1;

    while (!esVaciaCola(creador.retos)) {
        reto = primeroCola(creador.retos);
        
        if (reto.dificultad == maxDif && strcmp(reto.titulo, "-") != 0) {
            if (!impresoNombre) {
                printf("%s:\n", creador.alias);
                impresoNombre = 1;
            }
            printf("\t%d. %s [dificultad: %d]\n", contador, reto.titulo, reto.dificultad);
        }
        
        insertarCola(&colaAuxiliar, reto);
        suprimirCola(&creador.retos);
        contador++;
    }

    // Restaurar
    while (!esVaciaCola(colaAuxiliar)) {
        insertarCola(&creador.retos, primeroCola(colaAuxiliar));
        suprimirCola(&colaAuxiliar);
    }
    destruirCola(&colaAuxiliar);
}

// Recorrer arbol mirando max de cada creador
void _buscarRetosMaximosArbol(TABB A, int maxDif) {
    if (!esAbbVacio(A)) {
        _buscarRetosMaximosArbol(izqAbb(A), maxDif);
        
        TIPOELEMENTOABB creador;
        leerElementoAbb(A, &creador);
        _imprimirRetosMaximos(creador, maxDif);
        
        _buscarRetosMaximosArbol(derAbb(A), maxDif);
    }
}

void buscarRetoMasDificil(TABB A) {
    int maxDif = _maxDificultadArbol(A);

    if (maxDif == -1) {
        printf("\nNo hay retos registrados en la base de datos.\n");
        return;
    }

    printf("\nLa dificultad maxima registrada es: %d\n", maxDif);
    printf("Retos con dificultad %d:\n", maxDif);
    
    _buscarRetosMaximosArbol(A, maxDif);
}

// ### FUNCIONES MAX RETOS ###
int _longitudCola(TCOLA *cola) {
    if (esVaciaCola(*cola)) return 0;

    TCOLA colaAuxiliar;
    crearCola(&colaAuxiliar);
    TIPOELEMENTOCOLA reto;
    int contador = 0;

    while (!esVaciaCola(*cola)) {
        reto = primeroCola(*cola);
        
        // Si el título NO es el guion de vacío, sumamos 1
        if (strcmp(reto.titulo, "-") != 0) {
            contador++;
        }
        
        insertarCola(&colaAuxiliar, reto);
        suprimirCola(cola);
    }

    while (!esVaciaCola(colaAuxiliar)) {
        insertarCola(cola, primeroCola(colaAuxiliar));
        suprimirCola(&colaAuxiliar);
    }
    destruirCola(&colaAuxiliar);

    return contador;
}
int _maxRetosArbol(TABB A) {
    if (esAbbVacio(A)) return 0;

    int maxIzq = _maxRetosArbol(izqAbb(A));

    TIPOELEMENTOABB creador;
    leerElementoAbb(A, &creador);
    int maxCentro = _longitudCola(&creador.retos);

    int maxDer = _maxRetosArbol(derAbb(A));

    // Calculamos el mayor
    int max = maxCentro;
    if (maxIzq > max) max = maxIzq;
    if (maxDer > max) max = maxDer;

    return max;
}

void _imprimirCreadoresMaxRetos(TABB A, int maxRetos) {
    if (!esAbbVacio(A)) {
        _imprimirCreadoresMaxRetos(izqAbb(A), maxRetos);

        TIPOELEMENTOABB creador;
        leerElementoAbb(A, &creador);
        
        // Calculamos la longitud de su cola
        int numRetos = _longitudCola(&creador.retos);

        if (numRetos == maxRetos && maxRetos > 0) {
            printf("\t- %s\n", creador.alias);
        }

        _imprimirCreadoresMaxRetos(derAbb(A), maxRetos);
    }
}

void buscarCreadoresMasRetos(TABB A) {
    
    int maxRetos = _maxRetosArbol(A);

    if (maxRetos == 0) {
        printf("\nNo hay creadores con retos pendientes en la base de datos.\n");
        return;
    }

    printf("\nEl mayor numero de retos pendientes es: %d\n", maxRetos);
    printf("Creadores con %d retos:\n", maxRetos);

    _imprimirCreadoresMaxRetos(A, maxRetos);
}

// ### FUNCIONES MODIFICAR CREADOR ###

void modificarCreador(TABB *A) {
    char aliasBuscar[MAX_ALIAS];
    TIPOELEMENTOABB creador;
    int opcion = -1;

    printf("\nAlias del creador a modificar: ");
    scanf("%s", aliasBuscar);

    strcpy(creador.alias, aliasBuscar);

    if (esMiembroAbb(*A, creador)) {
        buscarNodoAbb(*A, aliasBuscar, &creador);

        while (opcion != 0) {
            printf("\nInformacion actual del creador:\n");
            printf("\tAlias: %s\n", creador.alias);
            printf("\tCategoria: %d\n", creador.categoria);
            printf("\tColectivo: %s\n", creador.colectivo);
            printf("\tSeguidores: %ld\n", creador.seguidores);
            printf("\tDescripcion: %s\n", creador.descripcion);

            printf("\n1. Cambiar alias\n");
            printf("2. Cambiar categoria\n");
            printf("3. Cambiar colectivo\n");
            printf("4. Cambiar numero de seguidores\n");
            printf("5. Cambiar estado de verificacion\n");
            printf("6. Cambiar descripcion\n");
            printf("0. Volver\n");

            printf("\nSeleccione una opcion: ");
            scanf("%d", &opcion);

            switch (opcion) {
                case 1: {
                    _cambiarAlias(A, &creador);
                    break;
                }
                case 2:
                    printf("Nueva categoria (1-5): ");
                    scanf("%d", &creador.categoria);
                    modificarElementoAbb(*A, creador);
                    printf("Categoria actualizada correctamente.\n");
                    break;
                case 3:
                    printf("Nuevo colectivo: ");
                    scanf("%s", &creador.colectivo);
                    modificarElementoAbb(*A, creador);
                    printf("Colectivo actualizado correctamente.\n");
                    break;
                case 4:
                    printf("Nuevos seguidores: ");
                    scanf("%ld", &creador.seguidores);
                    modificarElementoAbb(*A, creador);
                    printf("Seguidores actualizados correctamente.\n");
                    break;
                case 5:
                    printf("Nuevo verificado: ");
                    scanf("%d", &creador.verificado);
                    modificarElementoAbb(*A, creador);
                    printf("Verificado actualizado correctamente.\n");
                    break;
                case 6:
                    printf("Nueva descripción: ");
                    // Expresión regular para quitar espacios
                    scanf(" %[^\n]", creador.descripcion);
                    modificarElementoAbb(*A, creador);
                    printf("Descripción actualizada correctamente.\n");
                    break;
                case 0:
                    break;
                default:
                    printf("\nSeleccione una opción disponible\n");
                    break;
            }
        }
    } else {
        printf("\nError: El creador '%s' no existe en la base de datos.\n", aliasBuscar);
    }
}

void _cambiarAlias(TABB *A, TIPOELEMENTOABB *creador) {
    char nuevoAlias[MAX_ALIAS];
    printf("Introduce el nuevo alias: ");
    scanf("%s", nuevoAlias);

    // Creamos un nodo temporal para comprobar si el nuevo alias ya existe
    TIPOELEMENTOABB creadorNuevo;
    strcpy(creadorNuevo.alias, nuevoAlias);

    if (esMiembroAbb(*A, creadorNuevo)) {
        printf("Error: Ya existe un creador con el alias '%s'. No se puede cambiar.\n", nuevoAlias);
    } else {
        // Guardar alias viejo antes de modificar nada
        char aliasViejo[MAX_ALIAS];
        strcpy(aliasViejo, creador->alias); // -> porque creador es un puntero

        TCOLA colaSegura;
        _copiarCola(&(creador->retos), &colaSegura);

        // Eliminar nodo antiguo del árbol
        TIPOELEMENTOABB creadorViejo;
        strcpy(creadorViejo.alias, aliasViejo);
        suprimirElementoAbb(A, creadorViejo);

        // Actualizar creador
        strcpy(creador->alias, nuevoAlias);
        creador->retos = colaSegura;

        insertarElementoAbb(A, *creador); 

        printf("Alias modificado y creador recolocado correctamente.\n");
    }
}

// Función auxiliar que crea una cola nueva a partir de retos de creador 
void _copiarCola(TCOLA *original, TCOLA *copia) {
    crearCola(copia); 
    TCOLA colaAuxiliar;
    crearCola(&colaAuxiliar);
    TIPOELEMENTOCOLA reto;

    while (!esVaciaCola(*original)) {
        reto = primeroCola(*original);
        insertarCola(copia, reto);
        insertarCola(&colaAuxiliar, reto);
        suprimirCola(original);
    }

    while (!esVaciaCola(colaAuxiliar)) {
        insertarCola(original, primeroCola(colaAuxiliar));
        suprimirCola(&colaAuxiliar);
    }
    destruirCola(&colaAuxiliar);
}