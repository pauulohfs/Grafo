#include "estrutura.h"

int main()
{
	Grafo grafo;
	inicializarGrafo(&grafo);
	carregarArquivoGrafo(&grafo);

	int opcao;
	char cidade[TAMNOME], vizinho[TAMNOME];
	int distancia;
	char nomeArquivo[100];

	do
	{
		opcao = menu();
		switch (opcao)
		{
		case 1:
			printf("========================================================\n");
			printf("Inserir cidade e vizinho.\n");
			printf("Nome da cidade: ");
			scanf(" %[^\n]", cidade); // ler ate o fim da linha incluindo espacos
			printf("Nome do vizinho: ");
			scanf(" %[^\n]", vizinho);
			printf("Distancia: ");
			scanf("%d", &distancia);
			adicionarCidade(&grafo, cidade, vizinho, distancia);
			printf("Cidade e vizinho adicionados com sucesso!\n");
			break;

		case 2:
			printf("========================================================\n");
			printf("Remover cidade.\n");
			printf("Nome da cidade a remover: ");
			scanf(" %[^\n]", cidade);
			removerCidade(&grafo, cidade);
			break;

		case 3:
			printf("========================================================\n");
			printf("Imprimir Cidade e Vizinhos.\n");
			printf("Nome da cidade: ");
			scanf(" %[^\n]", cidade); 
			imprimirCidadeEVizinhos(&grafo, cidade);
			break;

		case 4:
			printf("Imprimir todo o grafo.\n");
			imprimirGrafo(&grafo);
			break;

		case 0:
			printf("Saindo...\n");
			break;

		default:
			printf("Opçao invlida! Tente novamente.\n");
		}
	}
	while (opcao != 0);

	return 0;
}
void inicializarGrafo(Grafo* grafo)
{
	grafo->numCidades = 0;
}

int vizinhoExiste(Vizinhos* vizinhos, char* vizinhoNome)
{
	while (vizinhos != NULL)
	{
		if (strcmp(vizinhos->nome, vizinhoNome) == 0)
		{
			return 1; // vizinho ja existe
		}
		vizinhos = vizinhos->proximo;
	}
	return 0; // vizinho nao existe
}


void adicionarCidade(Grafo* grafo, char* cidadeNome, char* vizinhoNome, int distancia)
{
	int i, j;

	// se a cidade ja existe
	for (i = 0; i < grafo->numCidades; i++)
	{
		if (strcmp(grafo->cidades[i].nome, cidadeNome) == 0)
		{
			break;
		}
	}

	// se nao existir adiciona no grafo
	if (i == grafo->numCidades)
	{
		strcpy(grafo->cidades[grafo->numCidades].nome, cidadeNome);
		grafo->cidades[grafo->numCidades].vizinhos = NULL;
		grafo->numCidades++;
	}

	// ve se o vizinho ja existe na lista de vizinhos da cidade
	if (!vizinhoExiste(grafo->cidades[i].vizinhos, vizinhoNome))
	{
		// adiciona o vizinho na cidade
		Vizinhos* novoVizinho = (Vizinhos*)malloc(sizeof(Vizinhos));
		strcpy(novoVizinho->nome, vizinhoNome);
		novoVizinho->distancia = distancia;
		novoVizinho->proximo = grafo->cidades[i].vizinhos;
		grafo->cidades[i].vizinhos = novoVizinho;
	}

	// ve se o vizinho ja existe na lista de cidades
	for (j = 0; j < grafo->numCidades; j++)
	{
		if (strcmp(grafo->cidades[j].nome, vizinhoNome) == 0)
		{
			// verifica se a cidade ja existe na lista de vizinhos do vizinho
			if (!vizinhoExiste(grafo->cidades[j].vizinhos, cidadeNome))
			{
				// o mais importante, adiciona a cidade atual como vizinho do vizinho
				Vizinhos* novoVizinhoDoVizinho = (Vizinhos*)malloc(sizeof(Vizinhos));
				strcpy(novoVizinhoDoVizinho->nome, cidadeNome);
				novoVizinhoDoVizinho->distancia = distancia; 
				novoVizinhoDoVizinho->proximo = grafo->cidades[j].vizinhos;
				grafo->cidades[j].vizinhos = novoVizinhoDoVizinho;
			}
			break;
		}
	}
}




