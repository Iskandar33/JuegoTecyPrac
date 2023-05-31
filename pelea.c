#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define SIZE 200
#define ENEMIGOSIZE 30
#define NOMBRESIZE 40
#define LORSIZE 200
#define CARTASIZE 15
#define CATEGORIASIZE 10
#define EFECTOSIZE 100
const char* s = "-";


typedef struct Jugador{
    int vida;
    int armadura;
    int oro;
    int deck[CARTASIZE];
    int enemigos[ENEMIGOSIZE];
    int opcionPsj;
    int posActual;
}Jugador;


typedef struct Personaje{
    char nombre[NOMBRESIZE];
    int vidaTotal;
    int fuerza;
    int dureza;
    int inteligencia;
    int oro;
}Personaje;


typedef struct Enemigo{
    char nombre[NOMBRESIZE];
    int vida;
    int ataque;
    char lore[LORSIZE];
}Enemigo;


typedef struct Carta{
    char nombre[NOMBRESIZE];
    int categoria;
    int num;
    int costoEnergia;
    char efecto[EFECTOSIZE];
}Carta;


typedef struct Nodo{
    int numCarta;
    int cantCarta;
    struct Nodo *sgte;
}Nodo;


typedef struct Lista{
    int tamaño;
    struct Nodo *cabeza;
}Lista;


/// Globales ///

Jugador jugador;

Enemigo arrayEnemigos[ENEMIGOSIZE];

Carta arrayCartas[CARTASIZE];

Personaje arregloPersonajes[3];

///                   ///


/// Prototipos ///


void leerTxtEnemigos();

void leerTxtCartas();

void leerTxtPersonajes();

Nodo* crearNodo(int numCarta);

Lista* crearLista();

void añadirCarta(Lista* listaCartas, int pCarta);

void eliminarMiniDeck(Lista* miniDeck);

void eliminarNodoMiniDeck(Lista* miniDeck, Nodo* nodoAux);

void crearMiniDeck(Lista* cartasMiniDeck);

void imprimirMiniDeck(Lista* miniDeck);

int utilizarCarta(Lista* miniDeck, int posicion, int hpEnemigo);

int obtenerEnergiaCarta(Lista* miniDeck, int posicion);

int menuLuchar(Lista* miniDeck, int energia);

void luchar(int tipoLucha);

void encontrarCarta();

void crearPersonajeJugador();

void menu();



///                                   ///

// int main(){
//     leerTxtEnemigos();
//     leerTxtCartas();
//     leerTxtPersonajes();
//     crearPersonajeJugador();
//     menu();
//     return 0;
// }


/// Leer txt ///


void leerTxtEnemigos(){
    FILE* file; 
    file = fopen("monstruos.txt", "r");
    char buffer[SIZE];
    char* token;
    int i = 0, vida, ataque;
    while(fgets(buffer, SIZE, file) != NULL){
        token = strtok(buffer, s);
        strcpy(arrayEnemigos[i].nombre, token);
        token = strtok(NULL, s);
        vida = atoi(token);
        arrayEnemigos[i].vida = vida;
        token = strtok(NULL, s);
        ataque = atoi(token);
        arrayEnemigos[i].ataque = ataque;
        token = strtok(NULL, s);
        strcpy(arrayEnemigos[i].lore, token);
        i++;
    }
    fclose(file);
}


void leerTxtCartas(){
    FILE* file;
    file = fopen("cartas.txt", "r");
    char buffer[SIZE];
    char* token;
    int j = 0, num, costoEnergia, categoria;
    while(fgets(buffer, SIZE, file) != NULL){
        token = strtok(buffer, s);
        strcpy(arrayCartas[j].nombre, token);
        token = strtok(NULL, s);
        categoria = atoi(token);
        arrayCartas[j].categoria = categoria;
        token = strtok(NULL, s);
        num = atoi(token);
        arrayCartas[j].num = num;
        token = strtok(NULL, s);
        costoEnergia = atoi(token);
        arrayCartas[j].costoEnergia = costoEnergia;
        token = strtok(NULL, s);
        strcpy(arrayCartas[j].efecto, token);
        j++;
    }
    fclose(file);
}


