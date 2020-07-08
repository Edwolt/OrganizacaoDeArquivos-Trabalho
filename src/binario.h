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
 * 
 * Por ser simples, nao tem motivo para cirar uma estrutura
 * por isso eh apenas um typedef
 */
typedef struct Binario Binario;

/**
 * Cria um novo arquivo binario com nome path
 * Cria nele o registro cabecalho
 * Se o arquivo ja existir, sobrescreve ele com um arquivo novo
 * 
 * Retorna o arquivo para escrita
 * Obs: Nao eh necessario pular o registro cabecalho
 * 
 * Retorna NULL se nao for possivel criar o arquivo
 * Retorna NULL se nao receber um path
 * 
 * Obs: Depois, use uma função para destruir o objeto
 */
Binario* binario_criar(char* path);

/**
 * Retorna arquivo binario com nome path aberto para leitura
 * Obs: Nao eh necessario pular o registro cabecalho
 * 
 * Retorna NULL se nao for possivel abrir o arquivo
 * Retorna NULL se nao receber um path
 * 
 * Obs: Depois, use uma funcao para destruir o objeto
 */
Binario* binario_abrirLeitura(char* path);

/**
 * Retorna o arquivo binario com nome path aberto para escrita
 * O arquivo tambem permite leitura
 * Obs: Nao eh necessario pular o registro cabecalho
 * 
 * Retorna NULL se nao for possivel abrir o arquivo
 * Retorna NULL se nao receber um path
 * 
 * Obs: Depois, use uma funcao para destruir o objeto
 */
Binario* binario_abrirEscrita(char* path);

/**
 * Destroi o objeto
 * 
 * Obs: Não apaga o arquivo, apenas desaloca memoria
 * Obs: Se for um arquivo de escrita, depois da execucao (senao ocorre erros) arquivo estara em disco
 */
void binario_fechar(Binario** binario);

/**
 * Faz o binario passar a apontar para o rrn dado
 * 
 * whence segue a mesma logica que o fseek:
 * * SEEK_SET: Vai para o registro com o rrn passado (Desconsidera na contagem o registro cabecalho)
 * * SEEK_CUR: Anda rrn registros a partir do registro dado
 * * SEEK_END: Anda rrn registros a partir do fim do arquivo
 */
void binario_apontar(Binario* binario, int rrn, int whence);

/**
 * Le um registro do arquivo onde esta sendo apontado
 * Retorna o Registro lido
 * Retorna NULL se o registro estiver logicamente removido
 * 
 * Salva em erro se ocorreu um erro
 * Retorna NULL se ocorrer um erro
 * 
 * O binario passa a apontar para o registro seguinte
 * 
 * Obs: Destrua o registro depois de usar
 */
Registro* binario_lerRegistro(Binario* binario, bool* erro);

/**
 * Retorna o registro que esta no rrn
 * Retorna NULL se o registro estiver logicamente removido
 * O binario passa a apontar para o registro seguinte
 * 
 * Salva em erro se ocorreu um erro
 * Retorna NULL se ocorrer um erro
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

bool binario_atualizar(Binario* binario, Registro* registro);

/**
 * Remove o registro para o qual o arquivo esta apontando
 * O binario passa a apontar para o registro seguinte
 */
bool binario_remover(Binario* binario);

//* =========================== *//
//* ===== Getter e Setter ===== *//
//* =========================== *//

int binario_getRRNProx(Binario* binario);
bool binario_estaVazio(Binario* binario);

#endif