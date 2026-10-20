#include <stdio.h>
#include <stdlib.h>
#include "grafo.h"
#include "string.h"

///////////////////////////////////////////////////////////////////////////
// PROTOTIPOS DE LAS FUNCIONES PRIVADAS ///////////////////////////////////
// DEFINICIONES NECESARIAS PARA CARGAR EL ARCHIVO
#define TAM_LINEA (2 * TAM_ALIAS + TAM_COLECTIVO + 80)
static int _cargarDatos(TGRAFO *G, const char *nombre_fichero);
static const char *_comprobarArgumentos(int argc, char *argv[]);
static void _procesarCreador(TGRAFO *G);
static void _procesarColaboracion(TGRAFO *G);
void _strip_line(char *linea);
// TUS FUNCIONES PRIVADAS
    /*
     * TODO:
     * ANHADE AQUI LOS PROTOTIPOS DE LAS FUNCIONES PRIVADAS QUE CREES
     */
//////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
// IMPLEMENTACION DE LAS FUNCIONES PUBLICAS

// Funcion que inicializa el grafo cargando los datos de fichero si existe
void inicializar(TGRAFO *G, int argc, char *argv[]){
    const char *nombreFichero;
    crearGrafo(G);
    nombreFichero = _comprobarArgumentos(argc, argv);
    if (nombreFichero != NULL) {
        _cargarDatos(G, nombreFichero);
    }
}
// TUS FUNCIONES PUBLICAS
    /*
     * TODO:
     * ANHADE AQUI LA IMPLEMENTACION DE LAS FUNCIONES QUE LLAMES DESDE EL MENU
     */
//////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
// IMPLEMENTACION DE LAS FUNCIONES PRIVADAS
    /*
     * TODO:
     * COMPLETAR LAS FUNCIONES _procesarCreador() Y _procesarColaboracion()
     */

//Funcion privada que comprueba los argumentos de main
static const char *_comprobarArgumentos(int argc, char *argv[]) {
    if (argc == 3 && strcmp(argv[1], "-f") == 0) {
        return argv[2];
    }
    fprintf(stderr,"Aviso: los argumentos de ejecución no son correctos.\n");
    fprintf(stderr,"Uso: ./creatornet -f <fichero_datos>\n");
    fprintf(stderr,"CreatorNet se iniciara con una red vacia.\n");
    return NULL;
}

//Funcion privada que carga los datos del archivo
static int _cargarDatos(TGRAFO *G, const char *nombreFichero) {
    FILE *fichero;
    char linea[TAM_LINEA];
    char *token;
    
    int cargaCorrecta=0;
    fichero = fopen(nombreFichero, "r");
    if (fichero == NULL) {
        fprintf(stderr,"Aviso: no se ha podido abrir el fichero '%s'.\n",nombreFichero);
        fprintf(stderr,"CreatorNet se iniciara con una red vacia.\n");
        return cargaCorrecta;
    }
    cargaCorrecta = 1;
    while (fgets(linea, sizeof linea, fichero)) {
        _strip_line(linea);
        token=strtok(linea,";");
        if(token){
            switch(token[0]){
                case 'C':
                    _procesarCreador(G); //actua sobre token actual
                    break;
                case 'A':
                    _procesarColaboracion(G); //actua sobre token actual
                    break;
                default:
                    printf("Tipo de registro desconocido: %s\n",token);
            }
        }
    }
    if (ferror(fichero)) {
        fprintf(stderr,"Aviso: se ha producido un error durante la "
                "lectura del fichero '%s'.\n",nombreFichero);
        cargaCorrecta = 0;
    }
    fclose(fichero);
    return cargaCorrecta;
}
//Funcion privada que elimina el cambio de linea al final del string
void _strip_line(char *linea) {
    linea[strcspn(linea, "\r\n")] = 0;
}

//Funcion privada para procesar el token del creador
static void _procesarCreador(TGRAFO *G) {
    TIPOVERTICE creador;
    char *token;

    /* Alias */
    // Hecho
    token = strtok(NULL, ";");

    if (token!=NULL)
    {
        strncpy(creador.alias, token, TAM_ALIAS - 1);
        creador.alias[TAM_ALIAS - 1] = '\0';
    }

    /* Categoria */
    // Hecho

    token = strtok(NULL, ";");

     if (token!=NULL)
    {
        creador.categoria=atoi(token);
    }

    /* Colectivo */
    // Hecho
    token = strtok(NULL, ";");
    if (token != NULL) {
        strncpy(creador.colectivo, token, TAM_COLECTIVO - 1);
        creador.colectivo[TAM_COLECTIVO - 1] = '\0';
    } else {
        strcpy(creador.colectivo, " ");
    }

    /* Existencia */
    // Hecho
    if (!existeVertice(*G, creador)) {
        crearVertice(G, creador);
    }
}