void leerTxtPersonajes(){
    FILE* file;
    file = fopen("personajes.txt", "r");
    char buffer[SIZE];
    char* token;
    int j = 0, vida, fuerza, dureza, inteligencia, oro;
    while(fgets(buffer, SIZE, file) != NULL){
        token = strtok(buffer, s);
        strcpy(arregloPersonajes[j].nombre, token);
        token = strtok(NULL, s);
        vida = atoi(token);
        arregloPersonajes[j].vidaTotal = vida;
        token = strtok(NULL, s);
        oro = atoi(token);
        arregloPersonajes[j].oro = oro;
        token = strtok(NULL, s);
        fuerza = atoi(token);
        arregloPersonajes[j].fuerza = fuerza;
        token = strtok(NULL, s);
        dureza = atoi(token);
        arregloPersonajes[j].dureza = dureza;
        token = strtok(NULL, s);
        inteligencia = atoi(token);
        arregloPersonajes[j].inteligencia = inteligencia;
        j++;
    }
    fclose(file);
}


///                                   ///


Nodo* crearNodo(int numCarta){
    int tamaño = sizeof(Nodo);
    Nodo* nodo = (Nodo*)malloc(tamaño);
    nodo->numCarta = numCarta;
    nodo->cantCarta = 1;
    nodo->sgte = NULL;
    return nodo;
}


Lista* crearLista(){
    Lista* lista = (Lista*)malloc(sizeof(Lista));
    lista->tamaño = 0;            
    lista->cabeza = NULL;
    return lista;
}


void añadirCarta(Lista* listaCartas, int pCarta){
    Nodo* nodoAux = listaCartas->cabeza; ///Se crea un nodo auxiliar que se inicia en la cabeza de la lista para poder recorrer la lista si es necesario
    Nodo* nuevaCarta;
    int flag = 0;
    nuevaCarta = crearNodo(pCarta);
    if(listaCartas->cabeza == NULL){  ///Si la lista esta vacía el nodo creado pasa a ser la cabeza de la lista
        listaCartas->cabeza = nuevaCarta;
        listaCartas->tamaño += 1;
        // printf("Cantidad: %d\n", listaCartas->cabeza->cantCarta);
    }
    else{
        while((nodoAux->sgte != NULL) && (flag != 1)){
            if(pCarta == nodoAux->numCarta){
                flag = 1;
            }
            else{
                nodoAux = nodoAux->sgte;
            }
        }
        if((flag == 1) || (nodoAux->numCarta == pCarta)){  ///Si el número de la carta en el arreglo ya esta en un nodo(significa que ya esta en la lista) se le suma 1 a la cantidad
            nodoAux->cantCarta++;
            // printf("Cantidad: %d\n", nodoAux->cantCarta);
        }
        else{
            nodoAux->sgte = nuevaCarta;  ///Si hay al menos 1 nodo, se adiciona el nodo a la última posición de la lista
            listaCartas->tamaño += 1;
            // printf("Cantidad: %d\n", nuevaCarta->cantCarta);
        }
    }
    // printf("carta añadida\n");
}


void eliminarNodoMiniDeck(Lista* miniDeck, Nodo* nodoAux){
    Nodo* nodoNuevo = miniDeck->cabeza;
    if(nodoAux->cantCarta > 1){
        nodoAux->cantCarta--;
        // printf("La carta ha sido eliminada\n");
    }
    else{
        if(miniDeck->cabeza == nodoAux){
            miniDeck->cabeza = nodoAux->sgte;
            // printf("La carta ha sido eliminada\n");        
        }
        else{
            while(nodoNuevo->sgte != nodoAux){
                nodoNuevo = nodoNuevo->sgte;
            }
            nodoNuevo->sgte = nodoNuevo->sgte->sgte;
            // printf("La carta ha sido eliminada\n");
        }
    }
}


void crearMiniDeck(Lista* cartasMiniDeck){
    int num;
    for(int i = 0; i < 5; i++){
        num = rand() % CARTASIZE;
        while(jugador.deck[num] == 0){
            num = rand() % CARTASIZE;
        }
        añadirCarta(cartasMiniDeck, num);
        // printf("Carta agregada al minideck\n");
    }
    // printf("chaolin\n");
}


