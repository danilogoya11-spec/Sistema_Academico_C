#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX 50
#define ARQUIVO "alunos.dat"
#define ARQUIVO_BACKUP "alunos_corrompido.dat"
#define MEDIA_MINIMA 6.0

//Situacao de matricula do aluno (campo status)
#define ATIVO 1
#define TRANCADO 2
#define TRANSFERIDO 3
#define FORMADO 4

//Estruturas
typedef struct {
	float Ac1;
	float Ac2;
	float Af;
}Avaliacoes;

typedef struct{
	Avaliacoes mat;
	Avaliacoes port;
	Avaliacoes hist;
	Avaliacoes cien;
	Avaliacoes geo;
}Disciplina;

typedef struct {
	char nome[MAX];
	int codigo;
	int status;
	Disciplina materia;
}Aluno;

//Funcoes Prototipo
void aloca(Aluno **p, int n);
void cadastro(Aluno *p, Aluno *vetor, int n);
void listar(Aluno *p);
void adicionar(Aluno **p, int *n);
void remover(Aluno **p, int *n);
int buscarIndice(Aluno *p, int n, int cod);
int codigoRepetido(Aluno *vetor, int n, Aluno *atual, int cod);
void alterarStatus(Aluno *p);
const char *textoStatus(int status);
float media(Avaliacoes *a);
void gerarRelatorio(Aluno *p, int n);

//Funcoes de leitura com validacao
void limparBuffer(void);
int lerInteiro(const char *msg);
float lerNota(const char *msg);
void lerAvaliacoes(Avaliacoes *a, const char *disciplina);

//Funcoes arquivos binarios
void salvar(Aluno *p, int n);
int salvarRegistro(Aluno *p, int pos);
void carregar(Aluno **p, int *n);

