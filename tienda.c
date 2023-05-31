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



void tienda(){
    srand(time(NULL));
    int pCarta1, pCarta2, pCarta3, pCarta4, pCarta5, pCarta6, v1, v2, v3, v4, v5, v6, opcionCompra = 0;
    printf("Bienvenido a la tienda\n");
    while(opcionCompra != -1){
        pCarta1 = rand() % 15;
        pCarta2 = rand() % 15;
        pCarta3 = rand() % 15;
        pCarta4 = rand() % 15;
        pCarta5 = rand() % 15;
        pCarta6 = rand() % 15;
        v1 = rand() % 101 + 20;
        v2 = rand() % 101 + 20;
        v3 = rand() % 101 + 20;
        v4 = rand() % 101 + 20;
        v5 = rand() % 101 + 20;
        v6 = rand() % 101 + 20;
        printf("\n1. Carta: \n Valor: %d\n Nombre: %s\n Tipo: %d\n Energía: %d\n Efecto: %s", 
            v1, arrayCartas[pCarta1].nombre, arrayCartas[pCarta1].categoria, 
                arrayCartas[pCarta1].costoEnergia, arrayCartas[pCarta1].efecto);
        printf("\n2. Carta: \n Valor: %d\n Nombre: %s\n Tipo: %d\n Energía: %d\n Efecto: %s", 
            v2, arrayCartas[pCarta2].nombre, arrayCartas[pCarta2].categoria, 
                arrayCartas[pCarta2].costoEnergia, arrayCartas[pCarta2].efecto);
        printf("\n3. Carta: \n Valor: %d\n Nombre: %s\n Tipo: %d\n Energía: %d\n Efecto: %s", 
            v3, arrayCartas[pCarta3].nombre, arrayCartas[pCarta3].categoria, 
                arrayCartas[pCarta3].costoEnergia, arrayCartas[pCarta3].efecto);
        printf("\n4. Carta: \n Valor: %d\n Nombre: %s\n Tipo: %d\n Energía: %d\n Efecto: %s", 
            v4, arrayCartas[pCarta4].nombre, arrayCartas[pCarta4].categoria, 
                arrayCartas[pCarta4].costoEnergia, arrayCartas[pCarta4].efecto);
        printf("\n5. Carta: \n Valor: %d\n Nombre: %s\n Tipo: %d\n Energía: %d\n Efecto: %s", 
            v5, arrayCartas[pCarta5].nombre, arrayCartas[pCarta5].categoria, 
                arrayCartas[pCarta5].costoEnergia, arrayCartas[pCarta5].efecto);
        printf("\n6. Carta: \n Valor: %d\n Nombre: %s\n Tipo: %d\n Energía: %d\n Efecto: %s", 
            v6, arrayCartas[pCarta6].nombre, arrayCartas[pCarta6].categoria, 
                arrayCartas[pCarta6].costoEnergia, arrayCartas[pCarta6].efecto);
        printf("\nSi desea comprar una carta, ingrese la posición, sino, ingrese -1: ");
        scanf("%d", &opcionCompra);
        if(opcionCompra == 1){
            if(jugador.oro >= v1){
                jugador.deck[pCarta1] += 1;
                jugador.oro -= v1;
                printf("Carta comprada con exito, tu oro actual es: %d", jugador.oro);
            }
            else{
                printf("No tienes suficiente dinero para comprar esa carta...\n");
            }
        }
        else if(opcionCompra == 2){
            if(jugador.oro >= v2){
                jugador.deck[pCarta2] += 1;
                jugador.oro -= v2;
                printf("Carta comprada con exito, tu oro actual es: %d", jugador.oro);
            }
            else{
                printf("No tienes suficiente dinero para comprar esa carta...\n");
            }
        }
        else if(opcionCompra == 3){
            if(jugador.oro >= v3){
                jugador.deck[pCarta3] += 1;
                jugador.oro -= v3;
                printf("Carta comprada con exito, tu oro actual es: %d", jugador.oro);
            }
            else{
                printf("No tienes suficiente dinero para comprar esa carta...\n");
            }
        }
        else if(opcionCompra == 4){
            if(jugador.oro >= v4){
                jugador.deck[pCarta4] += 1;
                jugador.oro -= v4;
                printf("Carta comprada con exito, tu oro actual es: %d", jugador.oro);
            }
            else{
                printf("No tienes suficiente dinero para comprar esa carta...\n");
            }
        }
        else if(opcionCompra == 5){
            if(jugador.oro >= v5){
                jugador.deck[pCarta5] += 1;
                jugador.oro -= v5;
                printf("Carta comprada con exito, tu oro actual es: %d", jugador.oro);
            }
            else{
                printf("No tienes suficiente dinero para comprar esa carta...\n");
            }
        }
        if(opcionCompra == 6){
            if(jugador.oro >= v6){
                jugador.deck[pCarta6] += 1;
                jugador.oro -= v6;
                printf("Carta comprada con exito, tu oro actual es: %d", jugador.oro);
            }
            else{
                printf("No tienes suficiente dinero para comprar esa carta...\n");
            }
        }
        else if(opcionCompra > 6){
            printf("Ingrese una opción válida...\n");
        }
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
    else if(tipo == 2){
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


void imprimirDeckJugador(){
    for(int i = 0; i < CARTASIZE; i++){
        if(jugador.deck[i] != 0){
            printf("Carta Nombre: %s\n Tipo: %d\n Energía: %d\n Efecto: %s\n", 
                arrayCartas[i].nombre, arrayCartas[i].categoria, arrayCartas[i].costoEnergia, arrayCartas[i].efecto);
        }
    }
}


int main(){
    leerTxtEnemigos();
    leerTxtCartas();
    leerTxtPersonajes();
    crearPersonajeJugador();
    tienda();
    imprimirDeckJugador();
    return 0;
}
