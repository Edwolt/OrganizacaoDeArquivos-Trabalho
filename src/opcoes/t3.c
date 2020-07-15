#include "../opcoes.h"

/**
 * Cria Indice
 * 
 * 8 bin ind
 * 
 * bin: arquivo binario de registros
 * ind: arquivo binario de indices
 * 
 * 
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

falha:
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
    /*
    TODO Pseudo Algoritmo
    
    rrn = indice.buscar(id)
    return binario.buscar(rrn)
    */
}

/**
 * Insercao com Indice
 * 
 * 10 bin ind n 
 * dados[0]
 * dados[1]
 * ...
 * dados[n]
 */
static void opcao10() {
    /*
    TODO Pseudo Algoritmo
    
    dados = lerDados()
    for i in dados;
    binario.insere(i)
        indice.insere(i)
    */
}