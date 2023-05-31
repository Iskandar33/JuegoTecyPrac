#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define SIZE 30 
#define NOMBRESIZE 20
#define EFECTOSIZE 100
#define CARTASIZE 15
#define CATEGORIASIZE 10


typedef struct Nodo{
    int numCarta;
    int cantCarta;
    struct Nodo *sgte;
}Nodo;


typedef struct Lista{
    int tamaño;
    struct Nodo *cabeza;
}Lista;


typedef struct Carta{
    char nombre[NOMBRESIZE];
    char categoria[CATEGORIASIZE];
    char efecto[EFECTOSIZE];
}Carta;


typedef struct Enemigo{
    char* nombre;
    int vida;
    int ataque;
    char* lore;
} Enemigo;


///


void menu();

void encontrarCarta(Carta [], Lista* listaCartas);

Nodo* crearNodo(int numCarta);

Lista* crearLista();

int deck(Lista* listaCartas, Carta []);

void eliminarNodo(Lista* listaCartas, Carta [], Nodo* nodoAux);

Enemigo crearEn(char* nombre, int vida, int ataque, char* desc);

Enemigo* crearBes(Enemigo* best);

/// 


int main(){
    menu();
    return 0;
}


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

void añadirCarta(Carta cartas[], Lista* listaCartas, int pCarta){
    Nodo* nodoAux = listaCartas->cabeza; ///Se crea un nodo auxiliar que se inicia en la cabeza de la lista para poder recorrer la lista si es necesario
    Nodo* nuevaCarta;
    int flag = 0;
    nuevaCarta = crearNodo(pCarta);
    if(listaCartas->cabeza == NULL){  ///Si la lista esta vacía el nodo creado pasa a ser la cabeza de la lista
        listaCartas->cabeza = nuevaCarta;
        listaCartas->tamaño += 1;
        printf("Cantidad: %d\n", listaCartas->cabeza->cantCarta);
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
            printf("Cantidad: %d\n", nodoAux->cantCarta);
        }
        else{
            nodoAux->sgte = nuevaCarta;  ///Si hay al menos 1 nodo, se adiciona el nodo a la última posición de la lista
            listaCartas->tamaño += 1;
            printf("Cantidad: %d\n", nuevaCarta->cantCarta);
        }
    }
}


void encontrarCarta(Carta cartas[], Lista* listaCartas){
    int pCarta = 0, opcion; /// pCarta: carta aleatoria
    Nodo* nodoAux = listaCartas->cabeza; ///Se crea un nodo auxiliar que se inicia en la cabeza de la lista para poder recorrer la lista si es necesario
    Nodo* nuevaCarta;
    srand(time(NULL));
    pCarta = rand() % 15;  ///Se obtiene número aleatorio de 0 a 14 para recorrer la lista de cartas
    printf(" Nombre: %s\n Categoría: %s\n Efecto: %s\n",
        cartas[pCarta].nombre, cartas[pCarta].categoria, cartas[pCarta].efecto);

    printf(" ¿Desea añadirla al deck?(1: SI, 2: NO): ");
    scanf("%d", &opcion);
    if(opcion == 1){
        añadirCarta(cartas, listaCartas, pCarta);
    }
}


int deck(Lista* listaCartas, Carta cartas[]){
    Nodo* nodoAux = listaCartas->cabeza;
    int num = 0, i = 0, opcion, eliminar;
    if(nodoAux == NULL){
        printf("Todavía no tienes cartas en tu deck\n");
    }
    else{
        while(nodoAux != NULL){
            num = nodoAux->numCarta;
            printf("Posición: %d\nNombre: %s\n Categoría: %s\n Efecto: %s\n Cantidad: %d\n", 
                i, cartas[nodoAux->numCarta].nombre, cartas[num].categoria, cartas[num].efecto, nodoAux->cantCarta);
            i++;
            nodoAux = nodoAux->sgte; 
        }
        printf("Si desea eliminar una carta, ingrese la posición de la carta(-1 si no desea): ");
        scanf("%d", &eliminar);
        if(eliminar > -1){
            nodoAux = listaCartas->cabeza;
            i = 0;
            while(i != eliminar){
                nodoAux = nodoAux->sgte;
            }
            eliminarNodo(listaCartas, cartas, nodoAux);
        }
        
    }
    printf(" ¿Desea volver al menu?(1: SI, 2: NO): ");
    scanf("%d", &opcion);
    return opcion;
}


