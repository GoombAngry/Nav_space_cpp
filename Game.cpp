#include <Windows.h>
#include <iostream>
#include "Game.h"
#include <chrono>
#include <thread>
#include <conio.h>
#include <random>
#include <string>
using namespace std;


/* Implementacion de constructores */
Meteorito::Meteorito() { /* Default constructor */ }
Disparo::Disparo() { /* Default constructor */ }
Nave::Nave() { /* Default constructor */ }
Game::Game() { /* Default constructor */ }
Game::Game(int x[2], int y[2], int vidas,int speedMeteoritos,int nMeteoritos) {
    // Inicializamos el iterador de identificadores para los meteoritos
    this->iterIdsMeteoritos = 0;
    if (speedMeteoritos < 0) { this->speedMeteoritos = 0; }
    if (vidas <= 0) { this->lifes = 3; }
    // El numero de meteoritos sera proporcional a la anchura del tablero un poco menos
    int nMeteoritosPuedenHaber = (x[1] - 2) - (x[0] - 2); // Numero de meteoritos que puede caber
    if (nMeteoritos < 0 || nMeteoritos > nMeteoritosPuedenHaber) { // Si se superan el numero de meteoritos proporcional a lo que puede haber si asignara por defecto 3 meteoritos
        nMeteoritos = 3;
    }
    
    this->nMeteoritos = nMeteoritos;
    this->speedMeteoritos = speedMeteoritos;
    this->lifes = vidas;

    this->score = 0; // Puntuacion inicial

	this->x[0] = x[0]; // Guardamos inicio x
	this->x[1] = x[1]; // Guardamos final x

	this->y[0] = y[0]; // Guardamos inicio y
	this->y[1] = y[1]; // Guardamos final y
	// cout << this->x[0] << " -> " << this->x[1] << endl;
	// cout << this->y[0] << " -> " << this->y[1] << endl;

    

    // Creacion de la nave
    this->nave = Nave((this->x[1] / 2), (this->y[1] - 1), "<^>");

    // Configurando cursor
	this->console = GetStdHandle(STD_OUTPUT_HANDLE);
	this->cursor.bVisible = FALSE; // Para que sea invisible el cursor
	this->cursor.dwSize = 1; // El size del cursor (va desde 1 hasta 100)
	SetConsoleCursorInfo(this->console, &cursor); // Seteamos la configuracion del cursor en la terminal que se esta ejecutando el programa

    // Generamos los meteoritos (seran 9 por defecto)
    //this->generatedMeteoritos(9);
}
Nave::Nave(int x, int y, string icon) {
    this->x = x;
    this->y = y;
    this->icon = icon;
}
Disparo::Disparo(int x, int y, string icon) {
    this->x = x;
    this->y = y;
    this->icon = icon;
}
Meteorito::Meteorito(int x, int y, string icon) {
    this->x = x;
    this->y = y;
    this->icon = icon;
}

