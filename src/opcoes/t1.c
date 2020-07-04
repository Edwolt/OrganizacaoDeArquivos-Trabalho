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
    // Le opcao
    char src[PATH_TAM];
    char dest[PATH_TAM];
    scanf(" %s %s", src, dest);

    // Criar arquivo com registro cabecalho
    Binario* bin = binario_criar(dest);
    if (!bin) {  // Falha ao criar arquivo
        printf("Falha no carregamento do arquivo.\n");
        return;
    }

    binario_fechar(&bin);

    Cabecalho* cab = cabecalho_criarVazio();  // O cabecalho do arquivo criado está vazios
    if (!cab) {
        printf("Falha no carregamento do arquivo.\n");
        return;
    }

    // Marca arquivo como insconsistente
    cabecalho_setStatus(cab, false);
    bool ok = binario_setCabecalho(dest, cab);
    if (!ok) {  // Falha ao modificar cabecalho
        printf("Falha no carregamento do arquivo.\n");
        return;
    }

    // Leitura e Escrita dos registro
    Registro* reg;

    CSV* csv = csv_abrir(src);
    if (!csv) {  // Falha ao abrir arquivo
        cabecalho_apagar(&cab);

        printf("Falha no carregamento do arquivo.\n");
        return;
    }

    bin = binario_abrirEscrita(dest);
    if (!bin) {  // Falha ao abrir arquivo
        cabecalho_apagar(&cab);
        csv_fechar(&csv);

        printf("Falha no carregamento do arquivo.\n");
        return;
    }

    while ((reg = csv_lerRegistro(csv))) {  // Enquanto houver registros para ler
        ok = binario_escreverRegistro(bin, reg);
        if (!ok) {  // Falha ao escrever registro
            cabecalho_apagar(&cab);
            registro_apagar(&reg);
            csv_fechar(&csv);
            binario_fechar(&bin);

            printf("Falha no carregamento do arquivo.\n");
            return;
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
        cabecalho_apagar(&cab);
        printf("Falha no carregamento do arquivo.\n");
        return;
    }

    cabecalho_apagar(&cab);

    // Imprimindo resultado
    binarioNaTela(dest);
}

/** 
 * 2 src
 * 
 * src: arquivo binario de registros
 * 
 * Imprime os dados do arquivo binario com nome path
 */
static void opcao2() {
    int i;  // Iteradores

    // Le opcao
    char path[PATH_TAM];
    scanf(" %s", path);

    // Verifica cabecalho
    Cabecalho* cab = binario_getCabecalho(path);
    if (!cab) {  // Falha ao ler registro cabecalho
        printf("Falha no processamento do arquivo.\n");
        return;
    }

    if (!cabecalho_getStatus(cab)) {  // Arquivo inconsistente
        cabecalho_apagar(&cab);

        printf("Falha no processamento do arquivo.\n");
        return;
    }

    if (cabecalho_getInseridos(cab) == 0) {  // Arquivo nao possui dados
        cabecalho_apagar(&cab);

        printf("Registro inexistente.\n");
        return;
    }

    // Le e imprime registros
    Binario* bin = binario_abrirLeitura(path);
    if (!bin) {  // Arquivo nao abriu
        cabecalho_apagar(&cab);

        printf("Falha no processamento do arquivo.\n");
        return;
    }

    Registro* reg;
    bool erro;
    int numRegs = cabecalho_getRRN(cab);
    cabecalho_apagar(&cab);
    for (i = 0; i < numRegs; i++) {
        reg = binario_lerRegistro(bin, &erro);

        if (erro) {  // Falha ao ler arquivo
            binario_fechar(&bin);

            printf("Falha no processamento do arquivo.\n");
            return;
        }

        if (!reg) continue;  // Registro removido

        registro_imprimir(reg);
        registro_apagar(&reg);
    }

    binario_fechar(&bin);
}
