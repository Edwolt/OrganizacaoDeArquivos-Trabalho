#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>  // Importa EXIT_SUCCESS e EXIT_FAILURE

#include "arquivo.h"
#include "binarioNaTela.h"
#include "csv.h"
#include "registro.h"

int main() {
    char destPath[100];
    char srcPath[100];
    int opcao;

    while (scanf("%d", &opcao) == 1) {  // Enquanto houver entradas no stdin
        switch (opcao) {
            case 1:
                scanf(" %s %s", srcPath, destPath);
                opcao1(srcPath, destPath);
                break;

            case 2:
                printf("Pintei");
                scanf(" %s", srcPath);
                break;

            default:
                break;
        }
    }
    return EXIT_SUCCESS;
}

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

void opcao2(char* srcPath) {
    FILE* arquivo = fopen(srcPath, "rb");
    Registro* registro;

    arquivo_abrir(arquivo);
    while (registro = arquivo_leRegistro(arquivo)) {
        registro_imprimir(registro);
    }

    arquivo_fechar(arquivo);
}