/* Implementacion de los metodos */
void Meteorito::getInfo() {
    cout << this->x << " - " << this->y << " " << this->icon << endl;
}
void Game::printCharacter(int x, int y, string character) { 
	COORD coordenadas;
	coordenadas.X = x;
	coordenadas.Y = y;
	SetConsoleCursorPosition(this->console, coordenadas);
	cout << character;
}
void Game::printMarco() {
	for (int coordY = this->y[0]; coordY <= this->y[1]; coordY++)
	{
		if (coordY == this->y[0] || coordY == this->y[1]) { // Para dibujar el tope y el bajo del marco
			for (int coordX = this->x[0]; coordX <= this->x[1]; coordX++)
			{
				this->printCharacter(coordX, coordY, "=");
			}
		}
		else {
			this->printCharacter(this->x[0], coordY, "|");
			this->printCharacter(this->x[1], coordY, "|");
		}
	}
}
void Game::moverNave() {
    if (_kbhit()) { // Esta funcion devuelve true cuando presionamos una tecla
        int  caracterCode = _getch();
        int yActual = this->nave.y, xActual = this->nave.x; // Obtenemos coordenadas actuales de la nave
        if (caracterCode == 119) { // Arriba
            if (yActual > this->y[0] + 1) {
                this->nave.y -= 1;
                this->printCharacter(xActual, yActual, "   ");
                this->printCharacter(this->nave.x, this->nave.y, this->nave.icon);

            }
        }
        else if (caracterCode == 100) { // Derecha
            if (xActual < (this->x[1] - Game::DELIMITER_RIGHT_X)) {
                this->nave.x += 1;
                this->printCharacter(xActual, yActual, "   ");
                this->printCharacter(this->nave.x, this->nave.y, "<^>");
            }
        }
        else if (caracterCode == 115) { // Abajo
            if (yActual < this->y[1] - 1) {
                nave.y += 1;
                this->printCharacter(xActual, yActual, "   ");
                this->printCharacter(this->nave.x, this->nave.y, this->nave.icon);
            }
        }
        else if (caracterCode == 97) { // Izquierda
            if (xActual > (this->x[0] + Game::DELIMITER_LEFT_X)) {
                this->nave.x -= 1;
                this->printCharacter(xActual, yActual, "   ");
                this->printCharacter(this->nave.x, this->nave.y, this->nave.icon);
            }
        }
        else if (caracterCode == 32) { // Disparo
            // Vamos a comprobar que no existe ningun meteorito en frente de la nave
            struct Disparo newDisparo = Disparo((this->nave.x + 1), (this->nave.y - 1), "|"); // Generamos el disparo
            bool isCollision = false;
            for (int i = 0; i < this->meteoritos.size(); i++)
            {
                struct Meteorito* m = &this->meteoritos.at(i);
                if (m->y == newDisparo.y && m->x == newDisparo.x) { // Meterito en frente detectacto (Lo eliminamos y la bala no es necesario meterla en el vector de los disparos)
                    m->x = this->randomNumber(this->x[0] + 2, this->x[1] - 2);
                    m->y = this->y[0] + 1;
                    this->score += 5;
                    this->updateScore();
                    this->printCharacter(m->x, m->y," "); // Eliminamos el meteorito
                    isCollision = true;
                }
            }
            if (!isCollision) { // En el caso de que no haya un meteorito delante al generar el disparo
                newDisparo.id_Disparo = this->iterIdsMeteoritos; // Asignamos un identificador para el disparo
                this->iterIdsMeteoritos += 1; // Incrementamos el identificador para el siguiente
                this->disparos.push_back(newDisparo); // Guardamos el disparo
            }
            
            
        }
    }
}
void Game::runGame() {
    this->printMarco(); // Pintamos el marco 
    this->printCharacter(this->nave.x, this->nave.y, this->nave.icon); // Pintamos la nave
    this->updateScore(); // Pintamos el marcador de puntos
    this->updateLifes(); // Pintamos el marcador de vidas
    thread generatedMeteoritos(&Game::generatedMeteoritos, this, this->nMeteoritos); // Se iran generando los meteoritos cada segundo 

    /* - EN GAME - */
    while (this->lifes != 0) { // El juego continuara hastaq ue queden 0 vidas
        this->printCharacter(0, 0, to_string(this->disparos.size())); // Imprimo el numero de disparos que existen
        this->printCharacter(30, 0, to_string(this->meteoritos.size())); // Imprimo el numero de meteoritos que existen
        this->moverNave(); // Para mover la nave
        this->drawMeteoritos(); // Pintamos los meteoritos
        this->drawDisparos(); // Pintamos los disparos
        this_thread::sleep_for(chrono::milliseconds(20));
        this->checkDisparos(); // Comprueba los disparos
        this_thread::sleep_for(chrono::milliseconds(20));
        this->checkMeteoritos(); // Comprueba los meteoritos
        this->clearDisparos();
    }
    generatedMeteoritos.join();
}
int Game::randomNumber(int min,int max) {
    random_device seed; // Obtenemos una semilla aleatoria
    mt19937 machineNumber(seed()); // Creamos un generador con la semilla generadas anteriormente
    int minimun(min), maximun(max); // Parametros del rango que queremos obtener del numero aleatoriamente
    uniform_int_distribution<int> distribution(minimun, maximun);
    return distribution(machineNumber);
}
void Game::checkDisparos() {
    vector<int> idShotsDeleted; // Guardamos los id de los disparos a eliminar
    for (int indexDisparo = 0; indexDisparo < this->disparos.size(); indexDisparo++)
    {
        struct Disparo* d = &this->disparos.at(indexDisparo);
        bool collision = false;
        if (d->y <= (this->y[0]+1)) { // Se encuentra en el tablero (Queda ver si hay un meteorito)
            collision = true;
            idShotsDeleted.push_back(d->id_Disparo);
        }
        else { // Comprobamos si hay un meteorito el la siguiente posicion
            for (int indexMeteorito = 0; indexMeteorito < this->meteoritos.size(); indexMeteorito++)
            {
                struct Meteorito* m = &this->meteoritos.at(indexMeteorito);
                if (d->y - 1 == m->y && d->x == m->x) { // El disparo tiene encima un meteorito
                    this->score += 5;
                    this->updateScore();
                    idShotsDeleted.push_back(d->id_Disparo);
                    this->printCharacter(m->x, m->y, " "); // Quitamos el meteorito
                    this->printCharacter(d->x, d->y, " "); // Quitamos el disparo
                    m->x = this->randomNumber(this->x[0] + 2, this->x[1] - 2);
                    m->y = this->y[0] + 1;
                    // Damos al meteorito nuevas coordenadas de reaparicion
                    collision = true;
                }
            }
        }
        if (!collision) { // No existe fuera del tablero ni colision
            this->printCharacter(d->x, d->y, " ");
            d->y -= 1;
            this->printCharacter(d->x, d->y,d->icon);

        }
        else {
            this->printCharacter(d->x, d->y, " "); // Si llega al final del tablero lo borramos por coordenada
        }
    }
    // Logica para eliminar los disparos que se salen o chocan
    for (int i : idShotsDeleted) {
        this->removedDisparoById(i);
    }
}
void Game::updateScore() {
    this->printCharacter(this->x[0], (this->y[0] - 1), "Score = " + to_string(this->score));
}
void Game::updateLifes() {
    this->printCharacter((this->x[1] - 10), (this->y[0] - 1), "Vidas = " + to_string(this->lifes));
}
void Game::checkMeteoritos() {
    vector<int> idShootDeleted;
    for (int indexMeteorito = 0; indexMeteorito < this->meteoritos.size(); indexMeteorito++)
    {
        bool isCollision = false;
        struct Meteorito* m = &this->meteoritos.at(indexMeteorito);
        // Vamos a comprobar que hay debajo de cada meteorito
        for (int indexDisparo = 0; indexDisparo < this->disparos.size(); indexDisparo++)
        {
            struct Disparo* d = &this->disparos.at(indexDisparo);
            if (m->y + 1 == d->y && m->x == d->x) { // El meteorito tiene un disparo justo debajo (Eliminamos el disparo y cambiamos de posicion al meteorito)
                this->score += 5;
                this->updateScore();
                idShootDeleted.push_back(d->id_Disparo); // Guardamos el indice del disparo a eliminar
                this->printCharacter(m->x, m->y, " "); // Eliminamos el meteorito dibujado
                this->printCharacter(d->x, d->y, " "); // Eliminamos el disparo dibujado
                isCollision = true; // Hay colision y no se modifica las coordenadas de este meteorito
                break;
            }
        }
        if (m->y >= this->y[1] - 1) { // Final del tablero
            this->printCharacter(m->x, m->y, " ");
            isCollision = true;
        }
        else if (m->y + 1 == this->nave.y && m->x >= this->nave.x && m->x <= this->nave.x + 2) { // Choca con la nave
            this->lifes -= 1; // Quitamos una vida
            this->updateLifes(); // Refrescamos las vidas
            this->printCharacter(m->x, m->y, " ");
            isCollision = true;
        }
        if (!isCollision) { // Si ese meteorito no colisiona seguira bajando por el tablero
            //cout << m->vueltas;
            if (m->vueltas == this->speedMeteoritos) {
                m->vueltas = 0;
                this->printCharacter(m->x, m->y, " ");
                m->y += 1;
            }
            else {
                m->vueltas += 1;
            }
        }
        else {
            
            m->x = this->randomNumber(this->x[0] + 2, this->x[1] - 2);
            m->y = this->y[0] + 1;
        }
    }

    // Logica para eliminar los disparos que se salen o chocan
    for (int i : idShootDeleted) {
        this->removedDisparoById(i);
    }

    
    
}
void Game::drawDisparos() {
    for (Disparo d : this->disparos) {
        this->printCharacter(d.x, d.y, d.icon);
    }
}
void Game::drawMeteoritos() {
    for (int i = 0; i < this->meteoritos.size(); i++)
    {
        struct Meteorito* m = &this->meteoritos.at(i);
        this->printCharacter(m->x, m->y, m->icon);
    }
}
void Game::clearDisparos() {
    for (Disparo d : this->disparos) {
        this->printCharacter(d.x,d.y + 1," ");
    }
}
void Game::generatedMeteoritos(int nMeteoritos) {
    while (nMeteoritos > 0) {
        struct Meteorito meteorito = Meteorito(this->randomNumber(this->x[0] + 2, this->x[1] - 2),this->y[0] + 1,"*");
        while (!this->isUniqueMeteorito(meteorito.x, meteorito.y)) {
            cout << "!";
            meteorito.x = this->randomNumber(this->x[0] + 2, this->x[1] - 2);
        }
        meteorito.vueltas = 0;        
        this->meteoritos.push_back(meteorito);
        nMeteoritos -= 1;
        this_thread::sleep_for(chrono::seconds(1));
    }
}
bool Game::isUniqueMeteorito(int x,int y) {
    bool isUnique = true;
    for (int i = 0; i < this->meteoritos.size(); i++)
    {
        struct Meteorito* mIter = &this->meteoritos.at(i);
        if (mIter->x == x && mIter->y == y) {
            isUnique = false;
            break;
        }
    }

    return isUnique;
}
void Game::removedDisparoById(int idDisparo) { 
    int indexRemoved = 0;
    for (int i = 0; i < this->disparos.size(); i++)
    {
        struct Disparo* d = &this->disparos.at(i);
        if (d->id_Disparo == idDisparo) {
            indexRemoved = i;
            break;
        }
    }
    this->disparos.erase(this->disparos.begin() + indexRemoved);

}
