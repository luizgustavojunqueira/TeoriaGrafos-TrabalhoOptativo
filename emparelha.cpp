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

        void adicionarVizinho(int destino){
            vizinhos.push_back(destino);
        }

};

class Grafo{
    public:
        int numVertices;
        int numArestas;
        vector<Vertice> listaAdjacencia;
        vector<int> verticesSaturados;
        vector<int> verticesNaoSaturados;

        Grafo(int numVertices, int numArestas): numVertices(numVertices), numArestas(numArestas),listaAdjacencia(numVertices), verticesSaturados(numVertices), verticesNaoSaturados(numVertices){}
        Grafo(){}

        void adicionarVertice(int grau){
            listaAdjacencia.push_back(Vertice(grau));
        }

        void adicionarVizinho(int u, int v){
            listaAdjacencia[u].adicionarVizinho(v);
        }
};

class Emparelhamento{
    public:
        int numArestas;
        vector<Aresta> arestas;

        Emparelhamento(int numAretas): numArestas(numArestas),  arestas(numArestas){}

        void adicionarAresta(int origem, int destino){
            arestas.push_back(Aresta(origem,destino));
        }

};

int verificaSaturacao(Emparelhamento *emp, int v){
    for(int i=0; i<emp->numArestas;i++){
        if(emp->arestas[i].destino==v || emp->arestas[i].origem==v) 
            return 1;
        else
            return 0;    
    }
        

}
int ListSaturadosNaoSaturados(Grafo *grafo, Emparelhamento *emp){
    for(int i=0; i<grafo->numVertices;i++){

        if(verificaSaturacao(emp,i)){
            grafo->verticesSaturados.push_back(i);   
        }
        else{
            grafo->verticesNaoSaturados.push_back(i);
        }

    }
}

int main(int argv, char** argc){

    //Se não forem passados os argumentos necessário o programa é encerrado
    if(argv != 3){
        return 0;
    }

    string linha;
    // Abre o arquivo passado como argumento
    ifstream arquivoEntrada(argc[1]);

    // Pega o vertice de origem passado como argumento
    int s = stoi(argc[2]);

    int numVertices;
    int numArestas;

    Grafo grafo;

    // Percorre linha por linha do arquivo de entrada
    while(getline(arquivoEntrada, linha)){
        istringstream linhaStream(linha);
        string palavra;
        
        // Percorre cada palavra em cada linha do arquivo de entrada
        while(linhaStream >> palavra){
            if(palavra == "I"){
                // Se a linha começa com G, pega o numero de vertices e arestas do grafo
                linhaStream >> numVertices >> numArestas;
                grafo = Grafo(numVertices, numArestas);
            }else if(palavra == "N"){
                // Se a linha começa com N, pega as informações de cada vertice do grafo
                int vertice,grau;
                linhaStream >> vertice >> grau;
                grafo.adicionarVertice(grau);
            }else if(palavra == "E"){
                // Se a linha começa com E, pega as informações de cada aresta do grafo
                int origem, destino;
                linhaStream >> origem >> destino;
                grafo.adicionarVizinho(origem, destino);
            }
        }

    }
    
    arquivoEntrada.close();
