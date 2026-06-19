#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <queue>
#include <map>
#include <set>
#include <climits>
#include <algorithm>

using namespace std;

struct ConexionNodo2Nodo {
    string nodoInicial;
    string nodoFinal;
    string aristaConexion;
    int tiempo;
    int costo;
};

class Digrafo {
public:
    set<string> Nodos;
    set<string> Aristas;
    vector<ConexionNodo2Nodo> rutas;

    bool cargarDesdeArchivo(const string& nombreArchivo) {
        ifstream archivo(nombreArchivo.c_str());
        if (!archivo.is_open()) return false;
        string linea;
        while (getline(archivo, linea)) {
            if (linea.empty()) continue;
            stringstream ss(linea);
            string ini, fin, idArista;
            int t, c;
            if (ss >> ini >> fin >> idArista >> t >> c) {
                Nodos.insert(ini);
                Nodos.insert(fin);
                Aristas.insert(idArista);
                ConexionNodo2Nodo r = {ini, fin, idArista, t, c};
                rutas.push_back(r);
            }
        }
        archivo.close();
        return true;
    }

    void mostrarEstructuraDigrafo() const {
        cout << "\n================= ELEMENTOS DEL DIGRAFO =================" << endl;

        cout << "V = { ";
        for (set<string>::const_iterator it = Nodos.begin(); it != Nodos.end(); ) {
            cout << *it;
            if (++it != Nodos.end()) cout << ", ";
        }
        cout << " }" << endl;

        cout << "E = { ";
        for (set<string>::const_iterator it = Aristas.begin(); it != Aristas.end(); ) {
            cout << *it;
            if (++it != Aristas.end()) cout << ", ";
        }
        cout << " }" << endl;
        cout << "=========================================================\n" << endl;
    }

    bool existeNodo(const string& nodo) const {
        return Nodos.find(nodo) != Nodos.end();
    }

    void ejecutarDijkstra(const string& origen, const string& destino, int criterio) {
        map<string, int> distancias;
        map<string, string> predecesor;
        map<string, string> aristaPredecesora;

        for (set<string>::const_iterator it = Nodos.begin(); it != Nodos.end(); ++it) {
            distancias[*it] = INT_MAX;
        }
        distancias[origen] = 0;

        priority_queue<pair<int, string>, vector<pair<int, string> >, greater<pair<int, string> > > pq;
        pq.push(make_pair(0, origen));

        while (!pq.empty()) {
            string u = pq.top().second;
            int dist_u = pq.top().first;
            pq.pop();

            if (dist_u > distancias[u]) continue;
            if (u == destino) break;

            for (size_t i = 0; i < rutas.size(); ++i) {
                const ConexionNodo2Nodo& r = rutas[i];
                if (r.nodoInicial == u) {
                    string v = r.nodoFinal;
                    int peso = (criterio == 1) ? r.costo : r.tiempo;

                    if (distancias[u] + peso < distancias[v]) {
                        distancias[v] = distancias[u] + peso;
                        predecesor[v] = u;
                        aristaPredecesora[v] = r.aristaConexion;
                        pq.push(make_pair(distancias[v], v));
                    }
                }
            }
        }

        if (distancias[destino] == INT_MAX) {
            cout << "\nNo existe ruta en el Digrafo (respetando direcciones) entre " << origen << " y " << destino << "." << endl;
        } else {
            cout << "\n>>>> RUTA MINIMA EN DIGRAFO (DIJKSTRA) <<<<" << endl;
            cout << "Criterio: " << (criterio == 1 ? "Costo" : "Tiempo") << " | Total: " << distancias[destino] << endl;

            vector<string> caminoNodos;
            vector<string> caminoAristas;
            string actual = destino;

            while (actual != origen) {
                caminoNodos.push_back(actual);
                caminoAristas.push_back(aristaPredecesora[actual]);
                actual = predecesor[actual];
            }
            caminoNodos.push_back(origen);

            reverse(caminoNodos.begin(), caminoNodos.end());
            reverse(caminoAristas.begin(), caminoAristas.end());

            cout << "Trayectoria dirigida: ";
            for (size_t i = 0; i < caminoNodos.size(); ++i) {
                cout << caminoNodos[i] << (i == caminoNodos.size() - 1 ? "" : " -> ");
            }
            cout << endl;

            cout << "Aristas utilizadas:   ";
            for (size_t i = 0; i < caminoAristas.size(); ++i) {
                cout << caminoAristas[i] << (i == caminoAristas.size() - 1 ? "" : ", ");
            }
            cout << endl;
        }
    }
};

int main() {
    Digrafo dg;
    if (!dg.cargarDesdeArchivo("red.txt")) {
        cout << "Error: No se pudo abrir 'red.txt'. Revisa que este en la misma carpeta." << endl;
        return 1;
    }

    dg.mostrarEstructuraDigrafo();

    string orig, dest;
    cout << "==================== PRACTICA 6: DIGRAFOS ====================" << endl;

    while (true) {
        cout << "Nodo origen: "; cin >> orig;
        if (dg.existeNodo(orig)) break;
        cout << "[!] Error: El nodo '" << orig << "' no pertenece al digrafo. Intenta de nuevo." << endl;
    }

    while (true) {
        cout << "Nodo destino: "; cin >> dest;
        if (dg.existeNodo(dest)) break;
        cout << "[!] Error: El nodo '" << dest << "' no pertenece al digrafo. Intenta de nuevo." << endl;
    }

    dg.ejecutarDijkstra(orig, dest, 1);
    return 0;
}