static void _procesarColaboracion(TGRAFO *G) {
    TIPOVERTICE creadorOrigen;
    TIPOVERTICE creadorDestino;
    TIPOARCO colaboracion;
    char *token;
    int posicionOrigen;
    int posicionDestino;

    /* Alias del/de la creador/a de origen */
    // Hecho
    token = strtok(NULL, ";"); 
    if (token != NULL) {
        strncpy(creadorOrigen.alias, token, TAM_ALIAS - 1);
        creadorOrigen.alias[TAM_ALIAS - 1] = '\0';
    }

    /* Alias del/de la creador/a de destino */
    // Hecho
    token = strtok(NULL, ";");
    if (token != NULL) {
        strncpy(creadorDestino.alias, token, TAM_ALIAS - 1);
        creadorDestino.alias[TAM_ALIAS - 1] = '\0';
    }

    /* Coste de la colaboracion */
    // Hecho
    token = strtok(NULL, ";");
    if (token != NULL) {
        colaboracion.coste = (float)atof(token);
    }

    /* Duracion */
    // Hecho
    token = strtok(NULL, ";");
    if (token != NULL) {
        colaboracion.duracion = (float)atof(token);
    }

    /* Modalidad */
    // Hecho
    token = strtok(NULL, ";");
    if (token != NULL) {
        colaboracion.modalidad = token[0];
    }

    /* Comprobacion e insercion de la colaboracion */
    // Hecho
    if (existeVertice(*G, creadorOrigen) && existeVertice(*G, creadorDestino)) {
        posicionOrigen = posicionVertice(*G, creadorOrigen);
        posicionDestino = posicionVertice(*G, creadorDestino);

        if (!existeArco(*G, posicionOrigen, posicionDestino)) {
            crearArco(G, posicionOrigen, posicionDestino,colaboracion);
        }
    }
}
//Opción a del menú, introducir un vertice en el grafo

void anhadirVertice(TGRAFO *G) {
    TIPOVERTICE v1;
    printf("Introduce vertice: ");
    scanf("%d", &v1);
    if (existeVertice(*G, v1))
        printf("Ese vertice ya esta en el grafo\n");
    else
        crearVertice(G, v1);
}
//Opción b del menú, eliminar un vértice del grafo

void eliminarVertice(TGRAFO *G) {
    TIPOVERTICE v1;
    printf("Introduce vertice: ");
    scanf("%d", &v1);
    if (existeVertice(*G, v1))
        borrarVertice(G, v1);
    else
        printf("Ese vertice no existe en el grafo\n");
}

//Opción c del menú, crear una relación entre dos vértices

void anhadirArco(TGRAFO *G) {
    TIPOVERTICE v1, v2;
    //Insertamos una nueva relación pidiendo los datos al usuario controlando que existan los vértices
    printf("Nueva relacion vertice1-->vertice2\n");
    //Vértice origen del arco
    printf("Introduce vertice origen: ");
    scanf("%d", &v1);
    if (!existeVertice(*G, v1)) {
        printf("El vertice %d no existe en el grafo\n", v1);
        return;
    }
    //Vértice destino del arco
    printf("Introduce vertice destino: ");
    scanf("%d", &v2);
    if (!existeVertice(*G, v2)) {
        printf("El vertice %d no existe en el grafo\n", v2);
        return;
    }
    //Creación del arco
    if (!existeArco(*G, posicionVertice(*G, v1), posicionVertice(*G, v2)))
        crearArco(G, posicionVertice(*G, v1), posicionVertice(*G, v2));
}

//Opción d del menú, eliminar una relación entre dos vértices
void eliminarArco(TGRAFO *G) {
    TIPOVERTICE v1, v2;
    //Eliminamos una relación pidiendo los datos al usuario controlando que existan los vértices
    printf("Eliminar relacion vertice1-->vertice2\n");
    //Vértice origen del arco
    printf("Introduce vertice origen: ");
    scanf("%d", &v1);
    if (!existeVertice(*G, v1)) {
        printf("El vertice %d no existe en el grafo\n", v1);
        return;
    }
    //Vértice destino del arco
    printf("Introduce vertice destino: ");
    scanf("%d", &v2);
    if (!existeVertice(*G, v2)) {
        printf("El vertice %d no existe en el grafo\n", v2);
        return;
    }
    //Eliminación del arco
    if (existeArco(*G, posicionVertice(*G, v1), posicionVertice(*G, v2)))
        borrarArco(G, posicionVertice(*G, v1), posicionVertice(*G, v2));
}

//Opción i del menú, imprimir el grafo
//Función que imprime el grafo utilizando num_vertices para saber cuántos vértices tiene
//y array_vertices para recuperar el vector de vértices y recorrerlo

void imprimirGrafo(TGRAFO G) {
    TIPOVERTICE *VECTOR; //Para almacenar el vector de vértices del grafo
    int N; //número de vértices del grafo

    //Para recorrerla, simplemente vamos a recorrer la matriz de adyacencia
    N = leerN(G);
    VECTOR = leerVertices(G);

    int i, j;
    printf("El grafo actual es:\n");
    for (i = 0; i < N; i++) {
        //Imprimo el vértice
        printf("Vertice(%d): %d\n", i, VECTOR[i]);
        //Chequeo sus arcos
        for (j = 0; j < N; j++)
            if (existeArco(G, i, j))
                printf("\t%d-->%d\n", VECTOR[i], VECTOR[j]);
    }
}

