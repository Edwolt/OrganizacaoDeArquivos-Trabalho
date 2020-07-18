#include "../opcoes.h"

/**
 * Busca Parametrizada
 * 
 * 3 bin m campo[1] valor[1] ... campo[m] valor[m]
 * 
 * bin: arquivo binario de registros
 * m: numero de duplas campo-valor que tem na linha
 * campo[i]: string
 * valor[i]: depende do campo[i]
 * 
 * Faz uma busca paramentrizada pelo arquivo bin
 * emque cada campo[i] tenha o valor[i]
 */
static void opcao3() {
    int i;
    bool erro;

    // Variaveis com alocacao dinamica
    Binario* bin = NULL;
    Criterio* criterio = NULL;
    Registro* reg = NULL;

    // Le opcao
    char path[PATH_TAM];
    scanf(" %s", path);

    criterio = criterio_criarDoStdin();
    if (!criterio) {  // Falha ao ler criterio
        printf("Falha no processamento do arquivo.\n");
        goto falha;
    }

    // Abre arquivo e verifica cabecalho
    bin = binario_abrirLeitura(path);
    if (!bin) {  // Falha ao abrir arquivo
        printf("Falha no processamento do arquivo.\n");
        goto falha;
    }

    if (binario_estaVazio(bin)) {  // O arquivo nao possui dados
        printf("Registro Inexistente.\n");
        goto falha;
    }

    // Busca no arquivo
    int numRegs = binario_getRRNProx(bin);
    bool imprimiu = false;
    bool unico = criterio_unico(criterio);  // Diz seo criterio so pode satisfazer um registro
    for (i = 0; i < numRegs; i++) {
        reg = binario_lerRegistro(bin, &erro);
        if (erro) {  // Falha ao ler registro
            printf("Falha no processamento do arquivo.\n");
            goto falha;
        }

        if (!reg) continue;

        if (criterio_satisfaz(criterio, reg)) {
            registro_imprimir(reg);
            imprimiu = true;
            if (unico) {  // Não precisa mais passear pelo arquivo
                registro_apagar(&reg);
                break;
            }
        }

        registro_apagar(&reg);
    }

    binario_fechar(&bin);
    criterio_apagar(&criterio);

    if (!imprimiu) printf("Registro Inexistente.\n");  // Nada foi impresso
    return;

falha:  // Ocorreu um erro e tem que desalocar variaveis (variaveis nao alocadas devem se NULL)
    binario_fechar(&bin);
    criterio_apagar(&criterio);
    registro_apagar(&reg);
}

/**
 * Busca por RRN
 * 
 * 4 bin rrn
 * 
 * bin: nome do arquivo binario
 * rrn: RRN do regitro a ser recuperado
 * 
 * Imprime o regitro de RRN igual a rrn do arquivo binario bin
 */
static void opcao4() {
    bool erro;

    // Variaveis com alocacao dinamica
    Binario* bin = NULL;
    Registro* reg = NULL;

    // Le opcao
    char path[PATH_TAM];
    scanf(" %s", path);

    int rrn;
    scanf("%d", &rrn);

    // Abre arquivo e verifica cabecalho
    bin = binario_abrirLeitura(path);
    if (!bin) {  // Falha ao abrir arquivo
        printf("Falha no processamento do arquivo.\n");
        goto falha;
    }

    if (rrn <= 0 || rrn > binario_getRRNProx(bin)) {  // Registro nao existe
        printf("Registro Inexistente.\n");
        goto falha;
    }

    // Le e imprime registro
    reg = binario_buscar(bin, rrn, &erro);
    if (erro) {  // Falha ao ler registro
        printf("Falha no processamento do arquivo.\n");
        goto falha;
    }

    binario_fechar(&bin);

    if (reg) {  // registro != NULL
        registro_imprimir(reg);
        registro_apagar(&reg);
    } else {
        printf("Registro Inexistente.\n");
    }
    return;

falha:  // Ocorreu um erro e tem que desalocar variaveis (variaveis nao alocadas devem se NULL)
    binario_fechar(&bin);
    registro_apagar(&reg);
}

