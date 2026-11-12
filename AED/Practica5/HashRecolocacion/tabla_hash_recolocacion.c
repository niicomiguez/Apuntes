#include "tabla_hash_recolocacion.h"

/* TABLA HASH CON RECOLOCACION */

//Inicializamos poniendo id vac�o en todas las posiciones
void InicializarTablaHash(TablaHash t) {
    for (int i = 0; i < N; i++)
        t[i].id[0] = VACIO;
}

/******* FUNCIONES HASH *******/

int FuncionHash(TIPOELEMENTO seguidor, unsigned int tipoFH) {
    int suma = 0, posicion = 0, digitos=2; // n�mero de d�gitos por trozo 
    const unsigned long constante = 2654435761u; /* valor para mezcla */
    unsigned long mezcla;
    char *endptr;
    unsigned int telf;
    
    switch (tipoFH) {
        case 1: //Funci�n hash 1 (CADENA DE CARACTERES)
            for (int i = strlen(seguidor.id) - 1; i >= 0; i--)
                suma += seguidor.id[i];
            posicion = suma % N;
            break;
        case 2:  //Funci�n hash 2 (CADENA DE CARACTERES)
            for (int i = strlen(seguidor.id) - 1; i >= 0; i--) {
                suma = (suma * 256 + seguidor.id[i]) % N;
            }
            posicion = suma;
            break;
        case 3:  //Funci�n hash 3 (ENTEROS)
                // Recorremos la clave en partes
                      
            for (int i = 0; i < strlen(seguidor.telefono); i += digitos) {
                char trozo[8] = {0};
                
                strncpy(trozo, seguidor.telefono+i, digitos);
                suma += atoi(trozo);
            }
            posicion=suma % N;
            break;
        case 4: //Funci�n hash 4 (ENTEROS)
            telf=strtol(seguidor.telefono,&endptr,10);
            mezcla = (unsigned long)telf * constante;
            posicion= mezcla % N;
            break;

    }
    return posicion;
}

/* Funcion que localiza la posicion del elemento cuando buscamos*/
////////////////////////////////////////////////////////////////////////
//A�adir el par�metro por referencia nPasosExtraB
////////////////////////////////////////////////////////////////////////
int _PosicionBuscar(TablaHash t, TIPOELEMENTO seguidor,unsigned int tipoFH, unsigned int tipoR, unsigned int a,int *nPasosExtraB) {
    /* Devuelve el sitio donde est� el seguidor que busco, o donde deberia estar. */
    /* No tiene en cuenta los borrados para avanzar.                     */
    int i,posicion;

    int ini = FuncionHash(seguidor, tipoFH);

    for (i = 0; i < N; i++) {
        switch (tipoR) {
            case 1: //lineal
                posicion = (ini + a * i) % N;
                break;
            case 2: //cuadr�tica
                posicion = (ini + i * i) % N;
                break;
        }


        if (t[posicion].id[0] == VACIO) { //si est� vac�o, dejo de buscar
            ///////////////////////////////////////////////
            //incremento en i el nPasosExtraB 
            *nPasosExtraB += i;
            ///////////////////////////////////////////////
            return posicion;
        }
        if (!strcmp(t[posicion].id, seguidor.id)) {//si lo encontr�, dejo de buscar
            ///////////////////////////////////////////////
            //incremento en i el nPasosExtraB
            *nPasosExtraB += i;
            ///////////////////////////////////////////////
            return posicion;
        }
    }
    ///////////////////////////////////////////////
    // no encontr� posici�n, actualizo nPasosExtraB al valor correspondiente
    *nPasosExtraB+=i;
    ///////////////////////////////////////////////
    return ini; //devuelvo la posici�n dada por la funci�n hash
}

/*Funcion que localiza la posicion para insertar un elemento */
////////////////////////////////////////////////////////////////////////
//A�adir como par�metros por referencia hayColisionI (0/1) y nPasosExtraI
////////////////////////////////////////////////////////////////////////
int _PosicionInsertar(TablaHash t, TIPOELEMENTO seguidor, unsigned int tipoFH, unsigned int tipoR, unsigned int a,int *hayColisionI, int *nPasosExtraI) {
    // Devuelve el sitio donde podriamos poner el seguidor seg�n la clave/funci�n hash usadas
    int i, posicion;
    
    int ini = FuncionHash(seguidor,tipoFH); //calculo la posici�n mediante la funci�n hash "tipoFH"

    for (i = 0; i < N; i++) {
        /////////////////////////////////////////////
        //cuando i no es 0, hay colisi�n
        if (i != 0) {
            *hayColisionI = 1;
        }
        /////////////////////////////////////////////

        //Intento recolocar en otra posici�n aux. Cuando i=0, aux=ini, pruebo en la posici�n dada por la funci�n hash
        switch (tipoR) {
            case 1: //recolocaci�n lineal
                posicion = (ini + a * i) % N;
                break;
            case 2: //Recolocaci�n cuadr�tica
                posicion = (ini + i * i) % N;
                break;
        }

        //Busco hueco 
        if (t[posicion].id[0] == VACIO || t[posicion].id[0] == BORRADO) {
            //Hueco encontrado, se han necesitado i intentos para ubicar el dato
            ////////////////////////////////////////////////////////////////////
            //Incrementar en i la variable nPasosExtraI
            *nPasosExtraI += i;
            
            return posicion;
        }
        //Si el elemento a insertar ya estaba en la tabla
        if (!strcmp(t[posicion].id, seguidor.id)) {
            //Ya est�, se han necesitado i intentos para encontrar el dato
            ////////////////////////////////////////////////////////////////////
            //Incrementar en i la variable nPasosExtraI
            *nPasosExtraI += i;
            ///////////////////////////////////////////////////////////////////
            return posicion;
        }
    }
    ///////////////////////////////////////////////
    // no encontr� posici�n, actualizo nPasosExtraI al valor correspondiente
    *nPasosExtraI+=i;
    ///////////////////////////////////////////////
    return ini; //devuelvo la posici�n dada por la funci�n hash
}

