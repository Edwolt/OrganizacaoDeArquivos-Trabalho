#ifndef BINARIO_H
#define BINARIO_H

#include <stdbool.h>
#include <stdio.h>  // Arquivos
#include <stdlib.h>  // Alocacao Dinamica

#include "alocaString.h"
#include "cabecalho.h"
#include "csv.h"
#include "registro.h"

/**
 * TAD Arquivo binario
 * Manipula um arquivo binario de registros
 * 
 * Por ser simples, nao tem motivo para cirar uma estrutura
 * por isso usei apenas um typedef
 */
typedef FILE Binario;

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
 * Cria um novo arquivo binário com nome path
 * a partir dos dados de um arquivo .csv usando o TAD CSV
 * 
 * Se o arquivo ja existir, sobrescreve ele com um arquivo novo
 * 
 * Retorna se foi possivel gerar
 */
bool binario_gerarDoCSV(char* path, CSV* csv);

/**
 * Retorna arquivo binario com nome path aberto para leitura
 * Obs: Nao eh necessario pular o registro cabecalho
 * 
 * Retorna NULL se nao for possivel Abrir o arquivo
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
 * Insere n registros no arquivo binario
 * O binario passa a apontar para o registro seguinte
 * Retorna se a operacao foi possivel
 */
bool binario_inserir(Binario* binario, Registro** registros, int n);

/**
 * Remove o registro para o qual o arquivo esta apontando
 * O binario passa a apontar para o registro seguinte
 */
bool binario_remover(Binario* binario);

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
 * Insere um registro no arquivo onde está sendo apontando
 * 
 * O binario passa a apontar para o registro seguinte
 * 
 * Retorna se foi possivel inserir o registro no arquivo
 */
bool binario_escreverRegistro(Binario* binario, Registro* registro);

/**
 * Atualiza um registro do arquivo
 * Obs: Nao verifica se esta no final do arquivo
 * A diferenca entre do escrever eh nao prencher o espaco nao ocupado com lixo
 * O binario passa a apontar para o registro seguinte
 * 
 * Retorna se foi possivel inserir o registro no arquivo
 */
bool binario_atualizarRegistro(Binario* binario, Registro* registro);

//* ============================== *//
//* ===== Registro Cabecalho ===== *//
//* ============================== *//

/**
 * Pega os valores do cabecalho de um arquivo
 * O parametro for NULL o campo correspondente nao sera lido
 * 
 * * status      = Status
 * * rrn         = RRNProxRegistro (Quantidade de registros contando os removidos e descontando o cabecalho)
 * * inseridos   = numeroRegistrosInseridos
 * * removidos   = numeroRegistrosRemovidos  
 * * atualizados = numeroRegistrosAtualizado
 */
bool binario_getCabecalho(char* path,
                          bool* status, int* rrn,
                          int* inseridos, int* removidos, int* atualizados);

/**
 * Atuliza o cabeçalho do arquivo
 * O parametro for NULL o campo correspondente nao sera atualizado
 * 
 * * status      = Status
 * * rrn         = RRNProxRegistro
 * * inseridos   = numeroRegistrosInseridos
 * * removidos   = numeroRegistrosRemovidos  
 * * atualizados = numeroRegistrosAtualizado
 */
bool binario_setCabecalho(char* path,
                          bool* status, int* rrn,
                          int* inseridos, int* removidos, int* atualizados);

#endif