void imprimirMiniDeck(Lista* miniDeck){
    Nodo* nodoAux = miniDeck->cabeza;
    int i = 0;
    while(nodoAux != NULL){
        printf("\nCarta: \n Posición: %d\n Nombre: %s\n Tipo: %d\n Energía: %d\n Efecto: %s Cantidad: %d\n", 
            i, arrayCartas[nodoAux->numCarta].nombre, arrayCartas[nodoAux->numCarta].categoria, 
                arrayCartas[nodoAux->numCarta].costoEnergia, arrayCartas[nodoAux->numCarta].efecto, nodoAux->cantCarta);
        i++;
        nodoAux = nodoAux->sgte;
    }
    // printf("XD\n");
}


int utilizarCarta(Lista* miniDeck, int posicion, int hpEnemigo){
    Nodo* nodoAux = miniDeck->cabeza;
    int i = 0, dañoTotal, armaduraTotal;
    while(i < posicion){
        nodoAux = nodoAux->sgte;
        // printf("qqqq\n");
        i++;
    }
    // printf("Adios ciclo\n");
    if(arrayCartas[nodoAux->numCarta].categoria == 2){
        printf("Carta de tipo ataque!\n");
        if(arregloPersonajes[jugador.opcionPsj].fuerza > 1){
            dañoTotal = 1.25 * (arrayCartas[nodoAux->numCarta].num);
            hpEnemigo -= dañoTotal;
            printf("Al tener una fuerza mayor, has infligido un mayor daño al enemigo\n");
            printf("Has utilizado la carta %s infligiendole %d de daño al enemigo\n", 
                arrayCartas[nodoAux->numCarta].nombre, dañoTotal);
        }
        else{
            dañoTotal = arrayCartas[nodoAux->numCarta].num;
            hpEnemigo -= dañoTotal;
            printf("Has utilizado la carta %s infligiendole %d de daño al enemigo\n", 
                arrayCartas[nodoAux->numCarta].nombre, dañoTotal);
        }
    }
    else if(arrayCartas[nodoAux->numCarta].categoria == 1){
        printf("Carta de tipo habilidad!\n");
        if(arregloPersonajes[jugador.opcionPsj].inteligencia > 1){
            dañoTotal = 1.25 * (arrayCartas[nodoAux->numCarta].num);
            hpEnemigo -= dañoTotal;
            printf("Al tener una inteligencia mayor, has infligido un mayor daño al enemigo\n");
            printf("Has utilizado la carta %s infligiendole %d de daño al enemigo\n", 
                arrayCartas[nodoAux->numCarta].nombre, dañoTotal);
        }
        else{
            dañoTotal = arrayCartas[nodoAux->numCarta].num;
            hpEnemigo -= dañoTotal;
            printf("Has utilizado la carta %s infligiendole %d de daño al enemigo\n", 
                arrayCartas[nodoAux->numCarta].nombre, dañoTotal);
        }
    }
    else{
        printf("Carta de tipo defensa!\n");
        if(arregloPersonajes[jugador.opcionPsj].dureza > 1){
            armaduraTotal = 1.25 * (arrayCartas[nodoAux->numCarta].num);
            jugador.armadura += armaduraTotal;
            printf("Al tener una dureza mayor, has obtenido más armadura\n");
            printf("Has utilizado la carta %s aumentando tu armadura durante este turno en %d\n", 
                arrayCartas[nodoAux->numCarta].nombre, armaduraTotal);
        }
        else{
            armaduraTotal = arrayCartas[nodoAux->numCarta].num;
            jugador.armadura += armaduraTotal;
            printf("Has utilizado la carta %s aumentando tu armadura durante este turno en %d\n", 
                arrayCartas[nodoAux->numCarta].nombre, armaduraTotal);
        }
    }
    eliminarNodoMiniDeck(miniDeck, nodoAux);
    // printf("Chaolin pinwin\n");
    return hpEnemigo;
}


int obtenerEnergiaCarta(Lista* miniDeck, int posicion){
    Nodo* nodoAux = miniDeck->cabeza;
    int i = 0;
    while(i != posicion){
        nodoAux = nodoAux->sgte;
        // printf("xdxdx\n");
        i++;
    }
    return arrayCartas[nodoAux->numCarta].costoEnergia;
}


