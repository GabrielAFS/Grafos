#include <iostream>
#include <fstream>
#include <list>
#include <stack>
#include <algorithm>

using namespace std;

class Grafo{
	int vertice;
	list<int> *adj;
	
	public:	
		int* high;
		Grafo(int v); //Construtor
		void addEdge(int v1, int v2);
		void DFSRecursive(int v, int PE[], int PS[], int pai[]);
		void DFSIterative(int v, int PE[], int PS[], int pai[]);
		void Componentes(int v, int PE[], int PS[], int pai[]);
		void ComponentesBiconexos(int v, int PE[], int PS[], int pai[]);
		void ComponentesFortes(int v, int PE[], int PS[], int comp[]);
};

Grafo::Grafo(int v){
	this->vertice = v;
	adj = new list<int>[v]; //Cria a Adj
	high = new int[v];
}

void Grafo::addEdge(int v1, int v2){
	adj[v1].push_back(v2);
}

int numPE = 1; //Contador de entrada da dfs
int numPS = 1; //Contador de saída da dfs
void Grafo::DFSRecursive(int v, int PE[], int PS[], int pai[]){
	PE[v] = numPE++; //marca vertice
	
	list<int>::iterator w; //Iterador para percorrer a Adj
	for(w = adj[v].begin(); w != adj[v].end(); w++){
		if(PE[*w] == 0){
			cout << "(" << v+1 << "," << *w+1 << ") -> Aresta da Arvore\n";
			pai[*w] = v; //insere o pai de w
			DFSRecursive(*w, PE, PS, pai);
		}
		//Verifica se é aresta de retorno
		else if(PS[*w] == 0 && PE[*w] < PE[v] && pai[v] != *w)
			cout << "(" << v+1 << "," << *w+1 << ") -> Aresta de Retorno\n";
		//Verifica se é aresta de cruzamento
		else if(PS[*w] > 0 && PE[*w] < PE[v])
			cout << "(" << v+1 << "," << *w+1 << ") -> Aresta de Cruzamento\n";
		//Verifica se é aresta de avanço
		else if(PS[*w] > 0 && PE[*w] > PE[v] && pai[*w] != v)
			cout << "(" << v+1 << "," << *w+1 << ") -> Aresta de Avanco\n";
	}
	PS[v] = numPS++; //totalmente explorado
}

void Grafo::DFSIterative(int v, int PE[], int PS[], int pai[]){
	stack<int> pilha;
	int temp;
	
	PE[v] = numPE++; //marca vertice
	pilha.push(v); //empilha o vertice raiz
	
	list<int>::iterator w; //Iterador para percorrer a Adj
	w = adj[v].begin();
	while(!pilha.empty()){
		while(w != adj[v].end()){
			if(PE[*w] == 0){
				cout << "(" << v+1 << "," << *w+1 << ") -> Aresta da Arvore\n";
				pai[*w] = v; //insere o pai de w
				v = *w; //v se torna w
				w = adj[v].begin(); //atualizo w
				PE[v] = numPE++; //marca vertice w
				pilha.push(v); //empilha o vertice w
			}
			//Verifica se é aresta de retorno
			else if(PS[*w] == 0 && PE[*w] < PE[v] && pai[v] != *w){
				cout << "(" << v+1 << "," << *w+1 << ") -> Aresta de Retorno\n";
				*w++;
			}
			//Verifica se é aresta de cruzamento
			else if(PS[*w] > 0 && PE[*w] < PE[v] && pai[*w] != v){
				cout << "(" << v+1 << "," << *w+1 << ") -> Aresta de Cruzamento\n";
				*w++;
			}
			//Verifica se é aresta de avanço
			else if(PS[*w] > 0 && PE[*w] > PE[v] && pai[*w] != v){
				cout << "(" << v+1 << "," << *w+1 << ") -> Aresta de Avanco\n";
				*w++;
			}
			else
				*w++;
		}
		PS[v] = numPS++; //totalmente explorado
		temp = pilha.top(); //o vertice totalmente explorado
		pilha.pop(); //remove o vertice totalmente explorado
		if(!pilha.empty()){
			v = pilha.top();
			/*Procura o ultimo vertice de v explorado e continua
			a percorrer a Adj depois dele*/
			w = find(adj[v].begin(), adj[v].end(), temp);
			*w++;
		}
	}
}