//Codigo
int main(){
	Aluno*  estudante = NULL;
	int quantidade = 0;
	int cod, opt1 = 0, opt2, i, pos, total, statusAnterior;
	carregar(&estudante, &quantidade);
	do{
		system("cls");
		printf("\t-- Sistema Academico --\n\n");
		printf("[1] Cadastrar\n");
		printf("[2] Buscar\n");
		printf("[3] Atualizar (dados, novos alunos ou status)\n");
		printf("[4] Remover\n");
		printf("[5] Listar (alunos ou relatorio da turma)\n");
		printf("[6] Sair\n");
		opt1 = lerInteiro("Opcao: ");
		switch(opt1){
			case 1:
				system("cls");
				printf("\t-- Cadastro --\n\n");
				if (quantidade > 0) {
					printf("Ja existem alunos cadastrados!\n");
					printf("Use a opcao Atualizar -> Adicionar novos alunos.\n");
					system("pause");
					break;
				}
				total = lerInteiro("Quantidade de Estudantes: ");
				if (total <= 0) {
					printf("Quantidade invalida! Informe um numero maior que zero.\n");
					system("pause");
					break;
				}
				aloca(&estudante, total);
				for(i = 0 ; i < total ; i++){
					//so compara com os alunos ja cadastrados (0 ate i-1)
					cadastro(&estudante[i], estudante, i);
				}
				quantidade = total;
				salvar(estudante, quantidade);
				system("pause");
			break;
			case 2:
				system("cls");
				printf("\t-- Buscar --\n\n");
				if(quantidade == 0){
					printf("Realize o cadastro primeiro!!\n");
					system("pause");
					break;
				}
				cod = lerInteiro("Codigo do Aluno: ");
				pos = buscarIndice(estudante, quantidade, cod);
				if(pos == -1)
					printf("Aluno nao encontrado!!\n");
				else
					listar(&estudante[pos]);
				system("pause");
			break;
			case 3:
				system("cls");
				printf("\t-- Atualizar --\n\n");
				if(quantidade == 0){
					printf("Realize o cadastro primeiro!!\n");
					system("pause");
					break;
				}
				printf("[1] Atualizar dados de um aluno\n");
				printf("[2] Adicionar novos alunos\n");
				printf("[3] Alterar status de um aluno\n");
				opt2 = lerInteiro("Opcao: ");
				switch(opt2){
					case 1:
						cod = lerInteiro("Digite o codigo do aluno: ");
						pos = buscarIndice(estudante, quantidade, cod);
						if(pos == -1){
							printf("Aluno Nao Encontrado!!\n");
							break;
						}
						printf("Aluno Encontrado!\nAtualize as informacoes\n");
						statusAnterior = estudante[pos].status;	//o status so muda pela opcao 3
						cadastro(&estudante[pos], estudante, quantidade);
						estudante[pos].status = statusAnterior;
						if(!salvarRegistro(&estudante[pos], pos))
							salvar(estudante, quantidade);
					break;
					case 2:
						adicionar(&estudante, &quantidade);
						salvar(estudante, quantidade);
					break;
					case 3:
						cod = lerInteiro("Digite o codigo do aluno: ");
						pos = buscarIndice(estudante, quantidade, cod);
						if(pos == -1){
							printf("Aluno Nao Encontrado!!\n");
							break;
						}
						alterarStatus(&estudante[pos]);
						//grava so este aluno no arquivo, direto na posicao dele
						if(!salvarRegistro(&estudante[pos], pos))
							salvar(estudante, quantidade);
					break;
					default:
						printf("Opcao invalida!\n");
					break;
				}
				system("pause");
			break;
			case 4:
				system("cls");
				printf("\t-- Remover --\n\n");
				if(quantidade == 0){
					printf("Realize o cadastro primeiro!!\n");
					system("pause");
					break;
				}
				remover(&estudante, &quantidade);
				salvar(estudante, quantidade);
				system("pause");
			break;
			case 5:
				system("cls");
				printf("\t-- Listar --\n\n");
				if(quantidade == 0){
					printf("Cadastro ainda nao foi realizado!!\n");
					system("pause");
					break;
				}
				printf("[1] Listar todos os alunos\n");
				printf("[2] Gerar relatorio da turma\n");
				opt2 = lerInteiro("Opcao: ");
				printf("\n");
				if(opt2 == 1){
					for(i = 0; i < quantidade; i++){
						listar(&estudante[i]);
					}
				}
				else if(opt2 == 2){
					gerarRelatorio(estudante, quantidade);
				}
				else{
					printf("Opcao invalida!\n");
				}
				system("pause");
			break;
			case 6:
				salvar(estudante, quantidade);
				free(estudante);
				estudante = NULL;
				printf("Finalizando Sistema...\n");
			break;
			default:
				system("cls");
				printf("Codigo Invalido\n");
				system("pause");
			break;
		}
	}while(opt1 != 6);
	return 0;
}

//Funcoes de leitura com validacao

//descarta tudo o que sobrou na linha digitada (inclusive o '\n')
void limparBuffer(void){
	int c;
	while((c = getchar()) != '\n' && c != EOF);
}

//repete a pergunta ate o usuario digitar um numero inteiro
int lerInteiro(const char *msg){
	int valor;
	printf("%s", msg);
	while(scanf("%d", &valor) != 1){
		if(feof(stdin)) exit(0);	//fim da entrada: encerra sem travar
		limparBuffer();
		printf("Entrada invalida! Digite um numero inteiro: ");
	}
	limparBuffer();
	return valor;
}

//repete a pergunta ate o usuario digitar uma nota entre 0 e 10
float lerNota(const char *msg){
	float valor;
	printf("%s", msg);
	while(scanf("%f", &valor) != 1 || valor < 0 || valor > 10){
		if(feof(stdin)) exit(0);
		limparBuffer();
		printf("Nota invalida! Digite um valor entre 0 e 10: ");
	}
	limparBuffer();
	return valor;
}

//le as tres notas de uma disciplina direto na struct apontada por 'a'
void lerAvaliacoes(Avaliacoes *a, const char *disciplina){
	printf("%s:\n", disciplina);
	a->Ac1 = lerNota("- Ac1: ");
	a->Ac2 = lerNota("- Ac2: ");
	a->Af  = lerNota("- Af: ");
}

