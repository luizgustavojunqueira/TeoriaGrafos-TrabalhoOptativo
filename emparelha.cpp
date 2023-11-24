/*  Autores:
    Luiz Gustavo S. S. Junqueira
    Camila C. Cáceres
*/

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <climits>
#include <set>

using namespace std;

class Aresta{
    public:
        int origem;
        int destino;
};

class Vertice{
    public:
        int grau;
        vector<int> vizinhos;

        Vertice(int grau) : grau(grau){}
};

class Grafo{
    public:
        int numVertices;
        int numArestas;
        vector<Vertice> listaAdjacencia;
};

class Emparelhamento{
    public:
        int numArestas;
};

int main(){

}