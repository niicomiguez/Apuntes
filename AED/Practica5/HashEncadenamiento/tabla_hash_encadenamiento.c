#include "tabla_hash_encadenamiento.h"

/* TABLA HASH CON ENCADENAMIENTO */

/******* FUNCIONES HASH *******/
int FuncionHash(TIPOELEMENTO seguidor, unsigned int tipoFH) {
    int suma = 0, posicion = 0, digitos=2; // número de dígitos por trozo (puedes cambiarlo)
    const unsigned long constante = 2654435761u; /* fraccion dorada en 32 bits */
    unsigned long mezcla;
    unsigned int telf;
    char *endptr;
    switch (tipoFH) {
        case 1: //Función hash 1 (CADENA DE CARACTERES)
            for (int i = strlen(seguidor.id) - 1; i >= 0; i--)
                suma += seguidor.id[i];
            posicion = suma % N;
            break;
        case 2:  //Función hash 2 (CADENA DE CARACTERES)
            for (int i = strlen(seguidor.id) - 1; i >= 0; i--) {
                suma = (suma * 256 + seguidor.id[i]) % N;
            }
            posicion = suma;
            break;
        case 3:  //Función hash 3 (enteros)
                // Recorremos la clave en partes
                      
            for (int i = 0; i < strlen(seguidor.telefono); i += digitos) {
                char trozo[8] = {0};
                
                strncpy(trozo, seguidor.telefono+i, digitos);
                suma += atoi(trozo);
            }
            posicion=suma % N;
            break;
        case 4: //Función hash 4 (enteros)
            telf=strtol(seguidor.telefono,&endptr,10);
            mezcla = (unsigned long)telf * constante;
            posicion= mezcla % N;
            break;

    }
    return posicion;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
//Hay que añadir el detector de colisiones, si hay colisión, se devuelve 1, y se devuelve 0 en caso contrario.
//Convertir la función de void a int (0/1) X
/////////////////////////////////////////////////////////////////////////////////////////////////
int InsertarHash(TablaHash *t, TIPOELEMENTO elemento, unsigned int tipoFH) {
    int pos = FuncionHash(elemento, tipoFH);
    /////////////////////////////////////////////////////////////////////////////////////
    //Inicializo hayColision a cero
    int hayColision=0;
    if (!esListaVacia((*t)[pos]))
    {
        hayColision=1;
    }
    
    //Se produce colisión cuando la lista de la posición pos NO está vacía
    ///////////////////////////////////////////////////////////////////////////////////
    insertarElementoLista(&(*t)[pos], primeroLista((*t)[pos]), elemento);

    return hayColision;
}

////////////////////////////////////////////////////////////////////
//Añadir nPasosExtraB como parámetro por referencia
//////////////////////////////////////////////////////////////////
int BuscarHash(TablaHash t, TIPOELEMENTO buscado, TIPOELEMENTO *e, int tipoclave, unsigned int tipoFH,int *nPasosExtraB) {
    TPOSICION p;
    unsigned int encontrado = 0;
    TIPOELEMENTO ele;

    int pos = FuncionHash(buscado, tipoFH);

    p = primeroLista(t[pos]);
    while (p != finLista(t[pos]) && !encontrado) {
        recuperarElementoLista(t[pos], p, &ele);
        if ((tipoclave==1 && strcmp(ele.id, buscado.id) == 0)||(tipoclave==2 && strcmp(ele.telefono, buscado.telefono) == 0)) {
            encontrado = 1;
            *e=ele;
        } 
        else {
            p = siguienteLista(t[pos], p);
                /////////////////////////////////////////////////////////////////////
                //UN PASO ADICIONAL PARA BUSCAR la clave DENTRO DE LA LISTA t[pos]
                //Incrementar nPasosExtraB
                (*nPasosExtraB)++;
                /////////////////////////////////////////////////////////////////////
        }
    } 
    return encontrado;
}

/* DEVUELVE 1 SI EL ELEMENTO e ESTA EN LA TABLA Y 0 SI NO LO ESTA */
//////////////////////////////////////////
// Añadir parámetro por referencia nPasosExtraB
///////////////////////////////////////////////
int EsMiembroHash(TablaHash t, TIPOELEMENTO seguidor, int tipoclave, unsigned int tipoFH,int *nPasosExtraB) {
    TPOSICION p;
    int encontrado = 0;
    TIPOELEMENTO ele;
    int pos = FuncionHash(seguidor,tipoFH);
    p = primeroLista(t[pos]);
    while (p != finLista(t[pos]) && !encontrado) {
        recuperarElementoLista(t[pos], p, &ele);
        if ((tipoclave==1 && strcmp(ele.id, seguidor.id) == 0)||(tipoclave==2 && strcmp(ele.telefono, seguidor.telefono) == 0)) {
                encontrado = 1;
            } else {
                p = siguienteLista(t[pos], p);
                /////////////////////////////////////////////////////////////////////
                //UN PASO ADICIONAL PARA BUSCAR la clave DENTRO DE LA LISTA t[pos]
                //Incrementar nPasosExtraB
                (*nPasosExtraB)++;

                /////////////////////////////////////////////////////////////////////
            }      
    }
    return encontrado;
}

/* Elimina de la tabla el elemento con la clave indicada */
//////////////////////////////////////////
// Añadir parámetro por referencia nPasosExtraE
///////////////////////////////////////////////
void BorrarHash(TablaHash *t, TIPOELEMENTO seguidor, int tipoclave, unsigned int tipoFH,int *nPasosExtraE) {
    TPOSICION p;
    TIPOELEMENTO elemento;
    int pos = FuncionHash(seguidor, tipoFH);
    int encontrado=0;

    p = primeroLista((*t)[pos]);
    while (p != finLista((*t)[pos]) && !encontrado) {
        recuperarElementoLista((*t)[pos], p, &elemento);
        if ((tipoclave==1 && strcmp(elemento.id, seguidor.id) == 0)||(tipoclave==2 && strcmp(elemento.telefono, seguidor.telefono) == 0)) {
                encontrado = 1;
            } else {
                p = siguienteLista((*t)[pos], p);
            ////////////////////////////////////////////////////////////////////
            //UN PASO ADICIONAL PARA BUSCAR la clave DENTRO DE LA LISTA t[pos]
               //Incrementar nPasosExtraE
                (*nPasosExtraE)++;
            /////////////////////////////////////////////////////////////////////
        }        
    }
    if (p != finLista((*t)[pos]))
        suprimirElementoLista(&(*t)[pos], p); 
}

/* CREAR TABLA VACIA */
void InicializarTablaHash(TablaHash t) {
    for (int i = 0; i < N; i++)
        crearLista(&t[i]);
}

/* DESTRUIR TABLA */
void DestruirTablaHash(TablaHash t) {
    for (int i = 0; i < N; i++)
        destruirLista(&t[i]);
}