//Funcoes Definicoes
void aloca(Aluno **p, int n){
	*p = (Aluno*)malloc(n*sizeof(Aluno));
	if(*p == NULL){
		printf("Erro de Alocacao!!\n");
		exit(1);
	}
}

//retorna a posicao do aluno com o codigo informado, ou -1 se nao existir
int buscarIndice(Aluno *p, int n, int cod){
	int i;
	for(i = 0; i < n; i++){
		if(p[i].codigo == cod)
			return i;
	}
	return -1;
}

//procura o codigo entre os 'n' alunos do vetor, ignorando o proprio aluno 'atual'
//(assim, na atualizacao, o aluno pode manter o mesmo codigo); retorna a posicao ou -1
int codigoRepetido(Aluno *vetor, int n, Aluno *atual, int cod){
	int i;
	for(i = 0; i < n; i++){
		if(&vetor[i] != atual && vetor[i].codigo == cod)
			return i;
	}
	return -1;
}

//preenche o aluno apontado por 'p'; 'vetor' e 'n' servem para impedir codigo repetido
void cadastro(Aluno *p, Aluno *vetor, int n){
	int cod, pos;
	memset(p, 0, sizeof(Aluno));	//zera o registro: nenhum lixo de memoria vai para o arquivo
	printf("\nNome do Aluno: ");
	if(scanf(" %49[^\n]", p->nome) != 1) exit(0);
	limparBuffer();	//descarta o excesso se o nome tiver mais de 49 caracteres
	do{
		cod = lerInteiro("Codigo do Aluno: ");
		pos = codigoRepetido(vetor, n, p, cod);
		if(pos != -1)
			printf("Codigo ja cadastrado para %s! Informe outro.\n", vetor[pos].nome);
	}while(pos != -1);
	p->codigo = cod;
	p->status = ATIVO;	//todo aluno novo comeca ativo
	lerAvaliacoes(&p->materia.port, "Portugues");
	lerAvaliacoes(&p->materia.mat, "Matematica");
	lerAvaliacoes(&p->materia.cien, "Ciencia");
	lerAvaliacoes(&p->materia.hist, "Historia");
	lerAvaliacoes(&p->materia.geo, "Geografia");
}

void listar(Aluno *p){
	printf("Nome do Aluno: %s\n", p->nome);
	printf("Codigo do Aluno: %d\n", p->codigo);
	printf("Status: %s\n", textoStatus(p->status));
	printf("Portugues:\n");
	printf("- Ac1: %.2f - Ac2: %.2f - Af: %.2f\n", p->materia.port.Ac1, p->materia.port.Ac2, p->materia.port.Af);
	printf("Matematica:\n");
	printf("- Ac1: %.2f - Ac2: %.2f - Af: %.2f\n", p->materia.mat.Ac1, p->materia.mat.Ac2, p->materia.mat.Af);
	printf("Ciencia:\n");
	printf("- Ac1: %.2f - Ac2: %.2f - Af: %.2f\n", p->materia.cien.Ac1, p->materia.cien.Ac2, p->materia.cien.Af);
	printf("Historia:\n");
	printf("- Ac1: %.2f - Ac2: %.2f - Af: %.2f\n", p->materia.hist.Ac1, p->materia.hist.Ac2, p->materia.hist.Af);
	printf("Geografia:\n");
	printf("- Ac1: %.2f - Ac2: %.2f - Af: %.2f\n\n", p->materia.geo.Ac1, p->materia.geo.Ac2, p->materia.geo.Af);
}