int menuLuchar(Lista* miniDeck, int energia){
    int opcion, i = 0;
    Nodo* nodoAux = miniDeck->cabeza;
    printf("\n\n Para utilizar la carta que quiere ingrese la posición\n");
    printf("\n Para terminar el turno ingresa -1\n");
    printf("\n\n Ingrese una opción: ");
    scanf("%d", &opcion);
    if((opcion < 6) && (opcion > -1)){
        while(i != opcion){
            nodoAux = nodoAux->sgte;
            // printf("xdxdx\n");
            i++;
        }
        while(arrayCartas[nodoAux->numCarta].costoEnergia > energia){ 
            printf("No puede seleccionar esta opción\nVuelva a escoger: ");
            scanf("%d", &opcion);
            while(i != opcion){
                nodoAux = nodoAux->sgte;
                i++;
            }
        }
    }
    // printf("Chaum\n");
    return opcion;
}


void luchar(int tipoLucha){
    int numeroEnemigo, hpEn, atEn, dañoEnemigo, cancelarTurno, accionEnemigo, accion, buffAtaque, gastoEnergia, energia = 5;
    Lista* miniDeck = crearLista();
    // Nodo* cartaEnergia;
    srand(time(NULL));
    if(tipoLucha == 1){ ///
        numeroEnemigo = rand() % 15;
    }
    else if(tipoLucha == 2){
        numeroEnemigo = rand() % 10 + 15;
    }
    else{
        numeroEnemigo = rand() % 5 + 25;
    }
    
    hpEn = arrayEnemigos[numeroEnemigo].vida;
    atEn = arrayEnemigos[numeroEnemigo].ataque;

    while((jugador.vida > 0) && (hpEn > 0)){ 
        printf("\n %s: Vida: %d/%d ", arrayEnemigos[numeroEnemigo].nombre, hpEn, arrayEnemigos[numeroEnemigo].vida);
        printf("\n %s: Vida: %d/%d Armadura: %d\n", arregloPersonajes[jugador.opcionPsj].nombre, 
            jugador.vida, arregloPersonajes[jugador.opcionPsj].vidaTotal, jugador.armadura);
        while((energia > 0) && (jugador.vida > 0) && (hpEn > 0)){
            accionEnemigo = rand() % 2;
            if(accionEnemigo == 1){
                printf("El enemigo piensa atacar e infligir %d de daño...\n", atEn);
            }
            else{
                printf("El enemigo piensa utilizar una habilidad...\n");
            }
            crearMiniDeck(miniDeck);
            while((energia > 0) && (cancelarTurno != 1) && (jugador.vida > 0) && (hpEn > 0)){
                imprimirMiniDeck(miniDeck);
                printf("Energía: %d", energia);
                accion = menuLuchar(miniDeck, energia);
                if(accion == -1){
                    cancelarTurno = 1;
                    printf("Turno terminado...\n");
                }
                else{
                    gastoEnergia = obtenerEnergiaCarta(miniDeck, accion);
                    hpEn = utilizarCarta(miniDeck, accion, hpEn);
                }
                energia -= gastoEnergia;
                printf("\n %s: Vida: %d/%d", arrayEnemigos[numeroEnemigo].nombre, hpEn, arrayEnemigos[numeroEnemigo].vida);
                printf("\n %s: Vida: %d/%d Armadura: %d\n", arregloPersonajes[jugador.opcionPsj].nombre, 
                    jugador.vida, arregloPersonajes[jugador.opcionPsj].vidaTotal, jugador.armadura);
            }
            if(hpEn > 0){
                if(accionEnemigo == 1){
                    if(jugador.armadura > 0){
                        if(jugador.armadura > atEn){
                            jugador.armadura -= atEn;
                            printf("Has bloqueado totalmente el ataque enemigo\n");
                            printf("Después del ataque enemigo, te quedó %d de armadura\n", jugador.armadura);
                        }
                        else{
                            printf("Has bloqueado %d de daño\n", jugador.armadura);
                            dañoEnemigo = atEn - jugador.armadura;
                            jugador.armadura = 0;
                            jugador.vida -= dañoEnemigo;
                            printf("El enemigo, %s te ha infligido %d de daño\n", 
                                arrayEnemigos[numeroEnemigo].nombre, dañoEnemigo);
                        }
                    }
                    else{
                        jugador.vida -= atEn;
                        printf("El enemigo, %s te ha infligido %d de daño\n", 
                            arrayEnemigos[numeroEnemigo].nombre, atEn);
                    }
                }
                else{
                    buffAtaque = rand() % 5 + 1;
                    atEn += buffAtaque;
                    printf("El enemigo ha mejorado su ataque en %d\n", buffAtaque);
                }
            }
            energia = 5;
            jugador.armadura = 0;
            cancelarTurno = 0;
            miniDeck->cabeza = NULL;
            printf(" %s: Vida: %d/%d", arrayEnemigos[numeroEnemigo].nombre, hpEn, arrayEnemigos[numeroEnemigo].vida);
            printf("\n %s: Vida: %d/%d Armadura: %d\n", arregloPersonajes[jugador.opcionPsj].nombre, 
                jugador.vida, arregloPersonajes[jugador.opcionPsj].vidaTotal, jugador.armadura);
            // printf("Tu armadura ha vuelto a 0...\n");
            printf("Turno terminado\n");
        }
    }
    // printf("xd?\n");
    if(hpEn <= 0){
        printf("\n %s ha sido derrotado.\n", arrayEnemigos[numeroEnemigo].nombre);
        encontrarCarta();
        jugador.enemigos[numeroEnemigo] = 1;
    }
    else if(jugador.vida <= 0){
        printf("\nHas sido derrotado.\n");
    }
}