void eliminarNodo(Lista* listaCartas, Carta cartas[], Nodo* nodoAux){
    int eliminar;
    Nodo* nodoNuevo = listaCartas->cabeza;
    printf(" Cuantas cartas desea eliminar(0 si no desea eliminar ninguna carta): ");
    scanf("%d", &eliminar);
    if((listaCartas->cabeza == nodoAux) && (eliminar == listaCartas->cabeza->cantCarta)){
        listaCartas->cabeza = nodoAux->sgte;
        printf("La carta ha sido eliminada\n");
    }
    else if((listaCartas->cabeza == nodoAux) && (eliminar < listaCartas->cabeza->cantCarta) && (eliminar > 0)){
        listaCartas->cabeza->cantCarta = listaCartas->cabeza->cantCarta - eliminar;
        printf("La carta ha sido eliminada\n");
    }
    else{
        if((eliminar < nodoAux->cantCarta) && (eliminar > 0)){
            nodoAux->cantCarta = nodoAux->cantCarta - eliminar;
            printf("La carta ha sido eliminada\n");
        }
        else if(eliminar == nodoAux->cantCarta){
            while(nodoNuevo->sgte != nodoAux){
                nodoNuevo = nodoNuevo->sgte;
            }
            nodoNuevo->sgte = nodoNuevo->sgte->sgte;
            printf("La carta ha sido eliminada\n");
        }
        else{
            printf("El número es mayor a las cartas actuales. \n");
        }
    }    
}


Enemigo crearEn(char* nombre, int vida, int ataque, char* desc){
	Enemigo ene;
	ene.nombre = nombre;
	ene.vida = vida;
	ene.ataque = ataque;
	ene.lore= desc;
	return ene;
}
Enemigo* crearBes(Enemigo* best){
	best[0] = crearEn("Rey oscuro",500,10,"Antiguo monarca inmortal, en busca de venganza");
	best[1] = crearEn("Sonic",300,15,"Erizo Super Rapido, mas rapido que el sonido");
	best[2] = crearEn("Scorpion",420,9,"Sombra de un antiguo maestro, heraldo del infierno");
	best[3] = crearEn("Tracer",350,11,"Cientifica con la habilidad de teletranporse, nunca falla");
	best[4] = crearEn("D.va",470,9,"Temible chica con un traje robot, no tiene piedad");
	best[5] = crearEn("Reaper",450,6,"Es un peligroso asesino fantasma, siempre que aparece, la muerte lo sigue");
	best[6] = crearEn("GENJI",400,9,"Cybord Ninja, un antiguo dragon covertido en hombre");
	best[7] = crearEn("Reinhard",470,14,"Un campeón que se rige por los códigos caballerescos del valor, la justicia y el coraje. Ataviado con una armadura potenciada y blandiendo un martillo");
	best[8] = crearEn("Widowmarker",450,15,"Una asesina perfecta. Va armada con un rifle de precisión, minas venenosas y la capacidad para engancharse a ubicaciones de tiro ventajosas en un abrir y cerrar de ojos");
	best[9] = crearEn("Mew",3500,100,"Mew es criatura mistica, pocos saben de su existencia y es el origen de toda la vida");
	best[10] = crearEn("Mewtwo",2700,120,"Mewtwo es una copia de Mew creada en un laboratorio, también es uno de los mas poderosos");
	best[11] = crearEn("Articuno",3300,140,"Articuno una de las tres aves lengedarias, Articuno puede controlar el hielo");
	best[12] = crearEn("Moltres",3100,700,"Moltres es otra de las aves legendarias esta es la contraparte de Articuno ya que puede controlar el fuego");
	best[13] = crearEn("Zapdos",4000,500,"Zapdos el menor de las tres aves legendarias, puede controlar los truenos y es es la el mas violento de los tres, su deseo de luchar es insaciable");
	best[14] = crearEn("Arthas",1500,300,"Arthas fue un humano en su vida pasada, pero cayó ante la corrupción y ahora es el dios de la muerte");
	best[15] = crearEn("Sylvanas",3000,200,"Sylvanas fue la primera victima de Arthas, este la condeno a ser una undead y estar bajo sus servicios por toda la eternidad");
	best[16] = crearEn("Jaina",2300,500,"Jaina una hechicera de gran calibre, controla las artes arcanas y se rumorea que tiene una relación con Arthas");
	best[17] = crearEn("Anduin",2900,100,"Anduin un caballero de la luz que traiciono la humanidad por su gran ambicion de poder");
	best[18] = crearEn("Thrall",4500,200,"Thrall es un orco rojo de gran tamaño, el cual fue exiliado de su aldea debido a que controlaba las artes elementales. Son un tabú en su aldea");
	best[19] = crearEn("Goku",5000,1000,"Un super guerrero, ama la lucha y tiene tranformaciones que incrementan su poder");
	best[20]= crearEn("Yasuo", 1500, 200, "La Espada sin Honor");
    best[21]= crearEn("Akshan", 750, 100, "El Centinela Rebelde");
    best[22]= crearEn("Gangplank", 800, 100, "El Azote de los Mares");
    best[23]= crearEn("Graves", 500, 200, "El Forajido");
    best[24]= crearEn("Draven", 300, 250, "El Ejecutor Glorioso");
    best[25]= crearEn("Kindred", 300, 275, "Los Cazadores Eternos");
    best[26]= crearEn("Lucian", 250, 200, "El Destello Purificador");
    best[27]= crearEn("Pyke", 200, 300, "El Destripador de los Puertos");
    best[28]= crearEn("Vladimir", 350, 250, "El Segador Carmesí");
    best[29]= crearEn("Zed", 100, 100, "El Maestro de las Sombras");
    return best;

}