void adicionar(Aluno** p, int* n){
	int i, antigos = *n;
	int novos;
	Aluno* temp;
	novos = lerInteiro("Quantidade de novos alunos: ");
	if (novos <= 0) {
		printf("Quantidade invalida!\n");
		return;
	}
	//a quantidade so e atualizada depois que o realloc der certo
	temp = (Aluno*) realloc(*p, (antigos + novos) * sizeof(Aluno));
	if (temp == NULL) {
		printf("Erro de alocacao!! Nenhum aluno foi adicionado.\n");
		return;	//o vetor antigo continua valido
	}
	*p = temp;
	for (i = antigos; i < antigos + novos; i++) {
		//compara com todos os alunos anteriores, inclusive os novos ja digitados
		cadastro(&(*p)[i], *p, i);
	}
	*n = antigos + novos;
}

void remover(Aluno** p, int* n){
	int j, cod, pos;
	Aluno* temp;
	cod = lerInteiro("Digite o codigo do aluno: ");
	pos = buscarIndice(*p, *n, cod);
	if(pos == -1){
		printf("Aluno nao encontrado ou nao cadastrado!!\n");
		return;
	}
	printf("Aluno %s removido!\n", (*p)[pos].nome);
	//desloca os alunos seguintes uma posicao para tras
	for (j = pos; j < *n - 1; j++) {
		(*p)[j] = (*p)[j + 1];
	}
	*n = *n - 1;
	if (*n == 0) {
		free(*p);
		*p = NULL;
	}
	else {
		temp = (Aluno*)realloc(*p, (*n) * sizeof(Aluno));
		if (temp != NULL) {
			*p = temp;
		}
	}
}

//devolve o nome do status para exibir na tela
const char *textoStatus(int status){
	switch(status){
		case ATIVO: return "Ativo";
		case TRANCADO: return "Trancado";
		case TRANSFERIDO: return "Transferido";
		case FORMADO: return "Formado";
		default: return "Desconhecido";
	}
}

//muda a situacao de matricula do aluno apontado por 'p'
void alterarStatus(Aluno *p){
	int novo;
	printf("\nAluno: %s\n", p->nome);
	printf("Status atual: %s\n\n", textoStatus(p->status));
	printf("[%d] Ativo\n[%d] Trancado\n[%d] Transferido\n[%d] Formado\n", ATIVO, TRANCADO, TRANSFERIDO, FORMADO);
	novo = lerInteiro("Novo status: ");
	while(novo < ATIVO || novo > FORMADO){
		novo = lerInteiro("Status invalido! Escolha de 1 a 4: ");
	}
	p->status = novo;
	printf("Status alterado para %s.\n", textoStatus(p->status));
}

//media simples das tres avaliacoes de uma disciplina
float media(Avaliacoes *a){
	return (a->Ac1 + a->Ac2 + a->Af) / 3;
}

//mostra a media de cada aluno em cada disciplina, a situacao e o resumo da turma
void gerarRelatorio(Aluno *p, int n){
	int i, k, reprovacoes, aprovados = 0, melhor = 0;
	int porStatus[5] = {0, 0, 0, 0, 0};
	float m[5], geral, maiorGeral = -1, somaTurma[5] = {0, 0, 0, 0, 0};
	const char *nomes[5] = {"Port", "Mat", "Cien", "Hist", "Geo"};

	printf("=========================== RELATORIO DA TURMA ===========================\n");
	printf("Criterio: media = (Ac1 + Ac2 + Af) / 3; aprovado com media >= %.1f em todas\n\n", MEDIA_MINIMA);
	printf("%-6s %-20s %-11s", "Codigo", "Nome", "Status");
	for(k = 0; k < 5; k++) printf(" %5s", nomes[k]);
	printf("  %5s  %s\n", "Geral", "Situacao");

	for(i = 0; i < n; i++){
		m[0] = media(&p[i].materia.port);
		m[1] = media(&p[i].materia.mat);
		m[2] = media(&p[i].materia.cien);
		m[3] = media(&p[i].materia.hist);
		m[4] = media(&p[i].materia.geo);
		geral = 0;
		reprovacoes = 0;
		for(k = 0; k < 5; k++){
			geral += m[k];
			somaTurma[k] += m[k];
			if(m[k] < MEDIA_MINIMA) reprovacoes++;
		}
		geral /= 5;
		if(geral > maiorGeral){
			maiorGeral = geral;
			melhor = i;
		}
		if(reprovacoes == 0) aprovados++;
		if(p[i].status >= ATIVO && p[i].status <= FORMADO) porStatus[p[i].status]++;

		printf("%-6d %-20.20s %-11s", p[i].codigo, p[i].nome, textoStatus(p[i].status));
		for(k = 0; k < 5; k++) printf(" %5.2f", m[k]);
		if(reprovacoes == 0)
			printf("  %5.2f  Aprovado\n", geral);
		else
			printf("  %5.2f  Reprovado em %d\n", geral, reprovacoes);
	}

	printf("\n%-39s", "Media da turma");
	for(k = 0; k < 5; k++) printf(" %5.2f", somaTurma[k] / n);
	printf("\n\nTotal de alunos: %d\n", n);
	printf("Aprovados: %d   Reprovados: %d\n", aprovados, n - aprovados);
	printf("Ativos: %d   Trancados: %d   Transferidos: %d   Formados: %d\n",
		porStatus[ATIVO], porStatus[TRANCADO], porStatus[TRANSFERIDO], porStatus[FORMADO]);
	printf("Maior media geral: %s (%.2f)\n", p[melhor].nome, maiorGeral);
	printf("===========================================================================\n\n");
}

