#pragma once
#include <Windows.h>
#include <iostream>
#include <vector>
using namespace std;

struct Nave {
	int x, y;
	string icon;
	Nave(int,int,string);
	Nave();
};
struct Disparo {
	int x, y,id_Disparo;
	string icon;
	Disparo();
	Disparo(int,int,string);
};
struct Meteorito {
	int x, y,vueltas;
	string icon;
	Meteorito();
	Meteorito(int, int, string);
	void getInfo(); // Obtener toda la informacion sobre ese meteorito
};
class Game {
private: 
	int x[2],y[2]; // Guardan el inicio y fin de cada coordenada (Para marcar el tablero) = [inicio,fin]
	int score; // Guardara la puntuacion del jugador
	int lifes; // Guardara las vidas del jugador
	int nMeteoritos; // Guardara el numero de meteorios que habran en el tablero
	int speedMeteoritos; // Cuanto mayor sea el numero mas rapido iran (El numero mas bajo a poner es 0)
	int iterIdsMeteoritos ; // El asignador de ids para identificar a los disparos
	static const int DELIMITER_LEFT_X = 1; // Esto indica lo que hay que sumar para determinar el limite de lateral izquierdo del juego
	static const int DELIMITER_RIGHT_X = 3; // Esto indica lo que hay que restar para determinar el limite de lateral derecho del juego
	struct Nave nave; // Guardara informacion respecto a la nave del jugador
	vector<Disparo> disparos; // En este vector se guardaran los disparos de la nave
	vector<Meteorito> meteoritos; // En este vector se almacenara los meteoritos existente en el tablero
	HANDLE console; // Va a contener informacion acerca de la consola que ejecuta el programa
	CONSOLE_CURSOR_INFO cursor; // Va a contener informacion acerca del cursor de la ventana


public:
	Game();
	Game(int[], int[],int,int,int);
	int randomNumber(int,int); // Generara un numero aleatorio establecido por un rango pasado por parametro (Para la aparicion de los meteoritos)
	void printCharacter(int, int, string); // Dibujara un caracter pasado en una posicion dada
	void printMarco(); // Imprime el marco del juego (Incluyendo puntuacion y vidas)
	void runGame(); // Este lanzara un hilo ejecutando el metodo listenerKeyboard
	void moverNave(); // Este metodo servira para mover la nave
	void checkDisparos(); // Comprobara y dibujara si choca con un meteorito o se sale del marco
	void updateScore(); // Este metodo actualizara la puntuacion cuando se le llame
	void updateLifes(); // Este metodo actualizara las vidas cuando se le llame
	void checkMeteoritos(); // Este metodo comprobara si un meteorito se sale o choca con la nave
    void drawDisparos(); // Dibuja los disparos en pantalla
    void drawMeteoritos(); // Dibujamos los meteoritos
	void clearDisparos(); // Este metodo limpiara los disparos de la pantalla
	void generatedMeteoritos(int); // Este metodo generara los meteoritos
	bool isUniqueMeteorito(int,int); // Se pasara unas coordenas nuevas e indicaran si son unicas
	void removedDisparoById(int); // Este eliminara los disapros del tablero (Pasando el id del disparo)
};
