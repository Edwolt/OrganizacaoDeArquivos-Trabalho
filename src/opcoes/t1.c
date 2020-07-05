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
    Cabecalho* cab = NULL;
    Registro* reg = NULL;

    // Le opcao
    char src[PATH_TAM];
    char dest[PATH_TAM];
    scanf(" %s %s", src, dest);

    // Criar arquivo com registro cabecalho
    bin = binario_criar(dest);
    if (!bin) {  // Falha ao criar arquivo
        printf("Falha no carregamento do arquivo.\n");
        goto falha;
    }

    binario_fechar(&bin);

    cab = cabecalho_criarVazio();  // O cabecalho do arquivo criado está vazios
    if (!cab) {  // Falha ao criar objeto cabecalho
        printf("Falha no carregamento do arquivo.\n");
        goto falha;
    }
    // Marca arquivo como insconsistente
    cabecalho_setStatus(cab, false);
    bool ok = binario_setCabecalho(dest, cab);
    if (!ok) {  // Falha ao modificar cabecalho
        printf("Falha no carregamento do arquivo.\n");
        goto falha;
    }

    // Leitura e Escrita dos registro
    csv = csv_abrir(src);
    if (!csv) {  // Falha ao abrir arquivo
        printf("Falha no carregamento do arquivo.\n");
        goto falha;
    }

    bin = binario_abrirEscrita(dest);
    if (!bin) {  // Falha ao abrir arquivo
        printf("Falha no carregamento do arquivo.\n");
        goto falha;
    }

    while ((reg = csv_lerRegistro(csv))) {  // Enquanto houver registros para ler
        ok = binario_escreverRegistro(bin, reg);
        if (!ok) {  // Falha ao escrever registro
            printf("Falha no carregamento do arquivo.\n");
            goto falha;
        }

        registro_apagar(&reg);
        cabecalho_inserir(cab);
    }

    csv_fechar(&csv);
    binario_fechar(&bin);

    // Marca arquivo como consistente e atualiza cabecalho
    cabecalho_setStatus(cab, true);
    ok = binario_setCabecalho(dest, cab);
    if (!ok) {  // Falha ao modificar cabecalho
        printf("Falha no carregamento do arquivo.\n");
        goto falha;
    }

    cabecalho_apagar(&cab);

    // Imprimindo resultado
    binarioNaTela(dest);
    return;

falha:  // Ocorreu um erro e tem que desalocar variaveis (variaveis nao alocadas devem se NULL)
    binario_fechar(&bin);
    csv_fechar(&csv);
    cabecalho_apagar(&cab);
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
    Cabecalho* cab = NULL;
    Registro* reg = NULL;

    // Le opcao
    char path[PATH_TAM];
    scanf(" %s", path);

    // Verifica cabecalho
    cab = binario_getCabecalho(path);
    if (!cab) {  // Falha ao ler registro cabecalho
        printf("Falha no processamento do arquivo.\n");
        goto falha;
    }

    if (!cabecalho_getStatus(cab)) {  // Arquivo inconsistente
        printf("Falha no processamento do arquivo.\n");
        goto falha;
    }

    if (cabecalho_getInseridos(cab) == 0) {  // Arquivo nao possui dados
        printf("Registro inexistente.\n");
        goto falha;
    }

    // Le e imprime registros
    bin = binario_abrirLeitura(path);
    if (!bin) {  // Arquivo nao abriu
        printf("Falha no processamento do arquivo.\n");
        goto falha;
    }

    bool erro;
    int numRegs = cabecalho_getRRN(cab);
    cabecalho_apagar(&cab);
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
    cabecalho_apagar(&cab);
    registro_apagar(&reg);
}