#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define SIZE 200
#define ENEMIGOSIZE 30
#define NOMBRESIZE 40
#define LORSIZE 200
#define CARTASIZE 15
#define CATEGORIASIZE 10
#define EFECTOSIZE 100


const char* s = "-";


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


///


void leerTxtEnemigos();

void leerTxtCartas();


///


Enemigo arrayEnemigos[ENEMIGOSIZE];
Carta arrayCartas[CARTASIZE];


int main(){
    leerTxtCartas();
    leerTxtEnemigos();
    for(int i = 0; i < ENEMIGOSIZE; i++){
        printf("Nombre: %s\nVida: %d\nAtaque: %d\nLore: %s\n", 
            arrayEnemigos[i].nombre,arrayEnemigos[i].vida, arrayEnemigos[i].ataque, arrayEnemigos[i].lore);
    }
    printf("\n");
    for(int i = 0; i < CARTASIZE; i++){
        printf("Nombre: %s\nTipo: %d\nNumero de capacidad: %d\nCosto de energia: %d\nEfecto: %s\n", 
            arrayCartas[i].nombre,arrayCartas[i].categoria, arrayCartas[i].num, arrayCartas[i].costoEnergia, arrayCartas[i].efecto);
    }
    return 0;
}


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