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

        Aresta(int origem, int destino): origem(origem), destino(destino){}
};

class Vertice{
    public:
        int grau;
        vector<int> vizinhos;

        Vertice(int grau) : grau(grau), vizinhos(grau){}
};

class Grafo{
    public:
        int numVertices;
        int numArestas;
        vector<Vertice> listaAdjacencia;

        Grafo(int numVertices, int numArestas): numVertices(numVertices), numArestas(numArestas){}
};

class Emparelhamento{
    public:
        int numArestas;
        vector<Aresta> arestas;

        Emparelhamento(int numAretas): numArestas(numArestas),  arestas(numArestas){}
};

int main(){

}