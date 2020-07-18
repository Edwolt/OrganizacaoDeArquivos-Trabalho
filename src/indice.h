#ifndef INDICE_H
#define INDICE_H

#include <stdbool.h>
#include <stdio.h>  // Arquivos
#include <stdlib.h>  // Alocao Dinamica

#include "registro.h"
#include "util.h"

#define RRNNULL -1

/**
 * TAD Indices para arquivo binario
 * Implementa um arquivo de indices com Arvore B
 */
typedef struct Indice Indice;

/**
 * Cria um arquivo de indice vazio para escrita com nome path
 * No arquivo ja eh escrito o registro cabecalho marcando o arquivo como inconsistente
 * O arquivo aponta para a primeira pagina
 * 
 * Se o arquivo ja existir, sobrescreve ele com um arquivo novo
 * Retorna NULL se nao for possivel criar o arquivo
 */
Indice* indice_criar(char* path);

/**
 * Abre arquivo de indice com nome path
 * Pode ser aberto para leitra ou para escrita dependendo da variavel escrita
 * Se for aberto para escrita ainda sera possivel ler dados do arquivo
 * O arquivo aponta para o primeiro registro de dados
 * 
 * Retorna NULL se nao for possivel abrir o arquivo
 * Retorna NULL se o arquivo estiver inconsistente
 */
Indice* indice_abrir(char* path, bool escrita);

/**
 * Abre arquivo para leitura
 * Wrapper para indice_abrir
 */
inline static Indice* indice_abrirLeitura(char* path) { return indice_abrir(path, false); }

/**
 * Abre arquivo para escrita
 * Wrapper para indice_abrir
 */
inline static Indice* indice_abrirEscrita(char* path) { return indice_abrir(path, true); }

/**
 * Fecha o arquivo (depois disso as escritas certamente estaram salvas no disco)
 * Se o arquivo estiver no modo escrita também atualiza o cabecalho
 * marcando o arquivo como consistente
 */
void indice_fechar(Indice** indice);

/**
 * Wrapper para o fseek
 * Faz o indice apontar para o rrn dado
 * 
 * whence usa as mesmas constantes que o fseek:
 * * SEEK_SET: Vai para o registro com o rrn passado (Desconsidera na contagem o registro cabecalho)
 * * SEEK_CUR: Anda rrn registros a partir do registro dado
 * * SEEK_END: Anda rrn registros a partir do fim do arquivo
 */
void indice_apontar(Indice* indice, int rrn, int whence);

/**
 * Retorna o rrn do arquivo de dados onde esta a chave com o id passado
 * O indice passa a apontar para o registro seguinte
 * 
 * Retorna RRNNULL se o registro nao foi encontrado
 * Retorna RRNNULL se ocorrer um erro
 * Retorna em acessos o numero de vezes que aconteceu um acesso a uma pagina
 * Retorna em erro se ocorreu um erro
 */
int indice_buscar(Indice* indice, int id, int* acessos, bool* erro);

/**
 * Insere o registro na Arvore B
 * Retorna se a operacao foi bem sucedida
 * Obs: Operacao requer escrita
 */
bool indice_inserir(Indice* indice, int id, int dado);

#endif