//Funcoes Arquivos Binarios Definicoes

//grava o vetor inteiro, substituindo o arquivo
void salvar(Aluno *p, int n) {
	FILE *arquivo = fopen(ARQUIVO, "wb");
	if (arquivo == NULL) {
		printf("Erro ao abrir o arquivo!\n");
		return;
	}
	if (n > 0) {
		if (fwrite(p, sizeof(Aluno), n, arquivo) != (size_t)n)
			printf("Erro ao gravar os registros!\n");
	}
	fclose(arquivo);
}

//grava um unico aluno na posicao dele dentro do arquivo, sem reescrever os outros
//retorna 1 se deu certo e 0 se nao conseguiu (quem chama grava o arquivo inteiro)
int salvarRegistro(Aluno *p, int pos) {
	FILE *arquivo = fopen(ARQUIVO, "r+b");
	int ok;
	if (arquivo == NULL) {
		return 0;
	}
	fseek(arquivo, (long)pos * (long)sizeof(Aluno), SEEK_SET);
	ok = (fwrite(p, sizeof(Aluno), 1, arquivo) == 1);
	fclose(arquivo);
	return ok;
}

void carregar(Aluno **p, int *n) {
	FILE *arquivo = fopen(ARQUIVO, "rb");
	long tamanho;
	size_t lidos;
	*p = NULL;
	*n = 0;
	if (arquivo == NULL) {
		return;	//primeira execucao: ainda nao existe arquivo
	}
	fseek(arquivo, 0, SEEK_END);
	tamanho = ftell(arquivo);
	if (tamanho < 0 || tamanho % sizeof(Aluno) != 0) {
		fclose(arquivo);
		//guarda o arquivo com outro nome para que o proximo salvar nao apague os dados
		remove(ARQUIVO_BACKUP);
		rename(ARQUIVO, ARQUIVO_BACKUP);
		printf("Arquivo invalido! Uma copia foi guardada em %s.\n", ARQUIVO_BACKUP);
		system("pause");
		return;
	}
	*n = (int)(tamanho / sizeof(Aluno));
	fseek(arquivo, 0, SEEK_SET);
	if (*n > 0) {
		*p = (Aluno*)malloc(*n * sizeof(Aluno));
		if (*p == NULL) {
			printf("Erro de alocacao!\n");
			*n = 0;
			fclose(arquivo);
			return;
		}
		lidos = fread(*p, sizeof(Aluno), *n, arquivo);
		if (lidos != (size_t)*n) {
			printf("Erro ao ler os registros!\n");
			free(*p);
			*p = NULL;
			*n = 0;
		}
	}
	fclose(arquivo);
}
