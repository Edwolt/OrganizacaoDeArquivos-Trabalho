#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>  // Importa EXIT_SUCCESS e EXIT_FAILURE

#include "binario.h"
#include "binarioNaTela.h"
#include "csv.h"
#include "registro.h"

// TODO atualizar documentacao

//* ================================= *//
//* ===== Prototipos de Funcoes ===== *//
//* ================================= *//

/**
 * Le um arquivo csv com nome src
 * e transforma em um arquivo binario e salva com nome dest
 */
void opcao1(char* src, char* dest);

/**
 * Imprime registros de um arquivo binario com nome path
 */
void opcao2(char* path);

//* ================ *//
//* ===== Main ===== *//
//* ================ *//

int main() {
    char src[100];
    char dest[100];
    int opcao;

    while (scanf("%d", &opcao) == 1) {  // Enquanto houver entradas no stdin
        switch (opcao) {
            case 1:
                scanf(" %s %s", src, dest);
                opcao1(src, dest);
                break;

            case 2:
                scanf(" %s", src);
                opcao2(src);
                break;

            default:
                printf("Opção invalida\n");
                return EXIT_FAILURE;
        }
    }
    return EXIT_SUCCESS;
}

//* =================== *//
//* ===== Funcoes ===== *//
//* =================== *//

void opcao1(char* src, char* dest) {
    FILE* csv = csv_abrir(src);
    if (!csv) {
        printf("Falha no carregamento do arquivo.\n");
        return;
    }

    FILE* bin = binario_criarDoCSV(dest, csv);
    if (!bin) {
        csv_fechar(&csv);
        printf("Falha no carregamento do arquivo.\n");
        return;
    }

    binario_fechar(&bin);

    binarioNaTela(dest);

    csv_fechar(&csv);
}

void opcao2(char* path) {
    int i;  // Iteradores

    bool status;
    int inseridos;
    int rrn;  // Quantidade de registros de dados no arquivo contando os removidos
    bool getted = binario_getCabecalho(path, &status, &rrn, &inseridos, NULL, NULL);

    if (!getted) {  // Ocorreu uma falha ao ler o registro cabecalho
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
    if (!bin) {
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