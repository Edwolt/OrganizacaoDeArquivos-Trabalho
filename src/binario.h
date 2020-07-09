#ifndef BINARIO_H
#define BINARIO_H

#include <stdbool.h>
#include <stdio.h>  // Arquivos
#include <stdlib.h>  // Alocacao Dinamica

#include "csv.h"
#include "registro.h"
#include "string.h"

/**
 * TAD Arquivo binario
 * Manipula um arquivo binario de registros
 */
typedef struct Binario Binario;

/**
 * Cria um novo arquivo com nome path
 * No arquivo ja eh escrito o registro cabecalho marcando o arquivo como inconsistente
 * O arquivo aponta para o primeiro registro de dados
 * 
 * Se o arquivo ja existir, sobrescreve ele com um arquivo novo
 * Obs: O arquivo aponta para o primeiro registro
 * 
 * Retorna NULL se nao for possivel criar o arquivo
 */
Binario* binario_criar(char* path);

/**
 * Abre arquivo com nome path para leitura
 * O arquivo aponta para o primeiro registro de dados
 * 
 * Retorna NULL se nao for possivel abrir o arquivo
 * Retorna NULL se o arquivo estiver inconsistente
 */
Binario* binario_abrirLeitura(char* path);

/**
 * Abre o arquivo com nome path para leitura e escrita
 * O arquivo aponta para o primeiro registro de dados
 * 
 * Retorna NULL se nao for possivel abrir o arquivo
 * Retorna NULL se o arquivo estiver inconsistente
 */
Binario* binario_abrirEscrita(char* path);

/**
 * Fecha o arquivo (depois disso as escritas certamente estaram salvas no disco)
 * Se o arquivo estiver no modo escrita também atualiza o cabecalho
 * marcando o arquivo como inconsistente
 */
void binario_fechar(Binario** binario);

/**
 * Faz o binario passar a apontar para o rrn dado
 * 
 * whence usa as mesmas constantes que o fseek:
 * * SEEK_SET: Vai para o registro com o rrn passado (Desconsidera na contagem o registro cabecalho)
 * * SEEK_CUR: Anda rrn registros a partir do registro dado
 * * SEEK_END: Anda rrn registros a partir do fim do arquivo
 */
void binario_apontar(Binario* binario, int rrn, int whence);

/**
 * Le um registro do arquivo onde esta sendo apontado
 * O binario passa a apontar para o registro seguinte
 * 
 * Retorna NULL se o registro estiver logicamente removido
 * Retorna NULL se ocorrer um erro
 * Retorna em erro se ocorreu um erro
 */
Registro* binario_lerRegistro(Binario* binario, bool* erro);

/**
 * Retorna o registro que esta no rrn passado
 * O binario passa a apontar para o registro seguinte
 * 
 * Retorna NULL se o registro estiver logicamente removido
 * Retorna NULL se ocorrer um erro
 * Retorna em erro se ocorreu um erro
 */
Registro* binario_buscar(Binario* binario, int rrn, bool* erro);

/**
 * Insere o registro no final do arquivo binario
 * Retorna se a operacao foi bem sucedida
 */
bool binario_inserir(Binario* binario, Registro* registro);

/**
 * Insere n registros no final do arquivo binario
 * Retorna se a operacao foi bem sucedida
 */
bool binario_inserirVarios(Binario* binario, Registro** registros, int n);

/**
 * Atualiza o registro para o qual o arquivo aponta
 * O binario passa a apontar para o registro seguinte
 * 
 * Retorna se a operacao foi bem sucedida
 */
bool binario_atualizar(Binario* binario, Registro* registro);

/**
 * Remove o registro para o qual o arquivo aponta
 * O binario passa a apontar para o registro seguinte
 * 
 * Retorna se a operacao foi bem sucedida
 */
bool binario_remover(Binario* binario);

//* =========================== *//
//* ===== Getter e Setter ===== *//
//* =========================== *//

int binario_getRRNProx(Binario* binario);
bool binario_estaVazio(Binario* binario);

#endif