void menu(){    
    int opcion, salir = 0;
    Enemigo* bestiario = (Enemigo*)malloc(sizeof(Enemigo) * SIZE);
    bestiario = crearBes(bestiario);
    Carta cartas[CARTASIZE] = {
        {"Bola de Fuego", "Habilidad", "Golpea al enemigo con una bola de fuego y le causa daño"},
        {"Plasma Shock", "Habilidad", "Golpea al enemigo con un proyectil de plasma causandole daño severo"},
        {"Cañón de Piedra", "Habilidad", "Perfora al enemigo con un proyectil impulsado a gran velocidad"},
        {"Eléctrico", "Habilidad", "Crea una potente descarga de electricidad que daña al enemigo"},
        {"Curar", "Habilidad", "Recupera una pequeña cantidad de vida"},
        {"Ataque Rápido", "Ataque", "Golpea al enemigo con un ataque rápido infligiendo una pequeña cantidad de daño"},
        {"Neutralizar", "Ataque", "Inflige una cantidad moderada de daño al enemigo"},
        {"Perdición", "Ataque", "Azota al enemigo con un fuerte ataque causando gran cantidad de daño"},
        {"Destripar", "Ataque", "Ataca al enemigo varias veces en un ataque generando una cantidad absurda de daño"},
        {"Patada Baja", "Ataque", "Golpea al enemigo con una patada baja causando daño moderado"},
        {"Proteger", "Defensa", "Aumenta tu defensa durante un turno"},
        {"Barrera", "Defensa", "Crea una barrera mágica que debilita los ataques enemigos"},
        {"Reflejar", "Defensa", "Aumenta tu defensa durante 2 turnos"},
        {"Armadura Mágica", "Defensa", "Aumenta tu defensa mágica durante 3 turnos"},
        {"Esquivar", "Defensa", "Esquiva el siguienta ataque enemigo"}
    };
    Lista* listaCartas = crearLista();
    srand(time(NULL));
    do{
        printf("\n     ~~Menu~~");
        printf("\n  1. Caminar");
        printf("\n  2. Deck ");
        printf("\n  3. Bestiario");
        printf("\n  4. Salir");
        printf("\n\n Introduzca opción(1 - 4): ");

        
        scanf("%d", &opcion);
        printf("\n");
        switch(opcion){
            case 1: 
            if(rand() % 2){
                encontrarCarta(cartas, listaCartas);
            }
            else{
                printf("No se encontró nada\n");
            }
            break;
            case 2: 
            while(salir != 1){
                salir = deck(listaCartas, cartas);
            }
            salir = 0;
            break;
            case 3: 
            for(int i = 0; i < SIZE ; i++){
				printf("Nombre: %s\n", bestiario[i].nombre);
				printf("Vida: %d\n", bestiario[i].vida);
				printf("Ataque: %d\n", bestiario[i].ataque);
				printf("Descripción:\n%s\n\n", bestiario[i].lore);
			}
            break;
            case 4: printf("Seleccionó salir: \n");
            break;
            default: printf("\nIngrese una opción valida. \n");
            break;
        }
    }while((opcion != 4));
}
