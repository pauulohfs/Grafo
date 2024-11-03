
#define estrutura_h
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAMNOME 256
#define MAXCIDADES 256 // Número máximo de cidades

typedef struct Vizinhos
{
	char nome[TAMNOME];
	int distancia;
	struct Vizinhos* proximo;
} Vizinhos;

typedef struct Cidade
{
	char nome[TAMNOME];
	Vizinhos* vizinhos;
} Cidade;

typedef struct Grafo
{
	Cidade cidades[MAXCIDADES];
	int numCidades;
} Grafo;

void carregarArquivoGrafo(Grafo* grafo);
void inicializarGrafo(Grafo* grafo);
void adicionarCidade(Grafo* grafo, char* cidadeNome, char* vizinhoNome, int distancia);
void inserirVizinho(Grafo* grafo, char* nomeCidade, char* nomeVizinho);
void removerCidade(Grafo* grafo, char* cidadeNome);
void imprimirCidadeEVizinhos(Grafo* grafo, char* cidadeNome);
int vizinhoExiste(Vizinhos* vizinhos, char* vizinhoNome);
void imprimirGrafo(Grafo* grafo);
int menu();