void encontrarCarta(){
    int opcion, pCarta1, pCarta2, pCarta3; /// pCarta: carta aleatoria
    Nodo* nodoAux; ///Se crea un nodo auxiliar que se inicia en la cabeza de la lista para poder recorrer la lista si es necesario
    Nodo* nuevaCarta;
    srand(time(NULL));
    pCarta1 = rand() % 15;  ///Se obtiene número aleatorio de 0 a 14 para recorrer la lista de cartas
    pCarta2 = rand() % 15;
    pCarta3 = rand() % 15;
    printf("\n1. Carta:\n Nombre: %s\n Tipo: %d\n Energía: %d\n Efecto: %s\n", 
            arrayCartas[pCarta1].nombre, arrayCartas[pCarta1].categoria,
                arrayCartas[pCarta1].costoEnergia, arrayCartas[pCarta1].efecto);
    printf("\n2. Carta:\n Nombre: %s\n Tipo: %d\n Energía: %d\n Efecto: %s\n", 
            arrayCartas[pCarta2].nombre, arrayCartas[pCarta2].categoria,
                arrayCartas[pCarta2].costoEnergia, arrayCartas[pCarta2].efecto);
    printf("\n3. Carta:\n Nombre: %s\n Tipo: %d\n Energía: %d\n Efecto: %s\n", 
            arrayCartas[pCarta3].nombre, arrayCartas[pCarta3].categoria,
                arrayCartas[pCarta3].costoEnergia, arrayCartas[pCarta3].efecto);
    printf("¿Desea agregar alguna de estas cartas?\n");
    printf("Ingrese la posición de la carta a agregar\n");
    printf("Si no desea agregar ninguna, ingrese -1\n");
    printf("Ingrese la opción: ");
    scanf("%d", &opcion);
    if(opcion != -1){
        if(opcion == 1){
            jugador.deck[pCarta1] += 1;
        }
        else if(opcion == 2){
            jugador.deck[pCarta2] += 1;
        }
        else if(opcion == 3){
            jugador.deck[pCarta3] += 1;
        }
        printf("La carta ha sido añadida...\n");
    }
}