void Grafo::ComponentesBiconexos(int v, int PE[], int PS[], int pai[]){
	PE[v] = numPE++; //marca vertice
	high[v] = PE[v]; //high inicializado
	
	list<int>::iterator w;
	for(w = adj[v].begin(); w != adj[v].end(); w++){
		if(PE[*w] == 0){
			pai[*w] = v; //insere o pai de w
			ComponentesBiconexos(*w, PE, PS, pai);
			//Verifica se existe um ciclo ou não
			//Se o high de w(filho de v) é menor do que v, então não existe aresta que alcance v.
			//Logo, não existe um ciclo e (w,v) é aresta de corte 
			if(high[*w] > high[v])
				cout << "(" << v+1 << "," << *w+1 << ") -> Aresta de Corte\n";
				
			high[v] = min(high[*w], high[v]);
		}
		else if(PS[*w] == 0 && PE[*w] < PE[v] && pai[v] != *w)
			high[v] = min(high[*w], high[v]);
	}
	
	PS[v] = numPS++; //totalmente explorado
}

int compNo = 0;
void Grafo::ComponentesFortes(int v, int PE[], int PS[], int comp[]){
	stack<int> pilha;
	PE[v] = numPE++; //marca vertice
	pilha.push(v); //insere v na pilha
	high[v] = PE[v]; //high inicializado
	
	list<int>::iterator w; //Iterador para percorrer a Adj
	for(w = adj[v].begin(); w != adj[v].end(); w++){
		if(PE[*w] == 0){
			cout << "(" << v+1 << "," << *w+1 << ") -> Aresta da Arvore\n";
			ComponentesFortes(*w, PE, PS, comp);
			//atualiza o high[v]
			high[v] = min(high[*w], high[v]);
		}
		else{
			if(PE[*w] < PE[v] && comp[*w] == 0)
				high[v] = min(high[*w], high[v]);
		}
	}
	PS[v] = numPS++; //totalmente explorado
	//Então v é um vértice forte
	if(high[v] == PE[v]){
		compNo++;
		//marca os vertices do novo componente
		int x;
		do{
			x = pilha.top();
			pilha.pop();
			comp[x] = compNo;
		}while(x != v);
	}
}

int varrePE(int PE[], int n){
	int i = 0;
	
	for(i = 0; i < n; i++)
		if(PE[i] == 0) break;
	
	return i;
}

void Grafo::Componentes(int v, int PE[], int PS[], int pai[]){
	unsigned int componentes = 0, i = v;
	
	do{
		cout << "----------------------------------------\nComponente " << componentes << endl;
		//DFSRecursive(i, PE, PS, pai);
		//DFSIterative(i, PE, PS, pai);
		ComponentesFortes(i, PE, PS, pai);
		i = varrePE(PE, vertice);
		componentes++;
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
	int *PE = new int[numVerts];
	int *PS = new int[numVerts];
	int *pai = new int[numVerts];
	int *comp = new int[numVerts];
	
	for(int i = 0; i < numVerts; i++){
		//Inserindo na Adj
		file1 >> numAdj;
		PE[i] = 0;
		PS[i] = 0;
		comp[i] = 0;
		for(int j = 0; j < numAdj; j++){
			file1 >> k;
			G.addEdge(i, k-1);
		}
	}
	
	//pai[0] = 0; //pai da raiz é ela mesma
	//G.DFSRecursive(0, PE, PS, pai);
	//G.DFSIterative(0, PE, PS, pai);
	//G.Componentes(2, PE, PS, pai);
	//G.ComponentesBiconexos(0, PE, PS, pai);
	G.Componentes(0, PE, PS, comp);
	
	for(int i = 0; i < numVerts; i++){
		file2 << "PE[" << i << "] = " << PE[i];
		file2 << " | PS[" << i << "] = " << PS[i];
		file2 << " | High[" << i << "] = " << G.high[i] << endl; 
	}
	
	file1.close();
	file2.close();
	
	return 0;
}
