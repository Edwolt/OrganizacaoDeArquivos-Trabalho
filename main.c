#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>  // EXIT_SUCCESS e EXIT_FAILURE

#include "binario.h"
#include "binarioNaTela.h"
#include "csv.h"
#include "registro.h"

/**
 * Le um arquivo CSV com nome src
 * e transforma em um arquivo binario e salva com nome dest
 */
void opcao1(char* src, char* dest) {
    FILE* csv = csv_abrir(src);
    if (!csv) {
        printf("Falha no carregamento do arquivo.\n");
        return;
    }

    bool ok = binario_gerarDoCSV(dest, csv);
    if (!ok) {
        csv_fechar(&csv);
        printf("Falha no carregamento do arquivo.\n");
        return;
    }

    binarioNaTela(dest);
    csv_fechar(&csv);
}

/**
 * Imprime registros de um arquivo binario com nome path
 */
void opcao2(char* path) {
    int i;  // Iteradores

    bool status;
    int inseridos;
    int rrn;  // Quantidade de registros de dados no arquivo contando os removidos
    bool ok = binario_getCabecalho(path, &status, &rrn, &inseridos, NULL, NULL);

    if (!ok) {  // Ocorreu uma falha ao ler o registro cabecalho
        printf("Falha no processamento do arquivo.\n");
        return;
    }

    if (!status) {  // O arquivo esta inconsistente
        printf("Falha no processamento do arquivo.\n");
        return;
    }

    if (inseridos == 0) {  // O arquivo nao possui dados
        printf("Registro inexistente.\n");
        return;
    }

    Binario* bin = binario_abrirLeitura(path);
    if (!bin) {  // O arquivo nao abriu
        printf("Falha no processamento do arquivo.\n");
        return;
    }

    Registro* registro;
    bool erro;

    for (i = 0; i < rrn; i++) {
        registro = binario_leRegistro(bin, &erro);

        if (erro) {
            printf("Falha no processamento do arquivo.\n");
            return;
        }

        if (registro == NULL) continue;

        registro_imprimir(registro);
        registro_apagar(&registro);
    }

    binario_fechar(&bin);
}

int main() {
    char src[100];
    char dest[100];
    int opcao;

    while (scanf("%d", &opcao) == 1) {  // Enquanto houver entradas no stdin
        switch (opcao) {
            /**
             * 1 src dest
             * src: nome do arquivo CSV de origem
             * dest: nome do arquivo binario de destino
             * 
             * Cria arquivo binario dest com os dados do arquivo CSV src
             */
            case 1:
                scanf(" %s %s", src, dest);
                opcao1(src, dest);
                break;

            /** 
             * 2 src
             * src: nome do arquivo binario
             * 
             * Imprime os dados de um arquivo binario de registros
             */
            case 2:
                scanf(" %s", src);
                opcao2(src);
                break;

            /**
             * 3 src m campo1 valor1 [campo2 valor2 [ ... ]]
             * src: nome do arquivo binario
             * m: numero de duplas campo-valor
             * campo valor: campo (string) seguido de um valor (depende do campo)
             * 
             * Faz uma busca paramentrizada pelo arquivo buscando registros
             * que cada campo_i tenha o valor_i 
             */
            case 3:
                printf("Operação não implementada :(\n");
                break;

            /**
             * 4 src rrn
             * src: nome do arquivo binario
             * rrn: RRN do regitro a ser recuperado
             * 
             * Imprime o valor do regitro com RRN igual ao passado
             */
            case 4:
                printf("Operação não implementada :(\n");
                break;

            /**
             * 5 src n
             * m1 nome11 valor11 [nome12 valor12 [ ... ]]
             * m2 nome21 valor21 [nome22 valor22 [ ... ]]
             * ...
             * mn nomen1 valorn1 [nomen2 valorn2 [ ... ]]
             * 
             * src: nome do arquivo binario
             * n: numero de registros
             * m: numero de dupla campo-valor
             * 
             * Remove registros seguindo n criterios diferentes
             * Cada linha subsequente diz o criterio de remoção
             * m_j diz quantas duplas campo-valor tem naquela linha
             * Para um registro passar pelo críterio o campo_ij deve ter valor_ij
             */
            case 5:
                printf("Operação não implementada :(\n");
                break;

            /**
             * 6 src n
             * cidadeMae1 cidadeBebe1 idNascimento1 idadeMae1 dataNascimento1 sexoBebe1 estadoMae1 estadoBebe1
             * cidadeMae2 cidadeBebe2 idNascimento2 idadeMae2 dataNascimento2 sexoBebe2 estadoMae2 estadoBebe2
             * ...
             * cidadeMaen cidadeBeben idNascimenton idadeMaen dataNascimenton sexoBeben estadoMaen estadoBeben
             * 
             * src: nome do arquivo binario
             * n: numero de registro
             * 
             * Insere n registros no arquivo binario src
             * embaixo segue o dados de cada registro
             */
            case 6:
                printf("Operação não implementada :(\n");
                break;

            /**
             * 7 src n
             * rrn1 m1 campo11 valor11 [nome12 valor12 [ ... ]]
             * rrn2 m2 nome21 valor21 [nome22 valor22 [ ... ]]
             * ...
             * rrn3 mn nomen1 valorn1 [nomen2 valorn2 [ ... ]]
             * 
             * src: nome do arquivo binario
             * n: numero de registro
             * rrn: RRN do registro
             * m: numero de duplas campo-valor
             * 
             * Atualiza n registros de rrn_i mudando o valor de m_i campos,
             * fazendo que o campo_ij tenha valor_ij
             */
            case 7:
                printf("Operação não implementada :(\n");
                break;

            default:
                printf("Opção invalida\n");
                return EXIT_FAILURE;
        }
    }
    return EXIT_SUCCESS;
}