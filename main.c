#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>  // Importa EXIT_SUCCESS e EXIT_FAILURE

#include "binario.h"
#include "binarioNaTela.h"
#include "csv.h"
#include "registro.h"

//* ================================= *//
//* ===== Prototipos de Funcoes ===== *//
//* ================================= *//

/**
 * 
 */
void opcao1(char* src, char* dest);

/**
 * 
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
                break;

            default:
                break;
        }
    }
    return EXIT_SUCCESS;
}

//* ===== Funcoes ===== *//

/*
CSV csv = new CSV(src)
Binario bin = new Binario(dest)

bin.status = false
Enquanto houver registros {
    bin.write(csv.getRegistro())
}
bin.status = true

binarioNaTela()
*/
void opcao1(char* src, char* dest) {
    FILE* csv = csv_open(src);
    if (!csv) {
        printf("Falha no carregamento do arquivo.\n");
        return;
    }

    FILE* bin = binario_new(dest);
    if (!bin) {
        csv_del(&csv);
        printf("Falha no carregamento do arquivo.\n");
        return;
    }

    Registro* registro;
    int cont = 0;  // Quantidade de Registros escritos

    bool status = false;
    binario_atualizaCabecalho(dest, &status, NULL, NULL, NULL, NULL);
    while ((registro = csv_lerRegistro(csv))) {
        binario_inserir(bin, registro);
        registro_del(&registro);
        cont++;
    }
    binario_del(&bin);

    status = true;
    binario_atualizaCabecalho(dest, &status, &cont, &cont, NULL, NULL);

    binarioNaTela(dest);

    csv_del(&csv);
}

void opcao2(char* path) {
    Binario* bin = binario_open(path);
    Registro* registro;

    while ((registro = binario_leRegistro(bin))) {
        registro_imprimir(registro);
        registro_del(&registro);
    }

    binario_del(&bin);
}