/* Funcion que indica si un elemento est� o no en la tabla */
/* Devuelve 1 si el elemento buscado est� en la tabla, 0 si no est� */
////////////////////////////////////////////////////////////////////////
//A�adir como par�metro por referencia nPasosExtraB
////////////////////////////////////////////////////////////////////////
int EsMiembroHash(TablaHash t, TIPOELEMENTO seguidor, int tipoclave, unsigned int tipoFH, unsigned int tipoR, unsigned int a,int *nPasosExtraB) {
 
    ///////////////////////////////////////////////
    //La funci�n _PosicionBuscar necesita nPasosExtraB
    ///////////////////////////////////////////////////////
    int posicion = _PosicionBuscar(t, seguidor, tipoFH, tipoR,a,nPasosExtraB);

    if (t[posicion].id[0] == VACIO)
        return 0;
    else {
        if (tipoclave==1) //clave id
            return (!strcmp(t[posicion].id, seguidor.id));
        else if (tipoclave==2) //clave telefono
            return (!strcmp(t[posicion].telefono, seguidor.telefono));
    }
    return 0;
}

/* BUSCA UN ELEMENTO CON LA CLAVE INDICADA EN LA TABLA HASH, Y LO DEVUELVE, 
 * ADEMAS DE INDICAR CON 1 QUE EXISTE EL ELEMENTO, Y CON 0 QUE NO ESTA EN LA TABLA */
////////////////////////////////////////////////////////////////////
//Añadir el par�metro por referencia nPasosExtraB
////////////////////////////////////////////////////////////////////
int BuscarHash(TablaHash t, TIPOELEMENTO clavebuscar, TIPOELEMENTO *e, int tipoclave, unsigned int tipoFH, unsigned int tipoR, unsigned int a,int *nPasosExtraB) {

    ///////////////////////////////////////////////
    //La funci�n _PosicionBuscar necesita nPasosExtraB
    ///////////////////////////////////////////////////////
    int posicion = _PosicionBuscar(t, clavebuscar, tipoFH, tipoR, a,nPasosExtraB);
    if (t[posicion].id[0] == VACIO)
        return 0;
    else {
        if ((tipoclave==1 && strcmp(t[posicion].id, clavebuscar.id) == 0) || (tipoclave==2 && strcmp(t[posicion].telefono,clavebuscar.telefono)==0) ) {
            *e = t[posicion];
            return 1;
        }
        else
            return 0;           
    }
}

/* Funcion que inserta un elemento en la tabla */
//////////////////////////////////////////////////////////////////////////////
//Añadir el par�metro por referencia nPasosExtraI
//Convertir la funci�n de void a int, devuelve si hay o no colisi�n (1/0)
//////////////////////////////////////////////////////////////////////////////
int InsertarHash(TablaHash t, TIPOELEMENTO e, int tipoclave, unsigned int tipoFH, unsigned int tipoR, unsigned int a,int *nPasosExtraI) {
    int posicion;
    ////////////////////////////////
    //Inicializar hayColisionI a 0, y pasarla como par�metro
    int hayColisionI = 0;
    //Enviar a _PosicionInsertar &hayColisionI y nPasosExtraI
    ////////////////////////////////
    posicion = _PosicionInsertar(t, e, tipoFH, tipoR,a,&hayColisionI,nPasosExtraI);
    /////////////////////////////////////////////////////////////

     if (t[posicion].id[0] == VACIO || t[posicion].id[0] == BORRADO) {
        t[posicion] = e;
    }
    else {
        printf("ERROR: No se encuentra espacio\n");
    }
    /////////////////////////////////
    //Devolver hayColisionI 
    return hayColisionI;
    ////////////////////////////////
}

/* Funcion que elimina un elemento de la tabla */
//////////////////////////////////////////////////////////////////////////////
//A�adir el par�metro por referencia nPasosExtraE
//////////////////////////////////////////////////////////////////////////////
void BorrarHash(TablaHash t, TIPOELEMENTO seguidor, int tipoclave, unsigned int tipoFH, unsigned int tipoR, unsigned int a,int *nPasosExtraE) {
    ///////////////////////////////////////////////
    //La funci�n _PosicionBuscar necesita nPasosExtraE
    ///////////////////////////////////////////////////////
    int posicion = _PosicionBuscar(t, seguidor, tipoFH, tipoR, a,nPasosExtraE);

    if (t[posicion].id[0] != VACIO && t[posicion].id[0] != BORRADO) {
        if ((tipoclave==1 && !strcmp(t[posicion].id, seguidor.id))||(tipoclave==2 && !strcmp(t[posicion].telefono, seguidor.telefono)) ){
            t[posicion].id[0] = BORRADO;
            t[posicion].id[1] = VACIO;
        }
          
    }
}

