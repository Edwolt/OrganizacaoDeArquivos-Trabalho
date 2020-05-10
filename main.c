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
leia destPath, srcPath
CSV src = new CSV(srcPath)
Arquivo dest = new Arquivo(destPath)

dest.status = false
Enquanto houver registros {
    arquivo.write(csv.getRegistro())
}
dest.status = true

binarioNaTela()
*/
void opcao1(char* src, char* dest) {
    FILE* csv = fopen(src, "r");
    FILE* binario = fopen(dest, "wb");
    Registro* registro;

    binario_setStatus(dest, false);
    while ((registro = csv_lerRegistro(csv))) {
        binario_inserir(binario, registro);
        registro_del(registro);
    }
    binario_del(binario);

    binario_setStatus(dest, true);

    binarioNaTela(dest);

    fclose(src);
    fclose(dest);
}

void opcao2(char* path) {
    Binario* bin = binario_open(path);
    Registro* registro;

    while (registro = binario_leRegistro(bin)) {
        registro_imprimir(registro);
    }

    binario_del(bin);
}