void carregarArquivoGrafo(Grafo* grafo)
{
	FILE* arquivo = fopen("cidades.txt", "r");
	if (arquivo == NULL)
	{
		printf("Erro ao abrir o arquivo.\n");
		return;
	}

	char cidade1[TAMNOME], cidade2[TAMNOME];
	int distancia;
	char linha[512];

	while (fgets(linha, sizeof(linha), arquivo))
	{
		linha[strcspn(linha, "\n")] = '\0';

		// ignora linhas que contem apenas ":"
		if (strcmp(linha, ":") == 0)
		{
			continue;
		}

		// a primeira cidade 
		strcpy(cidade1, linha);

		// ver os vizinhos da cidade ate encontrar o proximo ":"
		while (fgets(linha, sizeof(linha), arquivo) && strcmp(linha, ":\n") != 0 && strcmp(linha, ":\r\n") != 0)
		{
			linha[strcspn(linha, "\n")] = '\0';

			// pega a cidade vizinha
			strcpy(cidade2, linha);

			// pega a distancia da a cidade vizinha
			if (fgets(linha, sizeof(linha), arquivo))
			{
				linha[strcspn(linha, "\n")] = '\0';

				// converte a linha em um int
				distancia = atoi(linha);

				// Adiciona a cidade e seu vizinho ao grafo com a distância
				adicionarCidade(grafo, cidade1, cidade2, distancia);
			}
		}
	}

	fclose(arquivo);
}

void imprimirCidadeEVizinhos(Grafo* grafo, char* cidadeNome)
{
	for (int i = 0; i < grafo->numCidades; i++)
	{
		if (strcmp(grafo->cidades[i].nome, cidadeNome) == 0)
		{
			printf("===================================================================\n");
			printf("Cidade: %s\n", grafo->cidades[i].nome);
			Vizinhos* vizinho = grafo->cidades[i].vizinhos;
			while (vizinho != NULL)
			{

				printf("  Vizinhos: %s, Distancia: %d\n", vizinho->nome, vizinho->distancia);
				vizinho = vizinho->proximo;
			}
			return;
		}
	}
	printf("Cidade nao encontrada.\n");

}

void imprimirGrafo(Grafo* grafo)
{
	for (int i = 0; i < grafo->numCidades; i++)
	{
		printf("Cidade: %s\n", grafo->cidades[i].nome);
		Vizinhos* vizinho = grafo->cidades[i].vizinhos;
		while (vizinho != NULL)
		{
			printf("  Vizinhos: %s, Distancia: %d\n", vizinho->nome, vizinho->distancia);
			vizinho = vizinho->proximo;
		}
		printf("===================================================================\n");
	}
}

void removerCidade(Grafo* grafo, char* cidadeNome) {
    int i, j;

    // encontra a cidade a ser removida
    for (i = 0; i < grafo->numCidades; i++) {
        if (strcmp(grafo->cidades[i].nome, cidadeNome) == 0) {
            // remove as referencias da listas de vizinhos
            for (j = 0; j < grafo->numCidades; j++) {
                if (j != i) { // nao precisa verificar a propria cidade
                    Vizinhos** atual = &grafo->cidades[j].vizinhos;
                    while (*atual) {
                        if (strcmp((*atual)->nome, cidadeNome) == 0) {
                            Vizinhos* temp = *atual;
                            *atual = (*atual)->proximo; // remove o vizinho
                            free(temp);
                        } else {
                            atual = &(*atual)->proximo; // vai para o proximo vizinho
                        }
                    }
                }
            }

            // remove a cidade deslocando as restantes
            for (j = i; j < grafo->numCidades - 1; j++) {
                grafo->cidades[j] = grafo->cidades[j + 1];
            }
            grafo->numCidades--; // decrementa o número de cidades
            printf("Cidade removida com Sucesso.\n");
            return; 
        }
    }
    printf("Cidade %s nao encontrada no grafo.\n", cidadeNome);
}



int menu()
{
	int opcao;
	printf("\n\n===============Menu:======================\n\n");
	printf("1. Inserir cidade e vizinhos\n");
	printf("2. Remover cidade\n");
	printf("3. Imprimir Cidade e Vizinhos\n");
	printf("4. Imprimir todo o grafo\n");
	printf("0. Sair\n");
	printf("Escolha uma opcao: ");
	scanf("%d", &opcao);
	return opcao;
}

