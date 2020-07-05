#include "../opcoes.h"

/**
 * 3 bin m campo[1] valor[1] ... campo[m] valor[m]
 * 
 * bin: arq+uivo binario de registros
 * m: numero de duplas campo-valor que tem na linha
 * campo[i]: string
 * valor[i]: depende do campo[i]
 * 
 * Faz uma busca paramentrizada pelo arquivo bin
 * que cada campo[i] tenha o valor[i]
 */
static void opcao3() {
    int i;

    // Variaveis com alocacao dinamica
    Binario* bin = NULL;
    Cabecalho* cab = NULL;
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

    // Verifica cabecalho
    cab = binario_getCabecalho(path);
    if (!cab) {  // Falha ao ler cabecalho
        printf("Falha no processamento do arquivo.\n");
        goto falha;
    }

    if (!cabecalho_getStatus(cab)) {  // Arquivo inconsistente
        printf("Falha no processamento do arquivo.\n");
        goto falha;
    }

    if (cabecalho_getInseridos(cab) == 0) {  // O arquivo nao possui dados
        printf("Registro Inexistente.\n");
        goto falha;
    }

    // Busca no arquivo
    bin = binario_abrirLeitura(path);
    if (!bin) {  // Falha ao abrir arquivo
        printf("Falha no processamento do arquivo.\n");
        goto falha;
    }

    int numRegs = cabecalho_getRRN(cab);
    cabecalho_apagar(&cab);

    bool erro;
    bool imprimiu = false;
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
        }

        registro_apagar(&reg);
    }

    binario_fechar(&bin);
    criterio_apagar(&criterio);

    if (!imprimiu) printf("Registro Inexistente.\n");  // Nada foi impresso
    return;

falha:  // Ocorreu um erro e tem que desalocar variaveis (variaveis nao alocadas devem se NULL)
    binario_fechar(&bin);
    cabecalho_apagar(&cab);
    criterio_apagar(&criterio);
    registro_apagar(&reg);
}

/**
 * 4 bin rrn
 * 
 * bin: nome do arquivo binario
 * rrn: RRN do regitro a ser recuperado
 * 
 * Imprime o valor do regitro de RRN igual a rrn do arquivo binario bin
 */
static void opcao4() {
    // Variaveis com alocacao dinamica
    Binario* bin = NULL;
    Cabecalho* cab = NULL;
    Registro* reg = NULL;

    // Le opcao
    char path[PATH_TAM];
    scanf(" %s", path);

    int rrn;
    scanf("%d", &rrn);

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

    if (rrn <= 0 || rrn > cabecalho_getRRN(cab)) {  // Registro nao existe
        printf("Registro Inexistente.");
        goto falha;
    }

    cabecalho_apagar(&cab);

    // Le e imprime registro
    bin = binario_abrirLeitura(path);
    if (!bin) {  // Falha ao abrir arquivo
        printf("Falha no processamento do arquivo.\n");
        goto falha;
    }

    bool erro;
    reg = binario_buscar(bin, rrn, &erro);
    binario_fechar(&bin);

    if (erro) {  // Falha ao ler registro
        printf("Falha no processamento do arquivo.\n");
        goto falha;
    }

    if (reg) {  // registro != NULL
        registro_imprimir(reg);
        registro_apagar(&reg);
    } else {
        printf("Registro Inexistente.");
    }
    return;

falha:  // Ocorreu um erro e tem que desalocar variaveis (variaveis nao alocadas devem se NULL)
    binario_fechar(&bin);
    cabecalho_apagar(&cab);
    registro_apagar(&reg);
}

/**
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
 * Remove registros seguindo n criterios diferentes do arquivo binario bin
 * Cada linha subsequente diz o criterio de remoção
 * Para um registro passar pelo criterio e ser removido
 * No arquivo src o campo[i][j] deve valer valor[i][j]
 */
static void opcao5() {  // TODO busca pode ser melhorada se for por ID
    int i, j;

    // Variaveis com alocacao dinamica
    Binario* bin = NULL;
    Cabecalho* cab = NULL;
    Registro* reg = NULL;
    Criterio** criterios = NULL;

    // Le opcao
    char path[PATH_TAM];
    int n;

    scanf(" %s %d", path, &n);

    criterios = malloc(n * sizeof(Criterio*));
    if (!criterios) {  // Falha ao alocar vetor
        printf("Falha no processamento do arquivo.\n");
        goto falha;
    }

    // Le criterios
    for (i = 0; i < n; i++) {
        criterios[i] = criterio_criarDoStdin();
        if (!criterios[i]) {  // Nao foi possivel criar criterios
            printf("Falha no processamento do arquivo.\n");
            goto falha;
        }
    }

    // Verifica cabecalho
    cab = binario_getCabecalho(path);
    if (!cab) {  // Falha o ler registro cabecalho
        printf("Falha no processamento do arquivo.\n");
        goto falha;
    }

    if (!cabecalho_getStatus(cab)) {  // Arquivo incosistente
        printf("Falha no processamento do arquivo.\n");
        goto falha;
    }

    // Marca arquivo como inconsistente
    cabecalho_setStatus(cab, false);
    bool ok = binario_setCabecalho(path, cab);
    if (!ok) {  // Falha ao modificar cabecalho
        printf("Falha no carregamento do arquivo.\n");
        goto falha;
    }

    // Remove registros
    bin = binario_abrirEscrita(path);
    if (!bin) {  // Falha ao abrir arquivo
        printf("Falha no processamento do arquivo.\n");
        goto falha;
    }

    bool erro;
    int numRegs = cabecalho_getRRN(cab);
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
                cabecalho_remover(cab);
                break;
            }
        }

        registro_apagar(&reg);
    }

    for (i = 0; i < n; i++) criterio_apagar(&criterios[i]);
    free(criterios);
    criterios = NULL;

    binario_fechar(&bin);

    // Marca arquivo como consistente e atualiza cabacalho
    cabecalho_setStatus(cab, true);
    ok = binario_setCabecalho(path, cab);
    if (!ok) {  // Falha ao modificar cabecalho
        printf("Falha no carregamento do arquivo.\n");
        goto falha;
    }

    binarioNaTela(path);
    return;

