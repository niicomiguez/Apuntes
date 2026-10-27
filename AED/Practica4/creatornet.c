#include <stdio.h>
#include <stdlib.h>
#include "grafo.h"
#include "string.h"
#include <float.h>
#include <string.h>


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
void mostrarEstadisticas(TGRAFO G) {
    TIPOVERTICE* vertices = leerVertices(G);
    int n = leerN(G);

    if (n == 0) {
        printf("\nLa red esta vacia.\n");
        return;
    }

    int categorias[5] = {0};
    int colabPorCreador[n]; // Guardara el total de colaboraciones de cada creador
    int colabTotales = 0, colabD = 0, colabR = 0;
    float costeTotal = 0.0f, duracionTotal = 0.0f;
    int maxColab = -1;

    // 1. Contar creadores por categoría
    for (int i = 0; i < n; i++) {
        categorias[vertices[i].categoria - 1]++;
    }

    // 2. Recorrer colaboraciones y calcular métricas
    for (int i = 0; i < n; i++) {
        int colabCreador = 0; // Grado del creador actual

        for (int j = 0; j < n; j++) {
            if (existeArco(G, i, j)) {
                colabCreador++;

                // j > i para evaluar solamente la matriz triangular superior
                if (j > i) {
                    TIPOARCO arco = leerArco(G, i, j);
                    colabTotales++;

                    if (arco.modalidad == 'D') colabD++;
                    else if (arco.modalidad == 'R') colabR++;

                    costeTotal += arco.coste;
                    duracionTotal += arco.duracion;
                }
            }
        }

        colabPorCreador[i] = colabCreador; // Guardamos cuantas tiene el creador i

        // Actualizamos solo la cifra récord
        if (colabCreador > maxColab) {
            maxColab = colabCreador;
        }
    }

    // 3. Imprimir los resultados por pantalla
    printf("\n==================================================\n");
    printf("           ESTADISTICAS DE CREATORNET\n");
    printf("==================================================\n");
    printf("[+] Creadores/as registrados: %d\n", n);
    printf("    - Categoria 1 (Streamer): %d\n", categorias[0]);
    printf("    - Categoria 2 (Artista digital/musical): %d\n", categorias[1]);
    printf("    - Categoria 3 (E-sports): %d\n", categorias[2]);
    printf("    - Categoria 4 (Podcaster/Tecnologia): %d\n", categorias[3]);
    printf("    - Categoria 5 (Dev videojuegos): %d\n", categorias[4]);

    printf("\n[+] Colaboraciones totales: %d\n", colabTotales);
    printf("    - Directas [D]: %d\n", colabD);
    printf("    - Remotas  [R]: %d\n", colabR);

    if (colabTotales > 0) {
        printf("\n[+] Promedios de colaboracion:\n");
        printf("    - Coste medio: %.2f €\n", costeTotal / colabTotales);
        printf("    - Duracion media: %.2f h\n", duracionTotal / colabTotales);
    } else {
        printf("\n[+] No hay colaboraciones para calcular promedios.\n");
    }

    if (n > 0) {
        printf("\n[+] Creador/a(s) mas activo/a(s) (%d colaboraciones):\n", maxColab);
        for (int i = 0; i < n; i++) {
            if (colabPorCreador[i] == maxColab) {
                printf("    - Alias: %s\n", vertices[i].alias);
            }
        }
    }
    printf("==================================================\n");
}

void redDeCosteMinimo(TGRAFO G) {
    TIPOVERTICE *vertices = leerVertices(G);
    int N = leerN(G);

    if (N == 0) {
        printf("La red esta vacia.\n");
        return;
    }

    int verticesVisitados[N];
    for (int i = 0; i < N; i++) {
        verticesVisitados[i] = 0;
    }

    int numVisitados = 0;
    float costeTotal = 0.0f;
    int numComponentes = 0;

    // 1. Bucle principal: se repite mientras queden vértices por visitar en el grafo
    while (numVisitados < N) {
        // Buscar el primer vértice no visitado para iniciar una componente conexa
        int v = -1;
        for (int i = 0; i < N; i++) {
            if (verticesVisitados[i] == 0) {
                v = i;
                break;
            }
        }

        if (v == -1) break; // Todos los vértices han sido visitados

        verticesVisitados[v] = 1;
        numComponentes++;
        numVisitados++;

        printf("\nRed de coste minimo de Componente Conexa %d\n", numComponentes);

        // 2. Aplicar el algoritmo de Prim dentro de esta componente conexa
        int encontrado = 1;
        while (encontrado == 1) {
            encontrado = 0;
            float minimo = FLT_MAX;
            int vx = -1, vy = -1;
            TIPOARCO mejorArco;

            // Buscar el arco de menor coste que conecte un vértice visitado con uno no visitado
            for (int i = 0; i < N; i++) {
                if (verticesVisitados[i] == 1) {
                    for (int j = 0; j < N; j++) {
                        if (verticesVisitados[j] == 0 && existeArco(G, i, j)) {
                            TIPOARCO arco = leerArco(G, i, j);
                            if (!encontrado || arco.coste < minimo) {
                                minimo = arco.coste;
                                vx = i;
                                vy = j;
                                mejorArco = arco;
                                encontrado = 1;
                            }
                        }
                    }
                }
            }

            // Si encontramos un arco válido, lo añadimos al árbol de expansión
            if (encontrado) {
                verticesVisitados[vy] = 1;
                numVisitados++;
                costeTotal += minimo;

                // Formato especificado: AliasOrigen--Modalidad--AliasDestino : Coste €
                printf("%s--%c--%s : %.2f €\n",
                       vertices[vx].alias,
                       mejorArco.modalidad,
                       vertices[vy].alias,
                       minimo);
            }
        }
    }

    printf("\nCoste total del bosque: %.2f €\n", costeTotal);
}