/**
 * Remocao
 * 
 * 5 bin n
 * m[1] campo[1][1] valor[1][1] ... campo[1][m[1]] valor[1][m[1]]
 * m[2] campo[2][1] valor[2][1] ... campo[2][m[2]] valor[2][m[2]]
 * ...
 * m[n] campo[n][1] valor[n][1] ... campo[n][m[n]] valor[n][m[n]]
 * 
 * bin: arquivo binario de registros
 * n: numero de linhas abaixo
 * m[i]: numero de duplas campo valor que tem na linha
 * campo[i][j]: string
 * valor[i][j]: depende do campo[i][j]
 * 
 * Remove registros de arquivo binario bin se este satifazer um dos criterios
 * Cada linha subsequente diz o criterio de remoção
 * Para um registro satisfazer um criterio
 * no arquivo bin o campo[i][j] deve valer valor[i][j]
 */
static void opcao5() {
    /*
    TODO busca pode ser melhorada se for por ID
    Nao consegui pensar em uma forma de fazer isso ainda
    */

    int i, j;
    bool erro;

    // Variaveis com alocacao dinamica
    Binario* bin = NULL;
    Registro* reg = NULL;
    Criterio** criterios = NULL;

    // Le opcao
    char path[PATH_TAM];
    int n;

    scanf(" %s %d", path, &n);

    // Le criterios
    criterios = malloc(n * sizeof(Criterio*));
    if (!criterios) {  // Falha ao alocar vetor
        printf("Falha no processamento do arquivo.\n");
        goto falha;
    }

    for (i = 0; i < n; i++) {
        criterios[i] = criterio_criarDoStdin();
        if (!criterios[i]) {  // Nao foi possivel criar criterios
            printf("Falha no processamento do arquivo.\n");
            goto falha;
        }
    }

    // Abre arquivo
    bin = binario_abrirEscrita(path);
    if (!bin) {  // Falha ao abrir arquivo
        printf("Falha no processamento do arquivo.\n");
        goto falha;
    }

    // Remove registros
    int numRegs = binario_getRRNProx(bin);
    for (i = 0; i < numRegs; i++) {  // Itera sobre registro
        reg = binario_lerRegistro(bin, &erro);
        if (erro) {  // Falha ao ler registro
            printf("Falha no processamento do arquivo.\n");
            goto falha;
        }

        if (!reg) continue;

        for (j = 0; j < n; j++) {  // Itera sobre criterio
            if (criterio_satisfaz(criterios[j], reg)) {
                binario_apontar(bin, -1, SEEK_CUR);  // Volta para o inicio do registro lido
                binario_remover(bin);
                break;
            }
        }

        registro_apagar(&reg);
    }

    for (i = 0; i < n; i++) criterio_apagar(&criterios[i]);
    free(criterios);
    criterios = NULL;

    binario_fechar(&bin);

    binarioNaTela(path);
    return;

falha:  // Ocorreu um erro e tem que desalocar variaveis (variaveis nao alocadas devem se NULL)
    binario_fechar(&bin);
    registro_apagar(&reg);
    if (criterios) {
        for (i--; i >= 0; i--) criterio_apagar(&criterios[i]);  // Desaloca o que ja foi alocado
        free(criterios);
    }
}

/**
 * Insercao
 * 
 * 6 bin n
 * dados[1]
 * dados[2]
 * ...
 * dados[n]
 * 
 * bin: nome do arquivo binario
 * n: numero de linhas abaixo
 * dados[i]: composto por valores de cada campo do registro, deve ficar assim:
 * cidadeMae[i] cidadeBebe[i] idNascimento[i] idadeMae[i] dataNascimento[i] sexoBebe[i] estadoMae[i] estadoBebe[i]
 * 
 * Insere n registros no arquivo binario bin
 * O conteudo de cada registro inserido eh dados[i]
 */
static void opcao6() {
    int i;
    bool ok;

    // Variaveis com alocacao dinamica
    Binario* bin = NULL;
    Registro* reg = NULL;

    // Le opcao
    char path[PATH_TAM];
    int n;
    scanf(" %s %d", path, &n);

    // Abre arquivo
    bin = binario_abrirEscrita(path);
    if (!bin) {
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

        registro_apagar(&reg);
    }

    binario_fechar(&bin);

    // Imprimindo resultado
    binarioNaTela(path);
    return;

falha:  // Ocorreu um erro e tem que desalocar variaveis (variaveis nao alocadas devem se NULL)
    binario_fechar(&bin);
    registro_apagar(&reg);
}

