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
        int particao;
        vector<Vertice> listaAdjacencia;
        vector<int> verticesSaturados;
        vector<int> verticesInsaturados;
        vector<int>predecessores;
        vector<bool>visitados;

        Grafo(int numVertices, int numArestas): numVertices(numVertices), numArestas(numArestas),listaAdjacencia(numVertices), verticesSaturados(numVertices), verticesInsaturados(numVertices),
         predecessores(numVertices), visitados(numVertices){}
        Grafo(int particao): particao(particao){}
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
        Emparelhamento(){}
        
        void adicionarAresta(int origem, int destino){
            arestas.push_back(Aresta(origem,destino));
        }


};

int encontraVerticeEmparelhado(Emparelhamento *emp,Grafo *grafo, int v){
    for(int i=0; i<grafo->listaAdjacencia[v].grau;i++){
        if(verificaSaturacao(emp,grafo->listaAdjacencia[v].vizinhos[i])){
            return grafo->listaAdjacencia[v].vizinhos[i];
            
        }
    }

}

int verificaSaturacao(Emparelhamento *emp, int v){
    for(int i=0; i<emp->numArestas;i++){
        if(emp->arestas[i].destino==v || emp->arestas[i].origem==v) 
            return 1;
        else
            return 0;    
    }
        

}
void listaSaturadosNaoSaturados(Grafo *grafo, Emparelhamento *emp){
    for(int i=0; i<grafo->numVertices;i++){

        if(verificaSaturacao(emp,i)){
            grafo->verticesSaturados.push_back(i);   
        }
        else{
            grafo->verticesInsaturados.push_back(i);
        }

    }
}
/*
busca(G, v, M):
  Marque todos os vértices como não visitados
  Marque o predecessor de todos os vértices como inválido
  retorne busca-rec(G, v, M)

busca-rec(G, v, M):
Se v não visitado:
  Para cada vizinho u de v em ordem crescente:
    Se u não foi visitado:
      marque u como visitado
      marque o predecessor de u como v
      Se u é saturado:
        retorne u
      Senão
        encontre w o vértice emparelhado com u em M
        x = busca-rec(G,w,M)
    se x válido
      retorne x
  Retorne inválido
*/
int buscaRecurciva(Grafo*grafo, Emparelhamento *emp, int v){
    int x,w;
    if(!grafo->visitados[v]){

        for(int i=0; i< grafo->listaAdjacencia[v].grau;i++){            
            int u=grafo->listaAdjacencia[v].vizinhos[i];

            if(!grafo->visitados[u]){
                grafo->visitados[u]=true;
                grafo->predecessores[u]=1;
                if(verificaSaturacao(emp,u)){
                    return u;
                }else {
                    w=encontraVerticeEmparelhado(emp,grafo,u);
                    x =buscaRecurciva(grafo, emp,w);
                }
                if (grafo->predecessores[x]==1){
                    return x;
                }
          }
          return -1;
        }
    }
}

int busca(Grafo *grafo, Emparelhamento *emp, int v){

    for(int i=0; i<grafo->numVertices;i++){
        grafo->visitados[i]=false;
        grafo->predecessores[i]=-1;

    }
    return buscaRecurciva(grafo, emp,v);
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

    Grafo grafo;
    Emparelhamento emp;

    // Percorre linha por linha do arquivo de entrada
    while(getline(arquivoEntrada, linha)){
        istringstream linhaStream(linha);
        string palavra;
        
        // Percorre cada palavra em cada linha do arquivo de entrada
        while(linhaStream >> palavra){
            if(palavra == "G"){
                // Se a linha começa com G, pega o numero de vertices e arestas do grafo
                int numVertices,numArestas;
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
            else if(palavra == "B"){
                // Se a linha começa com B, pega o vertice que delimita a bipartição do grafo
                int particao;
                linhaStream >> particao;
                grafo=Grafo(particao);
                
            }
            else if(palavra == "M"){
                // Se a linha começa com M , pega o numero de aresta do emparelhamento
                int numArestasEmp;
                linhaStream >> numArestasEmp;
                emp=Emparelhamento(numArestasEmp);
                
            }
            else if(palavra == "P"){
                //se a linha começa com P, pega as informações de cada aresta do emparelhamento    
                int origem,destino;
                emp.adicionarAresta(origem,destino);

            }
        }
       
    }
    arquivoEntrada.close();
    //criar o arquivo de saida 
    listaSaturadosNaoSaturados(&grafo,&emp);

    for(int i=0;i<grafo.verticesSaturados.size();i++)

        std::cout << " " << grafo.verticesSaturados[i] << std::endl;
    
    for(int i=0;i<grafo.verticesInsaturados.size();i++)
        std::cout << " " << grafo.verticesInsaturados[i] << std::endl;
    
    

}
    
