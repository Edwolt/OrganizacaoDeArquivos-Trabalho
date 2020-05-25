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
    char arq1[100];
    char arq2[100];
    int opcao;

    while (scanf("%d", &opcao) == 1) {  // Enquanto houver entradas no stdin
        switch (opcao) {
            /**
             * 1 src dest
             * src: arquivo csv
             * dest:arquivo binario de registros
             * 
             * Cria arquivo binario dest com os dados do arquivo CSV src
             */
            case 1:
                scanf(" %s %s", arq1, arq2);
                opcao1(arq1, arq2);
                break;

            /** 
             * 2 src
             * src: arquivo binario de registros
             * 
             * Imprime os dados do arquivo binario src
             */
            case 2:
                scanf(" %s", arq1);
                opcao2(arq1);
                break;

            /**
             * 3 bin m campo[1] valor[1] ... campo[m] valor[m]
             * bin: arquivo binario de registros
             * m: numero de duplas campo-valor que tem na linha
             * campo[i]: string
             * valor[i]: depende do campo[i]
             * 
             * Faz uma busca paramentrizada pelo arquivo bin
             * que cada campo[i] tenha o valor[i]
             */
            case 3:
                printf("Operação não implementada :(\n");
                break;

            /**
             * 4 bin rrn
             * bin: nome do arquivo binario
             * rrn: RRN do regitro a ser recuperado
             * 
             * Imprime o valor do regitro de RRN igual a rrn do arquivo binario bin
             */
            case 4:
                printf("Operação não implementada :(\n");
                break;

            /**
             * 5 bin n
             * m[1] campo[1][1] valor[1][1] ... campo[1][m[1]] valor[1][m[1]]
             * m[2] campo[2][1] valor[2][1] ... campo[2][m[2]] valor[2][m[2]]
             * ...
             * m[n] campo[n][1] valor[n][1] ... campo[n][m[n]] valor[n][m[n]]
             * 
             * bin: arquivo binario de registros
             * n: numero de linhas abaixo
             * m[i]: numero de duplas campo valor que tem na linha
             * campo[i][j]: string
             * valor[i][j]: depende do campo[i][j]
             * 
             * Remove registros seguindo n criterios diferentes do arquivo binario bin
             * Cada linha subsequente diz o criterio de remoção
             * Para um registro passar pelo criterio e ser removido
             * No arquivo src o campo[i][j] deve valer valor[i][j]
             */
            case 5:
                printf("Operação não implementada :(\n");
                break;

            /**
             * 6 bin n
             * dados[1]
             * dados[2]
             * ...
             * dados[n]
             * 
             * bin: nome do arquivo binario
             * n: numero de linhas abaixo
             * dados[i]: composto por valores de cada campo do registro, deve ficar assim:
             * cidadeMae[i] cidadeBebe[i] idNascimento[i] idadeMae[i] dataNascimento[i] sexoBebe[i] estadoMae[i] estadoBebe[i]
             * 
             * Insere n registros no arquivo binario bin
             * O conteudo de cada registro inserido eh dados[i]
             */
            case 6:
                printf("Operação não implementada :(\n");
                break;

            /**
             * 7 bin n
             * rrn[1] m[1] campo[1][1] valor[1][1] ... campo[1][m[1]] valor[1][m[1]]
             * rrn[2] m[2] campo[2][1] valor[2][1] ... campo[2][m[2]] valor[1][m[2]]
             * ...
             * rrn[n] m[n] campo[n][1] valor[n][1] ... campo[n][m[n]] valor[n][m[n]]
             * 
             * bin: arquivo binario
             * n: numero de linhas abaixo
             * rrn[i]: inteiro
             * m[i]: numero de duplas campo-valor
             * campo[i][j]: string
             * valor[i][j]: depende do campo[i][j]
             * 
             * Atualiza n registros do arquivo binario bin
             * O registro de valor rrn[i] passa a tem o valor dos seus campos mudados
             * sendo que o campo[i][j] passa a valer valor[i][j]
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