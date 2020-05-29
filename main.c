#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>  // EXIT_SUCCESS, EXIT_FAILURE

#include "opcoes.h"

int main() {
    int opcao;
    while (scanf("%d", &opcao) == 1) {  // Enquanto houver entradas no stdin
        if (!opcoes_executar(opcao)) {  // Executa opcao
            // Nao foi possivel executar a opcao
            printf("Opção invalida\n");
            return EXIT_FAILURE;
        }
    }
    return EXIT_SUCCESS;
}