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

class Aresta
{
public:
    int origem;
    int destino;

    Aresta(int origem, int destino) : origem(origem), destino(destino) {}
    Aresta() {}
};

class Vertice
{
public:
    int grau;

    vector<int> vizinhos;
    Vertice(int grau) : grau(grau) {}
    Vertice() {}

    void adicionarVizinho(int destino)
    {
        vizinhos.push_back(destino);
    }
};

class Emparelhamento
{
public:
    int numArestas;
    vector<Aresta> arestas;

    Emparelhamento(int numArestas) : numArestas(numArestas) {}
    Emparelhamento() {}

    void adicionarAresta(int origem, int destino)
    {
        arestas.push_back(Aresta(origem, destino));
    }

    int verificaSaturacao(int v)
    {
        for (int i = 0; i < numArestas; i++)
        {
            // cout << "v: " << v << " aresta: (" << arestas[i].origem << ", " << arestas[i].destino << ") " << endl;
            if (arestas[i].destino == v || arestas[i].origem == v)
            {
                // cout << "V: " << v << " saturado" << endl
                return 1;
            }
        }
        // cout << "V: " << v << " insaturado" << endl
        return 0;
    }
};

class Grafo
{
public:
    int numVertices;
    int numArestas;
    int particao;
    Emparelhamento emp;
    vector<Vertice> listaAdjacencia;
    vector<int> verticesSaturados;
    vector<int> verticesInsaturados;
    vector<int> predecessores;
    vector<bool> visitados;

    Grafo(int numVertices, int numArestas) : numVertices(numVertices), numArestas(numArestas), visitados(numVertices), predecessores(numVertices) {}
    Grafo(int particao) : particao(particao) {}
    Grafo() {}

    void adicionarVertice(int grau)
    {
        listaAdjacencia.push_back(Vertice(grau));
    }

    void adicionarVizinho(int u, int v)
    {
        listaAdjacencia[u].adicionarVizinho(v);
        listaAdjacencia[v].adicionarVizinho(u);
    }

    void defineSaturadosEInsaturados()
    {

        for (int i = 0; i < numVertices; i++)
        {
            if (emp.verificaSaturacao(i))
            {
                verticesSaturados.push_back(i);
            }
            else
            {
                verticesInsaturados.push_back(i);
            }
        }
    }
    int encontraVerticeEmparelhado(int v)
    {
        for (int i = 0; i < listaAdjacencia[v].vizinhos.size(); i++)
        {
            if (emp.verificaSaturacao(listaAdjacencia[v].vizinhos[i]))
            {
                return listaAdjacencia[v].vizinhos[i];
            }
        }
        return -1;
    }

    int busca(int v)
    {
        for (int i = 0; i < numVertices; i++)
        {
            visitados[i] = false;
            predecessores[i] = -1;
        }
        return buscaRec(v);
    }

    int buscaRec(int v)
    {
        int x = -1;
        if (visitados[v] == false)
        {
            for (int i = 0; i < listaAdjacencia[v].vizinhos.size(); i++)
            {
                int u = listaAdjacencia[v].vizinhos[i];
                
                if (visitados[u] == false)
                {
                    visitados[u] = true;
                    predecessores[u] = v;
                    if (emp.verificaSaturacao(u))
                    {
                        return u;
                    }
                    else
                    {
                        int w = encontraVerticeEmparelhado(u);
                        x = buscaRec(w);
                    }
                }

                if (x != -1)
                {
                    return x;
                }
            }
        }

        return -1;
    }
};

int main(int argv, char **argc)
{

    // Se não forem passados os argumentos necessário o programa é encerrado
    if (argv != 2)
    {
        return 0;
    }

    string linha;
    // Abre o arquivo passado como argumento
    ifstream arquivoEntrada(argc[1]);

    Grafo grafo;
    Emparelhamento emp;

    // Percorre linha por linha do arquivo de entrada
    while (getline(arquivoEntrada, linha))
    {
        istringstream linhaStream(linha);
        string palavra;

        // Percorre cada palavra em cada linha do arquivo de entrada
        while (linhaStream >> palavra)
        {
            if (palavra == "G")
            {
                // Se a linha começa com G, pega o numero de vertices e arestas do grafo
                int numVertices, numArestas;
                linhaStream >> numVertices >> numArestas;
                grafo = Grafo(numVertices, numArestas);
            }
            else if (palavra == "N")
            {
                // Se a linha começa com N, pega as informações de cada vertice do grafo
                int vertice, grau;
                linhaStream >> vertice >> grau;
                grafo.adicionarVertice(grau);
            }
            else if (palavra == "E")
            {
                // Se a linha começa com E, pega as informações de cada aresta do grafo
                int origem, destino;
                linhaStream >> origem >> destino;
                grafo.adicionarVizinho(origem, destino);
            }
            else if (palavra == "B")
            {
                // Se a linha começa com B, pega o vertice que delimita a bipartição do grafo
                int particao;
                linhaStream >> particao;
                grafo.particao = particao;
            }
            else if (palavra == "M")
            {
                // Se a linha começa com M , pega o numero de aresta do emparelhamento
                int numArestasEmp;
                linhaStream >> numArestasEmp;
                emp = Emparelhamento(numArestasEmp);
            }
            else if (palavra == "P")
            {
                // se a linha começa com P, pega as informações de cada aresta do emparelhamento
                int origem, destino;
                linhaStream >> origem >> destino;
                emp.adicionarAresta(origem, destino);
            }
        }
    }
    arquivoEntrada.close();

    grafo.emp = emp;
    grafo.defineSaturadosEInsaturados();

    for(int i = 0; i < grafo.numVertices; i++){
        cout << "V: " << i << " ";
        for(int j = 0; j < grafo.listaAdjacencia[i].vizinhos.size(); j++){

            cout << " " << grafo.listaAdjacencia[i].vizinhos[j];
        }
        cout << endl;
    }

    cout << "Saturados: " << endl;

    for (int i = 0; i < grafo.verticesSaturados.size(); i++)
        std::cout << " " << grafo.verticesSaturados[i] << std::endl;

    cout << "Insaturados: " << endl;

    for (int i = 0; i < grafo.verticesInsaturados.size(); i++)
        std::cout << " " << grafo.verticesInsaturados[i] << std::endl;

    int a = grafo.busca(0);

    for (int i = 0; i < grafo.predecessores.size(); i++)
    {
        cout << i << " " << grafo.predecessores[i] << endl;
    }
}