void imprimirCaminoRec(TGRAFO G, TIPOVERTICE *vertices, int N, int P[][N], int origen, int destino) {
    if (origen != destino) {
        int previo = P[origen][destino];
        // Llamada recursiva para imprimir el tramo anterior
        imprimirCaminoRec(G, vertices, N, P, origen, previo);
        
        // Leemos el arco entre previo y destino para obtener la modalidad ('D' o 'R')
        TIPOARCO arco = leerArco(G, previo, destino);
        printf(" --%c-- %s", arco.modalidad, vertices[destino].alias);
    } else {
        // Caso base: imprime el vértice de origen
        printf("%s", vertices[origen].alias);
    }
}

void caminoMinimo(TGRAFO G, int criterio) {
    TIPOVERTICE *vertices = leerVertices(G);
    int N = leerN(G);

    if (N == 0) {
        printf("La red esta vacia.\n");
        return;
    }

    TIPOVERTICE v1, v2;
    printf("\n--- CAMINO MINIMO POR %s ---\n", (criterio == 1) ? "COSTE" : "DURACION");

    // 1. Pedir y validar alias de origen
    printf("Introduce alias de origen: ");
    scanf(" %s", v1.alias);
    if (!existeVertice(G, v1)) {
        printf("Error: El/La creador/a '%s' no existe en la red.\n", v1.alias);
        return;
    }

    // 2. Pedir y validar alias de destino
    printf("Introduce alias de destino: ");
    scanf(" %s", v2.alias);
    if (!existeVertice(G, v2)) {
        printf("Error: El/La creador/a '%s' no existe en la red.\n", v2.alias);
        return;
    }

    int origen = posicionVertice(G, v1);
    int destino = posicionVertice(G, v2);

    // 3. Declaración e inicialización de matrices D y P
    float D[N][N];
    int P[N][N];

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (i == j) {
                D[i][j] = 0.0f;
                P[i][j] = -1;
            } else if (existeArco(G, i, j)) {
                TIPOARCO arco = leerArco(G, i, j);
                // Si criterio == 1 usamos coste; si criterio == 2 usamos duracion
                D[i][j] = (criterio == 1) ? arco.coste : arco.duracion;
                P[i][j] = i;
            } else {
                D[i][j] = FLT_MAX;
                P[i][j] = -1;
            }
        }
    }

    // 4. Algoritmo de Floyd-Warshall (Cálculo de distancias mínimas)
    for (int k = 0; k < N; k++) {
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                if (D[i][k] != FLT_MAX && D[k][j] != FLT_MAX) {
                    if (D[i][j] > D[i][k] + D[k][j]) {
                        D[i][j] = D[i][k] + D[k][j];
                        P[i][j] = P[k][j];
                    }
                }
            }
        }
    }

    // 5. Impresión del resultado
    if (D[origen][destino] == FLT_MAX) {
        printf("El camino de %s minimo entre %s y %s NO EXISTE ya que no estan en la misma componente conexa.\n",
               (criterio == 1) ? "coste" : "duracion", v1.alias, v2.alias);
    } else {
        printf("\nCamino: ");
        imprimirCaminoRec(G, vertices, N, P, origen, destino);
        printf("\n");

        if (criterio == 1) {
            printf("Coste de la produccion: %.2f €\n", D[origen][destino]);
        } else {
            printf("Duracion total: %.2f h\n", D[origen][destino]);
        }
    }
}

void guardarRed(TGRAFO G, const char *nombreFichero)
{
    FILE *f = fopen(nombreFichero, "w");
    if (f == NULL) {
        printf("Error: No se pudo abrir o crear el fichero '%s' para guardar los datos.\n", nombreFichero);
        return;
    }

    int N = leerN(G);
    TIPOVERTICE *vertices = leerVertices(G);

    // 1. Escribir todos los creadores (vértices)
    // FORMATO EXACTO: C;Alias;Categoria;Colectivo;;
    for (int i = 0; i < N; i++) {
        fprintf(f, "C;%s;%d;%s;;\n", 
                vertices[i].alias, 
                vertices[i].categoria, 
                vertices[i].colectivo);
    }

    // 2. Escribir todas las colaboraciones (arcos)
    // FORMATO EXACTO: A;AliasOrigen;AliasDestino;Coste;Duracion;Modalidad
    for (int i = 0; i < N; i++) {
        for (int j = i + 1; j < N; j++) {
            if (existeArco(G, i, j)) {
                TIPOARCO arco = leerArco(G, i, j);
                fprintf(f, "A;%s;%s;%.2f;%.2f;%c\n", 
                        vertices[i].alias, 
                        vertices[j].alias, 
                        arco.coste, 
                        arco.duracion, 
                        arco.modalidad);
            }
        }
    }

    fclose(f);
    printf("\nRed guardada con exito en el fichero '%s'.\n", nombreFichero);
}

void cargarRed(TGRAFO *G, const char *nombreFichero)
{
    _cargarDatos(G, nombreFichero);
}