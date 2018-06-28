#include <stdio.h>
#include <stdlib.h>

//Digrafo aciclico
typedef struct Digrado{
	int vertice;
	int qtdAdj;
}Digrado;

typedef struct Elem{
	struct Digrado* pont;
	struct Digrado dig;
}Elem;

typedef Elem* Cabeca;

//Cria a cabeca da lista
Cabeca* initLista(){
	Cabeca* pont = (Cabeca*)malloc(sizeof(Cabeca));
	*pont = NULL;
	
	return pont;
}

//Insere um novo nó na lista
void insertLista(Cabeca* lista, Digrado did){
	Elem* no = (Elem*)malloc(sizeof(Elem));
	no->dig = did;
	no->pont = NULL;
	//Caso lista vazia
	if(*lista == NULL){
		*lista = no;
	}
	//Caso tenha mais de um elemento
	else{
		Elem* bro = *lista;
		while(bro->pont != NULL){
			bro = bro->pont;
		}
		bro->pont = no;
	}
}

//Calcula o grau de entrada de cada vertice
unsigned int calculaGrau(Digrado* vet, unsigned int grau, Cabeca** lista, int id, int n){
	int i = 0;
	
	//Percorre as n posições + as m posições da lista
	while(i < n){
		if(i != id && *lista[i] != NULL){
			Elem* bro = *lista[i]; 
			while(bro != NULL){
				if(vet[id].vertice == bro->dig.vertice){
					grau++;
					break;
				}
				bro = bro->pont;
			}
		}
		i++;
	}
	
	return grau;
}

//Imprime os vertices ordenados
void print(Cabeca** lista, Digrado* did, unsigned int* graus, int n){
	int i = 0, j = 0;
	int* vertis;
	
	//Procura o vertice com grau 0 e remove as arestas que divergem dele
	while(i < n){
		if(graus[i] < n && graus[i] == 0 && *lista[i] != NULL){
			Elem* bro = *lista[i];
			vertis = (int*)malloc(did[i].qtdAdj * sizeof(int));
			while(bro != NULL){
				vertis[j] = bro->dig.vertice;
				if(graus[vertis[j]-1] != 0)
					graus[vertis[j]-1]--;
					
				bro = bro->pont;
				j++;
			}
			printf("Vertice %d diverge em ", did[i].vertice);
			for(j = 0; j < did[i].qtdAdj; j++)
				printf("%d ", vertis[j]);
				
			printf("\n");
			*lista[i] = NULL;
			graus[i] = n; //porque um Digrado não pode ter mais de n-1 arestas
			break;
		}
		else if(graus[i] < n && graus[i] == 0 && *lista[i] == NULL){
			printf("Vertice %d diverge em ninguem.\n", did[i].vertice);
			*lista[i] = NULL;
			graus[i] = n;
			break;
		}
		i++;
	}
	free(vertis);
}

int main(int argc, char* argv[]){
	unsigned int* graus;
	unsigned int i, tam, j;
	Cabeca** vetList;
	Digrado did, *didico;
	FILE* file1 = fopen(argv[1], "r");
	
	fscanf(file1, "%d ", &tam);
	
	graus = (unsigned int*)calloc(tam, sizeof(unsigned int));
	vetList = (Cabeca**)malloc(tam * sizeof(Cabeca*));
	didico = (Digrado*)malloc(tam * sizeof(Digrado));
	
	for(i = 0; i < tam; i++){
		vetList[i] = initLista();
		didico[i].vertice = i+1;
		fscanf(file1, "%d ", &didico[i].qtdAdj);
		for(j = 0; j < didico[i].qtdAdj; j++){
			fscanf(file1, "%d ", &did.vertice);
			//printf("%d \n", did.vertice);
			insertLista(vetList[i], did);
		}
	}
	
	for(i = 0; i < tam; i++){
		graus[i] = calculaGrau(didico, graus[i], vetList, i, tam);
	}
	
	for(i = 0; i < tam; i++)
		print(vetList, didico, graus, tam);
	
	fclose(file1);
	free(didico);
	free(vetList);
	
	return 0;
}
