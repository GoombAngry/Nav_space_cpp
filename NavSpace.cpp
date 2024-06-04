#include <iostream>
#include <Windows.h>
#include <conio.h>
#include <thread>
#include <chrono>
#include "Game.h"
#include <random>
using namespace std;

int main()
{   
    /*      Testing Params
        (Altura)  - Eje y -> Desde 1 hasta 25
        (Anchura) - Eje x -> Desde 6 hasta 109

        Para ver el marco de juego

        for (int y = 1; y < 26; y++){
            for (int x = 6; x < 110; x++){
                paintCharacter(console, x, y,"*");
            }
        }

        119 -> w
        100 -> d
        115 -> s
        97 -> a
    

    // Vamos a obtener el identificador de la consola que se ejecuta
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE); // Obtenemos el identificador de la consola que ejecuta este programa (En este caso indicamoes en el metodo que controlaremos la salida de la consola)
    ocultarCursor(console);
   
    struct Nave nave (80,25); // Creacion de una nave (Probando el moviento)
    paintCharacter(console, nave.x, nave.y, "<^>");

    while (1) {
        if (_kbhit()) { // Esta funcion devuelve true cuando presionamos una tecla
            int  caracterCode = _getch();
            int yActual = nave.y, xActual = nave.x;
            if (caracterCode == 119) { // Arriba
                if (yActual > 1) {
                    nave.y -= 1;
                    paintCharacter(console, xActual, yActual, "   ");
                    paintCharacter(console, nave.x, nave.y, "<^>");

                }
            }
            else if (caracterCode == 100) { // Derecha
                if (xActual < 109) {
                    nave.x += 1;
                    paintCharacter(console, xActual, yActual, "   ");
                    paintCharacter(console, nave.x, nave.y, "<^>");
                }
            }
            else if (caracterCode == 115) { // Abajo
                if (yActual < 25) {
                    nave.y += 1;
                    paintCharacter(console, xActual, yActual, "   ");
                    paintCharacter(console, nave.x, nave.y, "<^>");
                }
            }
            else { // Izquierda
                if (xActual > 6) {
                    nave.x -= 1;
                    paintCharacter(console, xActual, yActual, "   ");
                    paintCharacter(console, nave.x, nave.y, "<^>");
                }
            }
        }
        this_thread::sleep_for(chrono::milliseconds(30));
    }

    (Valores con los que se hicieron los test)
    - int x[]{ 12,109 }, y[]{ 4,25 };
        
   */
   //      {AnchoTablero}, {altoTablero}
    int x[]{ 12,109 }, y[]{ 4,25 };
    Game game(x,y,3,1,8); // {AnchoTablero}, { altoTablero }, {nVidas}, {velocidadMeteoritos},{nMeteoritos}
    game.runGame();
    return 0;
}
