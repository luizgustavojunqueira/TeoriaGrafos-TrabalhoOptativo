/*
    Autores:
        Luiz Gustavo S. S. Junqueira
        Camila C. Cáceres
*/

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>

using namespace std;

// Classe para armazenar informações sobre uma Aresta do Grafo, no caso sua origem e seu destino
class Aresta
{
public:
    int origem;
    int destino;

    // Construtores
    Aresta(int origem, int destino) : origem(origem), destino(destino) {}
    Aresta() {}
};

// Classe para armazenar informações sobre um Vértice do grafo, no caso, seu grau e seus vizinhos
class Vertice
{
public:
    int grau;
    vector<int> vizinhos;

    // Construtores
    Vertice(int grau) : grau(grau) {}
    Vertice() {}

    // Adiciona o vizinho v ao vértice
    void adicionarVizinho(int v)
    {
        vizinhos.push_back(v);
    }
};

// Classe para armazenar o emparelhamento como um vetor de Arestas
class Emparelhamento
{
public:
    int numArestas;
    vector<Aresta> arestas;

    // Construtores
    Emparelhamento(int numArestas) : numArestas(numArestas) {}
    Emparelhamento() {}

    // Cria uma nova aresta e adiciona no vetor do emparelhamento
    void adicionarAresta(int origem, int destino)
    {
        arestas.push_back(Aresta(origem, destino));
    }

    // Verifica se o vértice V é saturado, ou sejá, está no emparelhamento
    int verificaSaturacao(int v)
    {
        // Percorre todas as arestas do emparelhamento
        for (int i = 0; i < numArestas; i++)
        {
            // Verifica se a aresta contem o vértice V
            if (arestas[i].destino == v || arestas[i].origem == v)
            {
                return 1; // Retorna 1 caso V seja saturado
            }
        }
        return 0; // Retorna 0 caso V seja insaturado
    }
};

// Classe para armazenar informações sobre o Grafo
class Grafo
{
public:
    int numVertices;
    int numArestas;
    int particao;                    // Bipartição do grafo apesar de não ser utilizado
    Emparelhamento emp;              // Emparelhamento do grafo
    vector<Vertice> listaAdjacencia; // Vetor que representa o grafo
    vector<int> verticesSaturados;
    vector<int> verticesInsaturados;
    // Vetores utilizados para calcular os caminhos M-Aumentantes
    vector<bool> visitados;
    vector<int> predecessores;

    // Construtores
    Grafo(int numVertices, int numArestas) : numVertices(numVertices), numArestas(numArestas), visitados(numVertices), predecessores(numVertices) {}
    Grafo(int particao) : particao(particao) {}
    Grafo() {}

    // Cria um novo vértice com o grau passado e adiciona na lista de vértices do grafo
    void adicionarVertice(int grau)
    {
        listaAdjacencia.push_back(Vertice(grau));
    }

    // Adiciona V como vizinho de U e U como vizinho de V
    void adicionarVizinho(int u, int v)
    {
        listaAdjacencia[u].adicionarVizinho(v);
        listaAdjacencia[v].adicionarVizinho(u);
    }

    // Classifica os vértices do grafo como saturados ou insaturados
    void defineSaturadosEInsaturados()
    {
        // Percorre os vértices do grafo
        for (int i = 0; i < numVertices; i++)
        {
            // Chama a função da classe Emparelhamento que verifica se um vértice passado está no mesmo
            if (emp.verificaSaturacao(i))
            {
                verticesSaturados.push_back(i); // Caso I seja saturado, adiciona no vetor de vértices saturados
            }
            else
            {
                verticesInsaturados.push_back(i); // Caso I não seja saturado, adiciona no vetor de vértices insaturados
            }
        }
    }

    // Encontra o vértice emparelhado com V no emparelhamento
    int encontraVerticeEmparelhado(int v)
    {
        // Percorre os vizinhos de V
        for (int i = 0; i < listaAdjacencia[v].grau; i++)
        {
            // Verifica se o vizinho I de V é saturado
            if (emp.verificaSaturacao(listaAdjacencia[v].vizinhos[i]))
            {
                return listaAdjacencia[v].vizinhos[i]; // Caso seja saturado, retorna ele
            }
        }
        return -1; // Caso V não tenha vizinhos no emparelhamento, retorna -1
    }

    // Inicializa a busca por um caminho M-Aumentante a partir do vértice V
    int busca(int v)
    {
        // Percorre os vértices do grafo
        for (int i = 0; i < numVertices; i++)
        {
            visitados[i] = false;  // Marca o vértice como não visitado
            predecessores[i] = -1; // Marca o predecessor do vértice como inválido
        }

        return buscaRec(v); // Realiza a busca
    }