falha:  // Ocorreu um erro e tem que desalocar variaveis (variaveis nao alocadas devem se NULL)
    binario_fechar(&bin);
    cabecalho_apagar(&cab);
    registro_apagar(&reg);
    if (criterios) {
        for (i--; i >= 0; i--) criterio_apagar(&criterios[i]);  // Desaloca o que ja foi alocado
        free(criterios);
    }
}

/**
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

    // Variaveis com alocacao dinamica
    Binario* bin = NULL;
    Cabecalho* cab = NULL;
    Registro** regs = NULL;

    // Le opcao
    char path[PATH_TAM];
    int n;
    scanf(" %s %d", path, &n);

    regs = malloc(n * sizeof(Registro*));
    if (!regs) return;  // Falha ao alocar vetor
    for (i = 0; i < n; i++) {
        regs[i] = registro_criarDoStdin();
        if (!regs[i]) {
            printf("Falha no processamento do arquivo.\n");
            goto falha;
        }
    }

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

    // Marca arquivo como inconsistente
    cabecalho_setStatus(cab, false);
    bool ok = binario_setCabecalho(path, cab);
    if (!ok) {  // Falha ao modificar cabecalho
        printf("Falha no processamento do arquivo.\n");
        goto falha;
    }

    // Insere registros no arquivo
    bin = binario_abrirEscrita(path);
    if (!bin) {
        printf("Falha no processamento do arquivo.\n");
        goto falha;
    }

    binario_apontar(bin, cabecalho_getRRN(cab), SEEK_SET);

    ok = binario_inserir(bin, regs, n);

    for (i = 0; i < n; i++) registro_apagar(&regs[i]);
    free(regs);
    regs = NULL;

    if (!ok) {  // Falha ao inserir no arquivo
        printf("Falha no processamento do arquivo.\n");
        goto falha;
    }

    binario_fechar(&bin);

    // Atualiza Cabecalho
    cabecalho_setStatus(cab, true);
    cabecalho_setRRN(cab, cabecalho_getRRN(cab) + n);  // rrn += n;
    cabecalho_setInseridos(cab, cabecalho_getInseridos(cab) + n);  // inseridos += n;

    ok = binario_setCabecalho(path, cab);
    cabecalho_apagar(&cab);
    if (!ok) {  // Falha ao modificar cabecalho
        printf("Falha no processamento do arquivo.\n");
        goto falha;
    }

    // Imprimindo resultado
    binarioNaTela(path);
    return;

falha:  // Ocorreu um erro e tem que desalocar variaveis (variaveis nao alocadas devem se NULL)
    binario_fechar(&bin);
    cabecalho_apagar(&cab);
    if (regs) {
        for (i = 0; i < n; i++) registro_apagar(&regs[i]);
        free(regs);
    }
}

/**
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
static void opcao7() {  // TODO busca pode ser melhorada se for por ID
    int i;

    // Variaveis com alocacao dinamica
    Binario* bin = NULL;
    Cabecalho* cab = NULL;
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

    // Verifica Cabecalho
    cab = binario_getCabecalho(path);
    if (!cab) {  // Falha ao ler registro cabecalho
        printf("Falha no processamento do arquivo.\n");
        goto falha;
    }

    if (!cabecalho_getStatus(cab)) {
        printf("Falha no processamento do arquivo.\n");
        goto falha;
    }

    // Marca arquivo como insconsistente
    cabecalho_setStatus(cab, false);
    bool ok = binario_setCabecalho(path, cab);
    if (!ok) {  // Falha ao modificar cabecalho
        printf("Falha no processamento do arquivo.\n");
        goto falha;
    }

    // Atualiza registros
    bin = binario_abrirEscrita(path);
    if (!bin) {  // Falha ao abrir arquivo
        printf("Falha no processamento do arquivo.\n");
        goto falha;
    }

    bool erro;
    int numRegs = cabecalho_getRRN(cab);
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
        ok = binario_atualizarRegistro(bin, reg);  // Escreve o registro em cima do anterior
        if (!ok) {
            printf("Falha no processamento do arquivo.\n");
            goto falha;
        }

        cabecalho_atualizar(cab);
        registro_apagar(&reg);
    }

    binario_fechar(&bin);
    free(rrnsAtualizar);
    rrnsAtualizar = NULL;
    for (i = 0; i < n; i++) criterio_apagar(&novosValores[i]);
    free(novosValores);
    novosValores = NULL;

    // Atualiza cabecalho
    cabecalho_setStatus(cab, true);
    ok = binario_setCabecalho(path, cab);
    cabecalho_apagar(&cab);
    if (!ok) {  // Falha ao modificar cabecalho
        printf("Falha no processamento do arquivo.\n");
        return;
    }

    // Imprime resultado
    binarioNaTela(path);
    return;

falha:  // Ocorreu um erro e tem que desalocar variaveis (variaveis nao alocadas devem se NULL)
    binario_fechar(&bin);
    cabecalho_apagar(&cab);
    registro_apagar(&reg);
    free(rrnsAtualizar);
    if (novosValores) {
        for (i--; i >= 0; i--) criterio_apagar(&novosValores[i]);
        free(novosValores);
    }
}