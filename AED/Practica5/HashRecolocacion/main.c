#include <stdio.h>
#include <stdlib.h>
#include "tabla_hash_recolocacion.h"

/// MODIFICACIONES EN main.c
/// Inserci�n (funci�n insercionArchivo): Variables nColisionesI, nPasosExtraI
/// B�squeda (funci�n busquedaArchivo): Variable nPasosExtraB

void insercionArchivo(FILE *fp, TablaHash t, int tipoclave, unsigned int tipoFH, unsigned int tipoR, unsigned int a, int *nColisionesI, int *nPasosExtraI);
void busquedaArchivo(FILE *fp, TablaHash t, int tipoclave, unsigned int tipoFH, unsigned int tipoR, unsigned int a, int *nPasosExtraB);

int main(int argc, char** argv) {
    //variable que almacenan el tipo de funci�n hash (1-2-3)
    unsigned int tipoFH; 
    //variable que almacena el tipo de recolocaci�n: lineal (1), o cuadr�tica (2)
    unsigned int tipoR;

    TablaHash t; //tabla hash

    //////////////////////////////////////////////////////////////////////////////////////
    //Definir las variables nColisionesI, nPasosExtraI y nPasosExtraB e inicializarlas a 0
    int tipoclave = 0;
    int nColisionesI = 0, nPasosExtraI = 0, nPasosExtraB = 0;
    //////////////////////////////////////////////////////////////////////////////////////

    //Abrimos el fichero de datos de ejemplo, con 9000 seguidores
    FILE *fp = fopen("comunidadCreatorVerse.csv", "rt");

    //Informo del tama�o de la tabla actual
    printf("Tama�o de la tabla hash: %d\n\n", N);
    
    //Pregunto al usuario que dato se usa como clave
    printf("\tDato a usar como clave:"
            "\n\t\t1: identificador de usuario"
            "\n\t\t2: n�mero de tel�fono");
            
    printf("\n\tTipo clave: ");
    scanf("%d", &tipoclave);

    //Pregunto al usuario que funci�n hash quiere utilizar
    printf("\tFunci�n hash a utilizar:"
            "\n\t\t1: m�todo divisi�n (suma ascii m�dulo N)"
            "\n\t\t2: suma ponderada con K=256"
            "\n\t\t3: funci�n de plegamiento (enteros)"
            "\n\t\t4: funci�n de mezcla (enteros) ");
    printf("\n\tTipo funci�n hash: ");
    scanf("%d", &tipoFH);

    printf("\nFUNCI�N HASH %u", tipoFH);

    //Pregunto al usuario que estrategia de recolocaci�n quiere utilizar
    //Si es lineal, tambi�n pido el valor de a
    printf("\tEstrategia de recolocaci�n (1: Lineal, 2: Cuadr�tica): ");
    scanf("%d", &tipoR);
    unsigned int a = 0; //inicializo a
    if (tipoR == 1) {
        printf("\tValor de a (1 para recolocaci�n simple): ");
        scanf("%u", &a);
        printf("RECOLOCACI�N LINEAL con a=%u", a);
    } else
        printf("RECOLOCACI�N CUADR�TICA");

    //Inicializo la tabla hash 
    InicializarTablaHash(t);

    printf("\n\t----INSERCI�N RECOLOCACI�N----");
    //llamo a la funci�n de inserci�n en t a partir del archivo
    //////////////////////////////////////////////////////////////////
    //A�adir los par�metros por referencia nColisionesI y nPasosExtraI
    //////////////////////////////////////////////////////////////////
    insercionArchivo(fp, t, tipoclave, tipoFH, tipoR, a, &nColisionesI, &nPasosExtraI);
    //////////////////////////////////////////////////////////////////
    //Imprimir nColisionesI y nPasosExtraI
    printf("\n\tNumero de colisiones = %d\n", nColisionesI);
    printf("\tPasos extra inserción = pasos extra búsqueda = %d\n", nPasosExtraI);
    
    //////////////////////////////////////////////////////////////////

    rewind(fp); //rebobino

     printf("\n\n\t----B�SQUEDA RECOLOCACI�N----");
    //Llamo a la funci�n de b�squeda en t a partir del archivo 
    //////////////////////////////////////////////////////////////////
    //A�adir el par�metros por referencia nPasosExtraB
    //////////////////////////////////////////////////////////////////
    busquedaArchivo(fp, t, tipoclave, tipoFH, tipoR, a, &nPasosExtraB);
    //////////////////////////////////////////////////////////////////
    //Imprimir nPasosExtraB
    printf("\n\t Pasos extra en búsqueda: %d\n", nPasosExtraB);

    //////////////////////////////////////////////////////////////////

    fclose(fp); //Cierro el archivo

    return (EXIT_SUCCESS);
}

//Funci�n para insertar usuarios en tabla hash, usando la funci�n hash "tipo"
//////////////////////////////////////////////////////////////////
//A�adir los par�metros por referencia nColisionesI y nPasosExtraI
//////////////////////////////////////////////////////////////////

void insercionArchivo(FILE *fp, TablaHash t, int tipoclave, unsigned int tipoFH, unsigned int tipoR, unsigned int a, int *nColisionesI, int *nPasosExtraI) {
    TIPOELEMENTO seguidor;
    int i=0;
    if (fp) {
        printf("\n\nInsertando datos...\n");
        for (i=0;i<9000;i++){
            fscanf(fp, "%[^,], %[^,],%s", seguidor.nombre,seguidor.id, seguidor.telefono);
    //////////////////////////////////////////////////////////////////////////////////////////
            //Modificar la funci�n InsertarHash para que devuelve: 1 si colisi�n, 0 en caso contrario
            //y acumular estos valores en nColisionesI
            //A�adir a InsertarHash par�metro nPasosExtraI por referencia
            /////////////////////////////////////////////////////////////////////////////////////////
            *nColisionesI += InsertarHash(t, seguidor, tipoclave, tipoFH, tipoR, a, nPasosExtraI);
        }
    } else {
        printf("El archivo no ha podido abrirse\n");
    }
}

//Funci�n para buscar un elemento le��do del archivo en la tabla t, usando la funci�n hash "tipoFH"
//////////////////////////////////////////////////////////////////
//A�adir el par�metro por referencia nPasosExtraB
//////////////////////////////////////////////////////////////////

void busquedaArchivo(FILE *fp, TablaHash t, int tipoclave, unsigned int tipoFH, unsigned int tipoR, unsigned int a, int *nPasosExtraB) {
    TIPOELEMENTO seguidor;
    int i;
    if (fp) {              
        for(i=0;i<9000;i++) {
            //El n�mero de colisiones es el mismo que en inserci�n, hay pasos adicionales al buscar en la lista
            ///////////////////////////////////////////////////////////////////////////////
            fscanf(fp, "%[^,], %[^,],%s", seguidor.nombre,seguidor.id, seguidor.telefono);
            //Modificar la funci�n BuscarHash() para que reciba nPasosExtraB por referencia 
            ///////////////////////////////////////////////////////////////////////////////
            BuscarHash(t, seguidor, &seguidor, tipoclave, tipoFH, tipoR, a, nPasosExtraB);
        }
    } else {
        printf("El archivo no ha podido abrirse\n");
    }
}