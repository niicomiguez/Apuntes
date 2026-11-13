#include <stdio.h>
#include <stdlib.h>
#include "tabla_hash_encadenamiento.h"

//Hay que a�adir como par�metro la variable nColisionesI: n�mero de colisiones en inserci�n
void insercionArchivo(FILE *fp, TablaHash *t, int tipoclave, unsigned int tipoFH,int *nColisionesI);
//Hay que a�adir como par�metro la variable nPasosExtraB: n�mero de pasos extra en b�squeda
void busquedaArchivo(FILE *fp, TablaHash t, int tipoclave, unsigned int tipoFH,int *nPasosExtraB);

int main(int argc, char** argv) {
    int tipoclave=0;
    //variable que almacenan el tipo de funci�n hash (1-2-3)
    unsigned int tipoFH; 

    TablaHash t; //tabla hash
    
    //////////////////////////////////////////////////////////////////////
    //Definir e inicializar a 0 las variables nColisionesI y nPasosExtraB
    //////////////////////////////////////////////////////////////////////

    int nColisionesI=0 , nPasosExtraB=0;
    //Abrimos el fichero de datos de ejemplo, con 9000 seguidores
    FILE *fp = fopen("comunidadCreatorVerse.csv", "rt");

     //Informo del tama�o de la tabla actual
    printf("Tama�o de la tabla hash: %d\n\n", N);
    
    //Pregunto al usuario el dato a usar como clave
    printf("\tDato a usar como clave:"
            "\n\t\t1: identificador de usuario"
            "\n\t\t2: n�mero de tel�fono");
            
    printf("\n\tTipo clave: ");
    scanf("%d", &tipoclave);

    //Pregunto al usuario la funci�n hash a utilizar
    printf("\tFunci�n hash a utilizar:"
            "\n\t\t1: m�todo divisi�n (suma ascii m�dulo N)"
            "\n\t\t2: suma ponderada con K=256"
            "\n\t\t3: funci�n de plegamiento (enteros)"
            "\n\t\t4: funci�n de mezcla (enteros) ");
    printf("\n\tTipo funci�n hash: ");
    scanf("%d", &tipoFH);

    printf("\nFUNCI�N HASH %u", tipoFH);
    
    //Inicializo la tabla hash con encadenamiento
    InicializarTablaHash(t);

    printf("\n\t----INSERCI�N ENCADENAMIENTO----\n");
    //llamo a la funci�n de inserci�n en t a partir del archivo y el tipo de funci�n hash a utilizar
    //////////////////////////////////////////////////////
    //A�adir como par�metro por referencia nColisionesI
    //////////////////////////////////////////////////////
    insercionArchivo(fp, &t, tipoclave, tipoFH, &nColisionesI);
    //////////////////////////////////////////////////////
    //Imprimir nColisionesI
    printf("\t Número de colisiones = %d\n", nColisionesI);

    //////////////////////////////////////////////////////

    rewind(fp); //rebobino

    printf("\n\n\t----B�SQUEDA ENCADENAMIENTO----\n");
    //Llamo a la funci�n de b�squeda en t a partir del archivo y el tipo de funci�n hash a utilizar
    //////////////////////////////////////////////////////
    //A�adir como par�metro por referencia nPasosExtraB
    //////////////////////////////////////////////////////
    busquedaArchivo(fp, t, tipoclave, tipoFH, &nPasosExtraB);
    //////////////////////////////////////////////////////
    //Imprimir nPasosExtraB
    printf("\t Pasos extra búsqueda =  %d\n", nPasosExtraB);

    //////////////////////////////////////////////////////
    
    DestruirTablaHash(t); //Destruyo la tabla
    fclose(fp); //Cierro el archivo    

    return (EXIT_SUCCESS);
}

//Funci�n para insertar usuarios en tabla hash, usando la funci�n hash "tipoFH"
///////////////////////////////////////
//A�adir el par�metro por referencia nColisionesI
///////////////////////////////////////
void insercionArchivo(FILE *fp, TablaHash *t, int tipoclave, unsigned int tipoFH,int *nColisionesI) {
    TIPOELEMENTO seguidor;
    int i=0;
    if (fp) {
        printf("\n\nInsertando datos...\n");
        for (i=0;i<9000;i++){
            fscanf(fp, "%[^,], %[^,],%s", seguidor.nombre,seguidor.id, seguidor.telefono);    
    //////////////////////////////////////////////////////////////////////////////////////////
            //Modificar la funci�n InsertarHash para que devuelva: 1 si colisi�n, 0 en caso contrario
            //y acumular estos valores en nColisionesI
            /////////////////////////////////////////////////////////////////////////////////////////
            *nColisionesI += InsertarHash(t, seguidor, tipoFH);      
        }
    } else {
        printf("El archivo no ha podido abrirse\n");
    }
}
   

//Funci�n para buscar un elemento le��do del archivo en la tabla t, usando la funci�n hash "tipoFH"
//////////////////////////////////////////////////////////////////
//A�adir el par�metro por referencia nPasosExtraB
//////////////////////////////////////////////////////////////////
void busquedaArchivo(FILE *fp, TablaHash t, int tipoclave, unsigned int tipoFH, int *nPasosExtraB) {
    TIPOELEMENTO seguidor;
    if (fp) {
        for (int i=0;i<9000;i++){
            fscanf(fp, "%[^,], %[^,],%s", seguidor.nombre,seguidor.id, seguidor.telefono);  
            //////////////////////////////////////////////////////////////////////////////////////////
            //A�a dir a BuscarHash() el par�metro por referencia nPasosExtraB
            BuscarHash(t, seguidor, &seguidor, tipoclave, tipoFH,nPasosExtraB);           
        }
    } else {
        printf("El archivo no ha podido abrirse\n");
    }
}