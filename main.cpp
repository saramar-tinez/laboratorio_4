#include <iostream>
#include <vector>
#include <map>
#include <list>
#include <fstream>
#include <sstream>
#include <limits>
using namespace std;

// CLASE ENLACE
/* Representa la conexion entre dos enrutadores
 * con su costo asociado*/

class Enlace {
public:
    string destino; // nombre del enrutador destino
    int    costo;   // costo del enlace

    Enlace(string d, int c) {
        destino = d;
        costo   = c;
    }
};
// CLASE ENRUTADOR
// Representa un enrutador de la red
// Guarda sus vecinos y su tabla de costos
class Enrutador {
private:
    string nombre; // identificador del enrutador (A, B, C...)

    // vecinos directamente conectados y el costo de cada enlace
    // usamos vector porque la cantidad de vecinos puede variar
    vector<Enlace> vecinos;

    // tabla de costos hacia otros destinos
    // map<destino, costo> -> acceso rapido por nombre
    map<string, int> tablaCostos;

    // tabla de caminos: guarda el camino completo a cada destino
    // map<destino, list<string>> -> lista de nodos del camino
    map<string, list<string>> tablaCaminos;

public:
    // constructor
    Enrutador(string n) {
        nombre = n;
        // el costo a si mismo es 0
        tablaCostos[nombre] = 0;
    }

    // getters
    string getNombre() { return nombre; }

    // agrega un vecino con su costo de enlace
    void agregarVecino(string destino, int costo) {
        // revisamos que no exista ya ese vecino
        for (int i = 0; i < (int)vecinos.size(); i++) {
            if (vecinos[i].destino == destino) {
                vecinos[i].costo = costo; // actualizamos si ya existe
                return;
            }
        }
        vecinos.push_back(Enlace(destino, costo));
        tablaCostos[destino] = costo; // costo inicial = costo directo
    }

    // elimina un vecino
    void eliminarVecino(string destino) {
        for (int i = 0; i < (int)vecinos.size(); i++) {
            if (vecinos[i].destino == destino) {
                vecinos.erase(vecinos.begin() + i);
                tablaCostos.erase(destino);
                tablaCaminos.erase(destino);
                return;
            }
        }
    }