/**
 * Atualizacao
 * 
 * 7 bin n
 * rrn[1] m[1] campo[1][1] valor[1][1] ... campo[1][m[1]] valor[1][m[1]]
 * rrn[2] m[2] campo[2][1] valor[2][1] ... campo[2][m[2]] valor[1][m[2]]
 * ...
 * rrn[n] m[n] campo[n][1] valor[n][1] ... campo[n][m[n]] valor[n][m[n]]
 * 
 * bin: arquivo binario
 * n: numero de linhas abaixo
 * rrn[i]: inteiro
 * m[i]: numero de duplas campo-valor
 * campo[i][j]: string
 * valor[i][j]: depende do campo[i][j]
 * 
 * Atualiza n registros do arquivo binario bin
 * O registro de valor rrn[i] passa a tem o valor dos seus campos mudados
 * sendo que o campo[i][j] passa a valer valor[i][j]
 */
static void opcao7() {
    int i;
    bool ok, erro;

    // Variaveis com alocacao dinamica
    Binario* bin = NULL;
    Registro* reg = NULL;
    int* rrnsAtualizar = NULL;
    Criterio** novosValores = NULL;

    // Le opcao
    char path[PATH_TAM];
    int n;
    scanf(" %s %d", path, &n);

    // Le o que deve atualizar (Guarda duplas campo-valor no TAD criterio)
    rrnsAtualizar = malloc(n * sizeof(int));
    if (!rrnsAtualizar) {  // Falha ao criar vetor de inteiros
        printf("Falha no processamento do arquivo.\n");
        goto falha;
    }

    novosValores = malloc(n * sizeof(Criterio*));
    if (!novosValores) {  // Falha ao criar vetor de criterios
        printf("Falha no processamento do arquivo.\n");
        goto falha;
    }

    for (i = 0; i < n; i++) {
        scanf("%d ", &rrnsAtualizar[i]);  // Le rrn
        novosValores[i] = criterio_criarDoStdin();  // Le duplas campo-valor

        if (!novosValores[i]) {  // Falha ao criar criterio
            printf("Falha no processamento do arquivo.\n");
            goto falha;
        }
    }

    // Abre arquivo
    bin = binario_abrirEscrita(path);
    if (!bin) {  // Falha ao abrir arquivo
        printf("Falha no processamento do arquivo.\n");
        goto falha;
    }

    // Atualiza registros
    int numRegs = binario_getRRNProx(bin);
    for (i = 0; i < n; i++) {
        if (rrnsAtualizar[i] < 0 || rrnsAtualizar[i] >= numRegs) continue;  // Verifica RRN

        reg = binario_buscar(bin, rrnsAtualizar[i], &erro);  // Le registro
        binario_apontar(bin, -1, SEEK_CUR);  // Volta para registro no RRN rrns[i]

        if (erro) {  // Erro ao alocar
            printf("Falha no processamento do arquivo.\n");
            goto falha;
        }

        if (!reg) continue;  // Registro removido

        // Atualiza registro
        criterio_atualizarRegistro(&novosValores[i], reg);  // Pega os valores em criterio e colocar no registro
        ok = binario_atualizar(bin, reg);  // Escreve o registro em cima do anterior
        if (!ok) {
            printf("Falha no processamento do arquivo.\n");
            goto falha;
        }

        registro_apagar(&reg);
    }

    binario_fechar(&bin);
    free(rrnsAtualizar);
    rrnsAtualizar = NULL;
    for (i = 0; i < n; i++) criterio_apagar(&novosValores[i]);
    free(novosValores);
    novosValores = NULL;

    // Imprime resultado
    binarioNaTela(path);
    return;

falha:  // Ocorreu um erro e tem que desalocar variaveis (variaveis nao alocadas devem se NULL)
    binario_fechar(&bin);
    registro_apagar(&reg);
    free(rrnsAtualizar);
    if (novosValores) {
        for (i--; i >= 0; i--) criterio_apagar(&novosValores[i]);
        free(novosValores);
    }
}