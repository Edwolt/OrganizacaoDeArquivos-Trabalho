#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>  // Importa EXIT_SUCCESS e EXIT_FAILURE

#include "arquivo.h"
#include "binarioNaTela.h"
#include "csv.h"
#include "registro.h"

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
int main() {
    char destPath[100];
    char srcPath[100];
    int opcao;

    scanf("%d", &opcao);
    switch (opcao) {
        case 1:
            scanf("%s %s", srcPath, destPath);
            opcao1(srcPath, destPath);
            break;

        case 2:
            scanf("%s", srcPath);
            break;

        default:
            break;
    }

    return EXIT_SUCCESS;
}

void opcao1(char* srcPath, char* destPath) {
    CSV* src = csv_new(srcPath);
    Arquivo* dest = arquivo_new(destPath);
    Registro* registro;

    arquivo_setStatus(dest, false);
    arquivo_criar(dest);
    while ((registro = csv_lerRegistro(src))) {
        arquivo_inserir(registro);
        registro_del(registro);
    }
    arquivo_salvar(dest);
    arquivo_setStatus(dest, true);

    binarioNaTela(destPath);

    arquivo_del(src);
    csv_del(dest);
}