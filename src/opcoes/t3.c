#include "../opcoes.h"

/**
 * Cria Indice
 * 
 * 8 bin ind
 * 
 * bin: arquivo binario de registros
 * ind: arquivo binario de indices
 * 
 * Cria um arquivo de indices para idNascimento a partir de um arquivo binario
 */
static void opcao8() {
    int i;
    bool ok, erro;

    // Variaveis com alocacao dinamica
    Binario* bin = NULL;
    Indice* ind = NULL;
    Registro* reg = NULL;

    // Le opcao
    char src[PATH_TAM], dest[PATH_TAM];
    scanf(" %s %s", src, dest);

    // Abre binario e cria arquivo indice
    bin = binario_abrirLeitura(src);
    if (!bin) {
        printf("Falha no processamento do arquivo.\n");
        goto falha;
    }

    ind = indice_criar(dest);
    if (!ind) {
        printf("Falha no processamento do arquivo.\n");
        goto falha;
    }

    // Insere registros do binario no indice
    int numRegs = binario_getRRNProx(bin);
    for (i = 0; i < numRegs; i++) {
        // Le registro
        reg = binario_lerRegistro(bin, &erro);
        if (erro) {  // Falha ao ler arquivo
            printf("Falha no processamento do arquivo.\n");
            goto falha;
        }

        if (!reg) continue;

        // Insere registro
        ok = indice_inserir(ind, registro_getIdNascimento(reg), i);
        if (!ok) {  // Falha ao escrever no arquivo
            printf("Falha no processamento do arquivo.\n");
            goto falha;
        }

        registro_apagar(&reg);
    }
    binario_fechar(&bin);
    indice_fechar(&ind);

    binarioNaTela(dest);
    return;

falha:  // Ocorreu um erro e tem que desalocar variaveis (variaveis nao alocadas devem se NULL)
    binario_fechar(&bin);
    indice_fechar(&ind);
    registro_apagar(&reg);
}

/**
 * Busca com Indice
 * 
 * 9 bin ind idNascimento valor
 */
static void opcao9() {
    bool erro;

    // Variaveis com alocao dinamica
    Binario* bin = NULL;
    Indice* ind = NULL;
    Registro* reg = NULL;

    // Le opcao
    int id;
    char pathBin[PATH_TAM], pathInd[PATH_TAM];
    scanf(" %s %s %*s %d", pathBin, pathInd, &id);

    // Abre binario
    bin = binario_abrirLeitura(pathBin);
    if (!bin) {  // Falha ao abrir arquivo
        printf("Falha no processamento do arquivo.");
        goto falha;
    }

    if (binario_estaVazio(bin)) {  // Binario nao possui registros
        printf("Registro inexistente.");
        goto falha;
    }

    // Abre indice
    ind = indice_abrirLeitura(pathInd);
    if (!ind) {  // Falha ao abrir arquivo
        printf("Falha no processamento do arquivo.");
        goto falha;
    }

    // Busca registro
    int acessos;
    int rrn = indice_buscar(ind, id, &acessos, &erro);
    if (erro) {  // Erro ao ler do indice
        printf("Falha no processamento do arquivo.");
        goto falha;
    }

    if (rrn == RRNNULL) {  // Registro nao encontrado no indice
        printf("Registro inexistente.");
        goto falha;
    }

    reg = binario_buscar(bin, rrn, &erro);
    if (erro) {  // Erro ao ler do binario
        goto falha;
    }

    if (!reg) {  // Registro esta apagado
        printf("Registro inexistente.");
        goto falha;
    }

    // Imprime registro
    registro_imprimir(reg);
    printf("Quantidade de paginas da arvore-B acessadas: %d\n", acessos);

    binario_fechar(&bin);
    indice_fechar(&ind);
    registro_apagar(&reg);
    return;

falha:  // Ocorreu um erro e tem que desalocar variaveis (variaveis nao alocadas devem se NULL)
    binario_fechar(&bin);
    indice_fechar(&ind);
    registro_apagar(&reg);
}

/**
 * Insercao com Indice
 * 
 * 10 bin ind n 
 * dados[0]
 * dados[1]
 * ...
 * dados[n]
 * 
 * dados[i]: composto por valores de cada campo do registro, deve ficar assim:
 * cidadeMae[i] cidadeBebe[i] idNascimento[i] idadeMae[i] dataNascimento[i] se
 * 
 * Insere n registros no arquivo binario bin e atualizando o indice ind
 * O conteudo de cada registro inserido eh dados[i]
 */
static void opcao10() {
    int i;
    bool ok;

    // Variaveis com alocacao dinamica
    Binario* bin = NULL;
    Indice* ind = NULL;
    Registro* reg = NULL;

    // Le opcao
    char pathBin[PATH_TAM], pathInd[PATH_TAM];
    int n;
    scanf(" %s %s %d", pathBin, pathInd, &n);

    // Abre binario
    bin = binario_abrirEscrita(pathBin);
    if (!bin) {
        printf("Falha no processamento do arquivo.\n");
        goto falha;
    }

    // Abre indices
    ind = indice_abrirEscrita(pathInd);
    if (!ind) {
        printf("Falha no processamento do arquivo.\n");
        goto falha;
    }

    // Insere registros no arquivo
    for (i = 0; i < n; i++) {
        reg = registro_criarDoStdin();
        if (!reg) {
            printf("Falha no processamento do arquivo.\n");
            goto falha;
        }

        ok = binario_inserir(bin, reg);
        if (!ok) {
            printf("Falha no processamento do arquivo.\n");
            goto falha;
        }

        ok = indice_inserir(ind, registro_getIdNascimento(reg), binario_getRRNProx(bin) - 1);
        if (!ok) {
            printf("Falha no processamento do arquivo.\n");
            goto falha;
        }

        registro_apagar(&reg);
    }

    indice_fechar(&ind);
    binario_fechar(&bin);

    // Imprimindo resultado
    binarioNaTela(pathInd);
    return;

falha:
    binario_fechar(&bin);
    indice_fechar(&ind);
    registro_apagar(&reg);
}