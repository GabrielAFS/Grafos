#include <iostream>
#include <fstream>
#include <list>
#include <queue>
#include <algorithm>

using namespace std;

class Grafo{
	int vertice;
	list<int> *adj;
	int* nivel;
	
	public:	
		Grafo(int v); //Construtor
		void addEdge(int v1, int v2);
		void BFS(int v, int largura[], int pai[], int PS[]);
		void Componentes(int v, int largura[], int pai[], int PS[]);
};

Grafo::Grafo(int v){
	this->vertice = v;
	adj = new list<int>[v]; //Cria a Adj
	nivel = new int[v];
}

void Grafo::addEdge(int v1, int v2){
	adj[v1].push_back(v2);
}

int numL = 1; //contador de largura
int numPS = 1; //contador de saída
void Grafo::BFS(int v, int largura[], int pai[], int PS[]){
	queue<int> fifo;
	largura[v] = numL++; //marca v
	nivel[v] = 0; //raiz tem nível 0
	pai[v] = -1; //raiz é o pai de todos. Logo, não tem pai
	
	fifo.push(v); //insere na fila
	list<int>::iterator w; //iterador para percorrer a Adj
	while(!fifo.empty()){
		v = fifo.front();
		fifo.pop(); //desemfileira
		for(w = adj[v].begin(); w != adj[v].end(); w++){
			if(largura[*w] == 0){
				cout << "(" << v+1 << "," << *w+1 << ") -> Aresta Pai\n";
				pai[*w] = v; //insere o pai de w
				largura[*w] = numL++; //marca w
				nivel[*w] = nivel[v] + 1; //nivel de w é no máximo nivel[v]+1 
				fifo.push(*w); //emfileira
			}
			//Verifica se é aresta irmão
			else if(pai[*w] == pai[v] && PS[*w] == 0)
				cout << "(" << v+1 << "," << *w+1 << ") -> Aresta Irmao\n";
			//Verifica se é aresta tio
			else if(pai[*w] != v && nivel[*w] == nivel[v]+1)
				cout << "(" << v+1 << "," << *w+1 << ") -> Aresta Tio\n";
			//Verifica se é aresta primo
			else if(pai[*w] != pai[v] && nivel[*w] == nivel[v] && PS[*w] == 0)
				cout << "(" << v+1 << "," << *w+1 << ") -> Aresta Primo\n";
		}
		PS[v] = numPS++; //totalmente explorado
	}
}

int varreLarg(int largura[], int n){
	int i = 0;
	
	for(i = 0; i < n; i++)
		if(largura[i] == 0) break;
	
	return i;
}

void Grafo::Componentes(int v, int largura[], int pai[], int PS[]){
	int comp = 0, i = v;
	
	do{
		cout << "----------------------------------------\nComponente " << comp << endl;
		BFS(i, largura, pai, PS);
		i = varreLarg(largura, vertice);
		comp++;
	}while(i < vertice);
}

int main(int argc, char* argv[]){
	int numVerts = 0, numAdj, k;
	//Abrindo arquivo
	ifstream file1;
	ofstream file2;
	file1.open(argv[1]);
	file2.open(argv[2]);
	
	//Inserindo qtdVertices
	file1 >> numVerts;
	Grafo G(numVerts);
	int *largura = new int[numVerts];
	int *pai = new int[numVerts];
	int *PS = new int[numVerts];
	
	for(int i = 0; i < numVerts; i++){
		//Inserindo na Adj
		file1 >> numAdj;
		largura[i] = 0;
		PS[i] = 0;
		for(int j = 0; j < numAdj; j++){
			file1 >> k;
			G.addEdge(i, k-1);
		}
	}
	
	//G.BFS(0, largura, pai, PS);
	G.Componentes(0, largura, pai, PS);
	
	for(int i = 0; i < numVerts; i++){
		file2 << "largura[" << i << "] = " << largura[i] << endl;
	}
	
	file1.close();
	file2.close();
	
	return 0;
}
