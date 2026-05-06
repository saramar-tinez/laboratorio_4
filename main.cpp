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

    // retorna los vecinos del enrutador
    vector<Enlace>& getVecinos() { return vecinos; }

    // retorna el costo a un destino (-1 si no existe)
    int getCosto(string destino) {
        if (tablaCostos.count(destino))
            return tablaCostos[destino];
        return -1;
    }

    // actualiza el costo a un destino
    void setCosto(string destino, int costo) {
        tablaCostos[destino] = costo;
    }

    // guarda el camino completo a un destino
    void setCamino(string destino, list<string> camino) {
        tablaCaminos[destino] = camino;
    }

    // retorna el camino a un destino
    list<string> getCamino(string destino) {
        if (tablaCaminos.count(destino))
            return tablaCaminos[destino];
        return list<string>(); // lista vacia si no hay camino
    }

    // imprime la tabla de costos del enrutador
    void mostrarTabla() {
        cout << "\nTabla de enrutamiento de " << nombre << ":" << endl;
        cout << "Destino | Costo | Camino" << endl;
        cout << "--------|-------|-------" << endl;
        for (auto& par : tablaCostos) {
            cout << par.first << "       | " << par.second << "     | ";
            // imprime el camino si existe
            if (tablaCaminos.count(par.first)) {
                list<string>& cam = tablaCaminos[par.first];
                for (auto it = cam.begin(); it != cam.end(); it++) {
                    if (it != cam.begin()) cout << " -> ";
                    cout << *it;
                }
            }
            cout << endl;
        }
    }
};

// =====================================================
// CLASE RED
// Representa la topologia completa de enrutadores
// Gestiona conexiones y calcula caminos optimos
// =====================================================
class Red {
private:
    // map<nombre, Enrutador*> -> acceso rapido por nombre
    map<string, Enrutador*> enrutadores;

public:
    // destructor: libera memoria de los enrutadores
    ~Red() {
        for (auto& par : enrutadores)
            delete par.second;
    }

    // agrega un enrutador a la red
    void agregarEnrutador(string nombre) {
        if (enrutadores.count(nombre)) {
            cout << "El enrutador " << nombre << " ya existe." << endl;
            return;
        }
        enrutadores[nombre] = new Enrutador(nombre);
        cout << "Enrutador " << nombre << " agregado." << endl;
    }

    // elimina un enrutador y todos sus enlaces
    void eliminarEnrutador(string nombre) {
        if (!enrutadores.count(nombre)) {
            cout << "El enrutador " << nombre << " no existe." << endl;
            return;
        }
        // eliminamos los enlaces que apuntan a este enrutador
        for (auto& par : enrutadores)
            par.second->eliminarVecino(nombre);

        delete enrutadores[nombre];
        enrutadores.erase(nombre);
        cout << "Enrutador " << nombre << " eliminado." << endl;
        // recalculamos los caminos
        calcularCaminos();
    }

    // agrega un enlace bidireccional entre dos enrutadores
    void agregarEnlace(string origen, string destino, int costo) {
        if (!enrutadores.count(origen) || !enrutadores.count(destino)) {
            cout << "Error: uno de los enrutadores no existe." << endl;
            return;
        }
        // el enlace es bidireccional
        enrutadores[origen]->agregarVecino(destino, costo);
        enrutadores[destino]->agregarVecino(origen, costo);
        cout << "Enlace " << origen << " <-> " << destino
             << " con costo " << costo << " agregado." << endl;
        calcularCaminos(); // recalculamos al cambiar la topologia
    }