    // Função que percorre o grafo a partir de um vértice insaturado V, construindo um caminho M-Aumentante
    int buscaRec(int v)
    {
        int x = -1;
        // Caso V ainda não tenha sido visitado
        if (visitados[v] == false)
        {
            // Percorre os vizinhos de V
            for (int i = 0; i < listaAdjacencia[v].grau; i++)
            {
                int u = listaAdjacencia[v].vizinhos[i]; // U é um vizinho de V

                // Caso U ainda não tenha sido visitado
                if (visitados[u] == false)
                {
                    visitados[u] = true;  // Marca u como visitado
                    predecessores[u] = v; // Marca o predecessor de U como V

                    // Caso U não seja saturado, retorna U
                    if (!emp.verificaSaturacao(u))
                    {
                        return u;
                    }
                    else // Caso U seja saturado
                    {
                        // Como U está no emparelhamento, ele possui um vizinho que também está, este vizinho é W
                        int w = encontraVerticeEmparelhado(u);

                        // Marca o predecessor de W como U
                        predecessores[w] = u;

                        // Chama a função recursivamente, agora a partir de W
                        x = buscaRec(w);
                    }
                }
                // Caso X seja um vértice válido
                if (x != -1)
                {
                    return x;
                }
            }
        }

        return -1; // Caso V tenha sido visitado, retorna -1
    }

    // Escreve na saída os vértices saturados e insaturados do Grafo
    void printarSaturadosInsaturados()
    {
        int qntSaturados = verticesSaturados.size();
        int qntInsaturados = verticesInsaturados.size();

        cout << "S";

        // Percorre os vértices saturados, escrevendo-os na saída
        for (int i = 0; i < qntSaturados; i++)
        {
            cout << " " << verticesSaturados[i];
        }

        cout << endl;

        cout << "I";

        // Percorre os vértices insaturados, escrevendo-os na saída
        for (int i = 0; i < qntInsaturados; i++)
        {
            cout << " " << verticesInsaturados[i];
        }

        cout << endl;
    }

    // Busca os caminhos M-Aumentantes a partir de cada vértice insaturado no Grafo
    void buscarCaminhosMAumentantes()
    {
        // Percorre os vértices insaturados do grafo
        for (int i = 0; i < (int)verticesInsaturados.size(); i++)
        {
            // A variável A contém o último vértice do caminho encontrado ou -1, caso não tenha encontrado
            int a = busca(verticesInsaturados[i]); // Realiza a busca a partir do vértice insaturado

            // Caso tenha encontrado um caminho
            if (a != -1)
            {
                printarCaminho(a); // Escreve o caminho na saída
            }
            else
            {
                // Caso não tenha encontrado um caminho
                cout << "N " << verticesInsaturados[i] << endl;
            }
        }
    }

    // Escreve o caminho M-Aumentante que termina em A na saída
    void printarCaminho(int a)
    {

        vector<int> caminho;
        // Loop para inserir os vértices no vetor CAMINHO ao contrário, visto que percorremos o vetor de predecessores do final para o inicio
        // Assim, o vetor CAMINHHO contém o caminho na ordem correta, de V até A
        while (a != -1)
        {
            caminho.insert(caminho.begin(), a);
            a = predecessores[a];
        }

        cout << "A";

        // Percorre o caminho e escreve os vértices dele na saída
        for (int i = 0; i < (int)caminho.size(); i++)
        {
            cout << " " << caminho[i];
        }
        cout << endl;
    }
};

int main(int argv, char **argc)
{

    // Se não forem passados os argumentos necessários o programa é encerrado
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
                // Adiciona o novo vértice no Grafo
                grafo.adicionarVertice(grau);
            }
            else if (palavra == "E")
            {
                // Se a linha começa com E, pega as informações de cada aresta do grafo
                int origem, destino;
                linhaStream >> origem >> destino;
                // Adiciona os vizinhos correspondentes
                grafo.adicionarVizinho(origem, destino);
            }
            else if (palavra == "B")
            {
                // Se a linha começa com B, pega o vertice que delimita a bipartição do grafo
                int particao;
                linhaStream >> particao;
                // Define a partição do Grafo
                grafo.particao = particao;
            }
            else if (palavra == "M")
            {
                // Se a linha começa com M, pega o número de aresta do emparelhamento
                int numArestasEmp;
                linhaStream >> numArestasEmp;
                // Instância um Emparelhamenot com o seu número de arestas
                emp = Emparelhamento(numArestasEmp);
            }
            else if (palavra == "P")
            {
                // Se a linha começa com P, pega as informações de cada aresta do emparelhamento
                int origem, destino;
                linhaStream >> origem >> destino;
                // Adiciona a aresta ao emparelhamento criado previamente
                emp.adicionarAresta(origem, destino);
            }
        }
    }
    arquivoEntrada.close(); // Fecha o arquivo de entrada

    // Define o emparelhamento na classe Grafo
    grafo.emp = emp;
    grafo.defineSaturadosEInsaturados(); // Define os vértices saturados e insaturados
    grafo.printarSaturadosInsaturados(); // Escreve os vértices saturados e insaturados na saída
    grafo.buscarCaminhosMAumentantes();  // Busca e escreve os caminhos M-Aumentantes do Grafo a partir dos vértices insaturados
}