void crearPersonajeJugador(){
    int tipo, cartaRandom;
    printf("Escoge el personaje: \n");
    printf("1. %s \n Vida = %d \n Oro = %d \n Fuerza = %d \n Dureza = %d \n Inteligencia = %d \n\n", 
        arregloPersonajes[0].nombre, arregloPersonajes[0].vidaTotal, arregloPersonajes[0].oro, 
            arregloPersonajes[0].fuerza, arregloPersonajes[0].dureza, arregloPersonajes[0].inteligencia);
    printf("2. %s \n Vida = %d \n Oro = %d \n Fuerza = %d \n Dureza = %d \n Inteligencia = %d \n\n", 
    arregloPersonajes[1].nombre, arregloPersonajes[1].vidaTotal, arregloPersonajes[1].oro, 
            arregloPersonajes[1].fuerza, arregloPersonajes[1].dureza, arregloPersonajes[1].inteligencia);
    printf("3. %s \n Vida = %d \n Oro = %d \n Fuerza = %d \n Dureza = %d \n Inteligencia = %d \n", 
    arregloPersonajes[2].nombre, arregloPersonajes[2].vidaTotal, arregloPersonajes[2].oro, 
            arregloPersonajes[2].fuerza, arregloPersonajes[2].dureza, arregloPersonajes[2].inteligencia);
    printf("Ingrese opción: ");
    scanf("%d", &tipo);
    if(tipo == 1){
        printf("\nHas escogido a %s, aventurate en las profundidades del abismo...\n", arregloPersonajes[0].nombre);
        jugador.opcionPsj = 0;
        jugador.vida = arregloPersonajes[0].vidaTotal;
        jugador.oro = arregloPersonajes[0].oro;
        jugador.armadura = 0;
        for(int i = 0; i < CARTASIZE; i++){
            if(i == 5){
                jugador.deck[i] = 4;
            }
            else if(i == 10){
                jugador.deck[i] = 4;
            }
            else{
                jugador.deck[i] = 0;
            }
        }
    }
    else if(tipo == 3){
        printf("\nHas escogido a %s, aventurate en las profundidades del abismo...\n", arregloPersonajes[1].nombre);
        jugador.opcionPsj = 1;
        jugador.vida = arregloPersonajes[1].vidaTotal;
        jugador.oro = arregloPersonajes[1].oro;
        jugador.armadura = 0;
        for(int i = 0; i < CARTASIZE; i++){
            if(i == 0){
                jugador.deck[i] = 2;
            }
            else if(i == 5){
                jugador.deck[i] = 2;
            }
            else if(i == 10){
                jugador.deck[i] = 4;
            }
            else{
                jugador.deck[i] = 0;
            }
        }
    }
    else{
        printf("\nHas escogido a %s, aventurate en las profundidades del abismo...\n", arregloPersonajes[2].nombre);
        jugador.opcionPsj = 2;
        jugador.vida = arregloPersonajes[2].vidaTotal;
        jugador.oro = arregloPersonajes[2].oro;
        jugador.armadura = 0;
        for(int i = 0; i < CARTASIZE; i++){
            if(i == 0){
                jugador.deck[i] = 4;
            }
            else if(i == 10){
                jugador.deck[i] = 4;
            }
            else{
                jugador.deck[i] = 0;
            }
        }
    }
    for(int i = 0; i < ENEMIGOSIZE; i++){
        jugador.enemigos[i] = 0;
    }
    // printf("personaje creado\n");
}


void menu(){
    int opcion, d = 0;
    do{
        printf("\n     ~~Menu~~    ");
        printf("\n  1. Luchar");
        printf("\n  2. Bestiario");
        printf("\n  3. Salir");
        printf("\n\n Introduzca opción(1 - 3): ");

        
        scanf("%d", &opcion);
        switch(opcion){
            case 1: printf("\nEntrará en una lucha aleatoria");
                    luchar(1);
            break;
            case 2:;
            break;
            case 3: printf("\nSeleccionó salir: \n");
            break;
            default: printf("\nIngrese una opción valida. \n");
            break;
        }
    }while(opcion != 3);
}


int main(){
    leerTxtEnemigos();
    leerTxtCartas();
    leerTxtPersonajes();
    // for(int i = 0; i < CARTASIZE; i++){
    //     printf("Nombre: %s\nTipo: %d\nNumero de capacidad: %d\nCosto de energia: %d\nEfecto: %s\n", 
    //         arrayCartas[i].nombre,arrayCartas[i].categoria, arrayCartas[i].num, 
    //             arrayCartas[i].costoEnergia, arrayCartas[i].efecto);
    // }
    crearPersonajeJugador();
    menu();
    return 0;
}