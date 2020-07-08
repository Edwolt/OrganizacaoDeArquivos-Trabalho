#include "../opcoes.h"

/**
 * 1 src dest
 * 
 * src: arquivo csv
 * dest:arquivo binario de registros
 * 
 * Le o arquivo CSV com nome src
 * e armazena os dados lidos em um arquivo binario de registros,
 * salvando-o com o nome dest
 */
static void opcao1() {
    // Variaveis com alocacao dinamica
    Binario* bin = NULL;
    CSV* csv = NULL;
    Registro* reg = NULL;

    // Le opcao
    char src[PATH_TAM];
    char dest[PATH_TAM];
    scanf(" %s %s", src, dest);

    // Criar arquivo com registro cabecalho
    bin = binario_criar(dest);
    if (!bin) {  // Falha ao criar arquivo
        printf("Falha no carregamento do arquivo. (bin)\n");
        goto falha;
    }

    // Leitura e Escrita dos registro
    csv = csv_abrir(src);
    if (!csv) {  // Falha ao abrir arquivo
        printf("Falha no carregamento do arquivo.\n");
        goto falha;
    }

    bool ok;
    while ((reg = csv_lerRegistro(csv))) {  // Enquanto houver registros para ler
        ok = binario_inserir(bin, reg);
        if (!ok) {  // Falha ao escrever registro
            printf("Falha no carregamento do arquivo. (reg)\n");
            goto falha;
        }

        registro_apagar(&reg);
    }

    csv_fechar(&csv);
    binario_fechar(&bin);

    // Imprimindo resultado
    binarioNaTela(dest);
    return;

falha:  // Ocorreu um erro e tem que desalocar variaveis (variaveis nao alocadas devem se NULL)
    binario_fechar(&bin);
    csv_fechar(&csv);
    registro_apagar(&reg);
}

/** 
 * 2 src
 * 
 * src: arquivo binario de registros
 * 
 * Imprime os dados do arquivo binario com nome path
 */
static void opcao2() {
    int i;

    // Variaveis com alocacao dinamica
    Binario* bin = NULL;
    Registro* reg = NULL;

    // Le opcao
    char path[PATH_TAM];
    scanf(" %s", path);

    bin = binario_abrirLeitura(path);
    if (!bin) {  // Arquivo nao abriu
        printf("Falha no processamento do arquivo.\n");
        goto falha;
    }

    // Verifica cabecalho
    if (!binario_getStatus(bin)) {  // Arquivo inconsistente
        printf("Falha no processamento do arquivo.\n");
        goto falha;
    }

    if (binario_estaVazio(bin)) {  // Arquivo nao possui dados
        printf("Registro inexistente.\n");
        goto falha;
    }

    // Le e imprime registros

    bool erro;
    int numRegs = binario_getRRNProx(bin);
    for (i = 0; i < numRegs; i++) {
        reg = binario_lerRegistro(bin, &erro);
        if (erro) {  // Falha ao ler arquivo
            printf("Falha no processamento do arquivo.\n");
            goto falha;
        }

        if (!reg) continue;  // Registro removido

        registro_imprimir(reg);
        registro_apagar(&reg);
    }

    binario_fechar(&bin);
    return;

falha:  // Ocorreu um erro e tem que desalocar variaveis (variaveis nao alocadas devem se NULL)
    binario_fechar(&bin);
    registro_apagar(&reg);
}