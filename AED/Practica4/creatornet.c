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
void inicializar(TGRAFO *G, int argc, char *argv[])
{
    const char *nombreFichero;
    crearGrafo(G);
    nombreFichero = _comprobarArgumentos(argc, argv);
    if (nombreFichero != NULL)
    {
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

// Funcion privada que comprueba los argumentos de main
static const char *_comprobarArgumentos(int argc, char *argv[])
{
    if (argc == 3 && strcmp(argv[1], "-f") == 0)
    {
        return argv[2];
    }
    fprintf(stderr, "Aviso: los argumentos de ejecución no son correctos.\n");
    fprintf(stderr, "Uso: ./creatornet -f <fichero_datos>\n");
    fprintf(stderr, "CreatorNet se iniciara con una red vacia.\n");
    return NULL;
}

// Funcion privada que carga los datos del archivo
static int _cargarDatos(TGRAFO *G, const char *nombreFichero)
{
    FILE *fichero;
    char linea[TAM_LINEA];
    char *token;
    int cargaCorrecta = 0;
    fichero = fopen(nombreFichero, "r");
    if (fichero == NULL)
    {
        fprintf(stderr, "Aviso: no se ha podido abrir el fichero '%s'.\n", nombreFichero);
        fprintf(stderr, "CreatorNet se iniciara con una red vacia.\n");
        return cargaCorrecta;
    }
    cargaCorrecta = 1;
    while (fgets(linea, sizeof linea, fichero))
    {
        _strip_line(linea);
        token = strtok(linea, ";");
        if (token)
        {
            switch (token[0])
            {
            case 'C':
                _procesarCreador(G); // actua sobre token actual
                break;
            case 'A':
                _procesarColaboracion(G); // actua sobre token actual
                break;
            default:
                printf("Tipo de registro desconocido: %s\n", token);
            }
        }
    }
    if (ferror(fichero))
    {
        fprintf(stderr, "Aviso: se ha producido un error durante la "
                        "lectura del fichero '%s'.\n",
                nombreFichero);
        cargaCorrecta = 0;
    }
    fclose(fichero);
    return cargaCorrecta;
}
// Funcion privada que elimina el cambio de linea al final del string
void _strip_line(char *linea)
{
    linea[strcspn(linea, "\r\n")] = 0;
}

// Funcion privada para procesar el token del creador
static void _procesarCreador(TGRAFO *G)
{
    TIPOVERTICE creador;
    char *token;

    /* Alias */
    // Hecho
    token = strtok(NULL, ";");

    if (token != NULL)
    {
        strncpy(creador.alias, token, TAM_ALIAS - 1);
        creador.alias[TAM_ALIAS - 1] = '\0';
    }

    /* Categoria */
    // Hecho

    token = strtok(NULL, ";");

    if (token != NULL)
    {
        creador.categoria = atoi(token);
    }

    /* Colectivo */
    // Hecho
    token = strtok(NULL, ";");
    if (token != NULL)
    {
        strncpy(creador.colectivo, token, TAM_COLECTIVO - 1);
        creador.colectivo[TAM_COLECTIVO - 1] = '\0';
    }
    else
    {
        strcpy(creador.colectivo, " ");
    }

    /* Existencia */
    // Hecho
    if (!existeVertice(*G, creador))
    {
        crearVertice(G, creador);
    }
}

static void _procesarColaboracion(TGRAFO *G)

{
    TIPOVERTICE creadorOrigen;
    TIPOVERTICE creadorDestino;
    TIPOARCO colaboracion;
    char *token;
    int posicionOrigen;
    int posicionDestino;

    /* Alias del/de la creador/a de origen */
    // Hecho
    token = strtok(NULL, ";");
    if (token != NULL)
    {
        strncpy(creadorOrigen.alias, token, TAM_ALIAS - 1);
        creadorOrigen.alias[TAM_ALIAS - 1] = '\0';
    }

    /* Alias del/de la creador/a de destino */
    // Hecho
    token = strtok(NULL, ";");
    if (token != NULL)
    {
        strncpy(creadorDestino.alias, token, TAM_ALIAS - 1);
        creadorDestino.alias[TAM_ALIAS - 1] = '\0';
    }

    /* Coste de la colaboracion */
    // Hecho
    token = strtok(NULL, ";");
    if (token != NULL)
    {
        colaboracion.coste = (float)atof(token);
    }

    /* Duracion */
    // Hecho
    token = strtok(NULL, ";");
    if (token != NULL)
    {
        colaboracion.duracion = (float)atof(token);
    }

    /* Modalidad */
    // Hecho
    token = strtok(NULL, ";");
    if (token != NULL)
    {
        colaboracion.modalidad = token[0];
    }

    /* Comprobacion e insercion de la colaboracion */
    // Hecho
    if (existeVertice(*G, creadorOrigen) && existeVertice(*G, creadorDestino))
    {
        posicionOrigen = posicionVertice(*G, creadorOrigen);
        posicionDestino = posicionVertice(*G, creadorDestino);

        if (!existeArco(*G, posicionOrigen, posicionDestino))
        {
            crearArco(G, posicionOrigen, posicionDestino, colaboracion);
        }
    }
}
// Opción a del menú, introducir un vertice en el grafo

void anhadirCreador(TGRAFO *G)
{
    TIPOVERTICE creador;

    // 1. Pedir alias
    printf("Introduce alias del/de la creador/a: ");
    scanf(" %s", creador.alias);

    // 2. Validar si ya existe
    if (existeVertice(*G, creador))
    {
        printf("Error: Ya existe un/a creador/a con el alias '%s' en la red.\n", creador.alias);
        return;
    }

    // 3. Pedir categoría en bucle hasta que introduzca un valor válido (1 a 5)
    do
    {
        printf("Introduce categoria (1: Streamer, 2: Artista, 3: E-sports, 4: Podcaster/Divulgador, 5: Dev): ");
        if (scanf("%d", &creador.categoria) != 1)
        {
            while (getchar() != '\n')
                ;                  // Limpia el buffer si introducen letras
            creador.categoria = 0; // Fuerza un valor inválido para repetición
        }
        if (creador.categoria < 1 || creador.categoria > 5)
        {
            printf("Categoria no valida. Debe ser un numero entero entre 1 y 5.\n");
        }
    } while (creador.categoria < 1 || creador.categoria > 5);

    // 4. Pedir colectivo (si no introduce nada o pone '-', se gestiona)
    printf("Introduce colectivo (o '-' si no pertenece a ninguno): ");
    scanf(" %s", creador.colectivo);

    // 5. Insertar vértice en el grafo
    crearVertice(G, creador);
    printf("Creador/a '%s' anhadido/a correctamente.\n", creador.alias);
}
// Opción b del menú, eliminar un vértice del grafo

void eliminarCreador(TGRAFO *G)
{
    TIPOVERTICE creador;

    // 1. Pedir el alias del creador
    printf("Introduce alias del/de la creador/a a eliminar: ");
    scanf(" %s", creador.alias);

    // 2. Comprobar si existe y borrarlo
    if (existeVertice(*G, creador))
    {
        borrarVertice(G, creador);
        printf("Creador/a '%s' y todas sus colaboraciones han sido eliminados.\n", creador.alias);
    }
    else
    {
        printf("Error: El/La creador/a '%s' no existe en la red.\n", creador.alias);
    }
}
// Opción c del menú, crear una relación entre dos vértices

void crearColaboracion(TGRAFO *G)
{
    TIPOVERTICE v1, v2;
    TIPOARCO colaboracion;
    int pos1, pos2;

    printf("\n--- NUEVA COLABORACION ---\n");

    // 1. Pedir y validar alias de origen
    do
    {
        printf("Introduce alias del/de la creador/a origen: ");
        scanf(" %s", v1.alias);
        if (!existeVertice(*G, v1))
        {
            printf("Error: El/La creador/a '%s' no existe en la red. Intentalo de nuevo.\n", v1.alias);
        }
    } while (!existeVertice(*G, v1));

    // 2. Pedir y validar alias de destino (debe existir y ser distinto del origen)
    do
    {
        printf("Introduce alias del/de la creador/a destino: ");
        scanf(" %s", v2.alias);
        if (!existeVertice(*G, v2))
        {
            printf("Error: El/La creador/a '%s' no existe en la red. Intentalo de nuevo.\n", v2.alias);
        }
        else if (strcmp(v1.alias, v2.alias) == 0)
        {
            printf("Error: Un/a creador/a no puede colaborar consigo mismo/a. Intentalo de nuevo.\n");
        }
    } while (!existeVertice(*G, v2) || strcmp(v1.alias, v2.alias) == 0);

    pos1 = posicionVertice(*G, v1);
    pos2 = posicionVertice(*G, v2);

    // 3. Comprobar si ya existe una colaboración previa entre ambos
    if (existeArco(*G, pos1, pos2))
    {
        printf("Error: Ya existe una colaboracion entre '%s' y '%s'.\n", v1.alias, v2.alias);
        return;
    }

    // 4. Pedir coste en bucle (debe ser >= 0)
    do
    {
        printf("Introduce el coste de la colaboracion (>= 0): ");
        if (scanf("%f", &colaboracion.coste) != 1)
        {
            while (getchar() != '\n')
                ; // Limpia el buffer en caso de texto
            colaboracion.coste = -1.0f;
        }
        if (colaboracion.coste < 0.0f)
        {
            printf("Coste no valido. Debe ser un numero mayor o igual que cero.\n");
        }
    } while (colaboracion.coste < 0.0f);

    // 5. Pedir duración en bucle (debe ser > 0)
    do
    {
        printf("Introduce la duracion en horas (> 0): ");
        if (scanf("%f", &colaboracion.duracion) != 1)
        {
            while (getchar() != '\n')
                ; // Limpia el buffer en caso de texto
            colaboracion.duracion = 0.0f;
        }
        if (colaboracion.duracion <= 0.0f)
        {
            printf("Duracion no valida. Debe ser un numero estrictamente mayor que cero.\n");
        }
    } while (colaboracion.duracion <= 0.0f);

    // 6. Pedir modalidad en bucle (debe ser 'D' o 'R')
    do
    {
        printf("Introduce la modalidad (D para Directa, R para Remota): ");
        scanf(" %c", &colaboracion.modalidad);

        // Convertir a mayúscula si la meten en minúscula
        if (colaboracion.modalidad == 'd')
            colaboracion.modalidad = 'D';
        if (colaboracion.modalidad == 'r')
            colaboracion.modalidad = 'R';

        if (colaboracion.modalidad != 'D' && colaboracion.modalidad != 'R')
        {
            printf("Modalidad no valida. Debe ser 'D' o 'R'.\n");
        }
    } while (colaboracion.modalidad != 'D' && colaboracion.modalidad != 'R');

    // 7. Insertar el arco en el grafo
    crearArco(G, pos1, pos2, colaboracion);
    printf("Colaboracion creada con exito entre '%s' y '%s'.\n", v1.alias, v2.alias);
}

// Opción d del menú, eliminar una relación entre dos vértices
void eliminarColaboracion(TGRAFO *G)
{
    TIPOVERTICE v1, v2;
    int pos1, pos2;

    printf("\n--- ELIMINAR COLABORACION ---\n");

    // 1. Pedir alias del primer creador
    printf("Introduce alias del/de la primer/a creador/a: ");
    scanf(" %s", v1.alias);

    // 2. Comprobar si existe el vertice origen
    if (!existeVertice(*G, v1))
    {
        printf("Error: El/La creador/a '%s' no existe en la red.\n", v1.alias);
        return;
    }

    // 3. Pedir alias del segundo creador
    printf("Introduce alias del/de la segundo/a creador/a: ");
    scanf(" %s", v2.alias);

    // 4. Comprobar si existe el vertice destino
    if (!existeVertice(*G, v2))
    {
        printf("Error: El/La creador/a '%s' no existe en la red.\n", v2.alias);
        return;
    }

    // 5. Obtener posiciones de los vertices en el grafo
    pos1 = posicionVertice(*G, v1);
    pos2 = posicionVertice(*G, v2);

    // 6. Verificar si existe la colaboracion y eliminarla
    if (existeArco(*G, pos1, pos2))
    {
        borrarArco(G, pos1, pos2);
        printf("Colaboracion entre '%s' y '%s' eliminada correctamente.\n", v1.alias, v2.alias);
    }
    else
    {
        printf("Error: No existe ninguna colaboracion registrada entre '%s' y '%s'.\n", v1.alias, v2.alias);
    }
}

void mostrarRed(TGRAFO G)
{
    TIPOVERTICE *VECTOR;
    TIPOARCO arco;
    int N, i, j;

    N = leerN(G);
    VECTOR = leerVertices(G);

    for (i = 0; i < N; i++)
    {
        // Formato: AdaLoop (categoria 4, NightOwls)
        printf("%s (categoria %d, %s)\n", VECTOR[i].alias, VECTOR[i].categoria, VECTOR[i].colectivo);

        // Recorremos los arcos para mostrar sus colaboraciones
        for (j = 0; j < N; j++)
        {
            if (existeArco(G, i, j))
            {
                arco = leerArco(G, i, j);
                // Formato: [R] CodeComet coste: 140.00 € duracion: 8.00 h
                printf("\t[%c] %s coste: %.2f € duracion: %.2f h\n",
                       arco.modalidad, VECTOR[j].alias, arco.coste, arco.duracion);
            }
        }
    }
}

void consultarCreador(TGRAFO G)
{
    TIPOVERTICE creador;
    TIPOVERTICE *VECTOR;
    TIPOARCO arco;
    int pos, N, j;
    int tieneColaboraciones = 0;

    printf("\n--- CONSULTAR CREADOR/A ---\n");
    printf("Alias que se desea consultar: ");
    scanf(" %s", creador.alias);

    // 1. Verificar si el/la creador/a existe
    if (!existeVertice(G, creador))
    {
        printf("Error: El/La creador/a '%s' no existe en la red.\n", creador.alias);
        return;
    }

    // 2. Obtener la posición del vértice y los datos de la red
    pos = posicionVertice(G, creador);
    N = leerN(G);
    VECTOR = leerVertices(G);

    printf("Colaboraciones de %s:\n", creador.alias);

    // 3. Recorrer la fila correspondiente al creador buscando sus arcos
    for (j = 0; j < N; j++)
    {
        if (existeArco(G, pos, j))
        {
            arco = leerArco(G, pos, j);
            // Formato especificado: [R] CodeComet: coste 140.00 € duracion 8.00 h[cite: 7]
            printf("\t[%c] %s: coste %.2f € duracion %.2f h\n",
                   arco.modalidad, VECTOR[j].alias, arco.coste, arco.duracion);
            tieneColaboraciones = 1;
        }
    }

    if (!tieneColaboraciones)
    {
        printf("El/La creador/a '%s' no tiene colaboraciones asociadas actualmente.\n", creador.alias);
    }
}