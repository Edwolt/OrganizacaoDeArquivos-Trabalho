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
    int i;  // Iteradores

    // Le opcao
    char path[PATH_TAM];
    scanf(" %s", path);

    Criterio* criterio = criterio_criarDoStdin();
    if (!criterio) {  // Falha ao ler criterio
        printf("Falha no processamento do arquivo.\n");
        return;
    }

    // Verifica cabecalho
    Cabecalho* cab = binario_getCabecalho(path);
    if (!cab) {  // Falha ao ler cabecalho
        criterio_apagar(&criterio);

        printf("Falha no processamento do arquivo.\n");
        return;
    }

    if (!cabecalho_getStatus(cab)) {  // Arquivo inconsistente
        criterio_apagar(&criterio);
        cabecalho_apagar(&cab);

        printf("Falha no processamento do arquivo.\n");
        return;
    }

    if (cabecalho_getInseridos(cab) == 0) {  // O arquivo nao possui dados
        criterio_apagar(&criterio);
        cabecalho_apagar(&cab);

        printf("Registro Inexistente.\n");
        return;
    }

    // Busca no arquivo
    Binario* bin = binario_abrirLeitura(path);
    if (!bin) {  // Falha ao abrir arquivo
        criterio_apagar(&criterio);
        cabecalho_apagar(&cab);

        printf("Falha no processamento do arquivo.\n");
        return;
    }

    int numRegs = cabecalho_getRRN(cab);
    cabecalho_apagar(&cab);

    Registro* reg;
    bool erro;
    bool imprimiu = false;
    for (i = 0; i < numRegs; i++) {
        reg = binario_lerRegistro(bin, &erro);

        if (erro) {  // Falha ao ler registro
            printf("Falha no processamento do arquivo.\n");
            return;
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
    // Le opcao
    char path[PATH_TAM];
    scanf(" %s", path);

    int rrn;
    scanf("%d", &rrn);

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

    if (rrn <= 0 || rrn > cabecalho_getRRN(cab)) {  // Registro nao existe
        cabecalho_apagar(&cab);

        printf("Registro Inexistente.");
        return;
    }

    cabecalho_apagar(&cab);

    // Le e imprime registro
    Binario* bin = binario_abrirLeitura(path);
    if (!bin) {  // Falha ao abrir arquivo
        printf("Falha no processamento do arquivo.\n");
        return;
    }

    bool erro;
    Registro* reg = binario_buscar(bin, rrn, &erro);
    binario_fechar(&bin);

    if (erro) {  // Falha ao ler registro
        printf("Falha no processamento do arquivo.\n");
        return;
    }

    if (reg) {  // registro != NULL
        registro_imprimir(reg);
        registro_apagar(&reg);
    } else {
        printf("Registro Inexistente.");
    }
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
    int i, j;  // Iteradores

    // Le opcao
    char path[PATH_TAM];
    int n;

    scanf(" %s %d", path, &n);

    Criterio** criterios = malloc(n * sizeof(Criterio*));
    if (!criterios) {  // Falha ao alocar vetor
        printf("Falha no processamento do arquivo.\n");
        return;
    }

    // Le criterios
    for (i = 0; i < n; i++) {
        criterios[i] = criterio_criarDoStdin();
        if (!criterios[i]) {  // Nao foi possivel criar criterios
            for (i--; i >= 0; i--) criterio_apagar(&criterios[i]);  // Desaloca o que ja foi alocado
            free(criterios);

            printf("Falha no processamento do arquivo.\n");
            return;
        }
    }

    // Verifica cabecalho
    Cabecalho* cab = binario_getCabecalho(path);
    if (!cab) {  // Falha o ler registro cabecalho
        for (i = 0; i < n; i++) criterio_apagar(&criterios[i]);
        free(criterios);

        printf("Falha no processamento do arquivo.\n");
        return;
    }

    if (!cabecalho_getStatus(cab)) {  // Arquivo incosistente
        cabecalho_apagar(&cab);
        for (i = 0; i < n; i++) criterio_apagar(&criterios[i]);
        free(criterios);

        printf("Falha no processamento do arquivo.\n");
        return;
    }

    if (cabecalho_getInseridos(cab) == 0) {  // O arquivo nao possui dados
        cabecalho_apagar(&cab);
        for (i = 0; i < n; i++) criterio_apagar(&criterios[i]);
        free(criterios);

        printf("Registro Inexistente.\n");
        return;
    }

    // Marca arquivo como inconsistente
    cabecalho_setStatus(cab, false);
    bool ok = binario_setCabecalho(path, cab);
    if (!ok) {  // Falha ao modificar cabecalho
        cabecalho_apagar(&cab);
        for (i = 0; i < n; i++) criterio_apagar(&criterios[i]);
        free(criterios);

        printf("Falha no carregamento do arquivo.\n");
        return;
    }

    // Remove registros
    Binario* bin = binario_abrirEscrita(path);

    if (!bin) {  // Falha ao abrir arquivo
        cabecalho_apagar(&cab);
        for (i = 0; i < n; i++) criterio_apagar(&criterios[i]);
        free(criterios);

        printf("Falha no processamento do arquivo.\n");
        return;
    }

    bool erro;
    bool apagou = false;
    Registro* reg;
    int numRegs = cabecalho_getRRN(cab);
    for (i = 0; i < numRegs; i++) {  // Itera sobre registro
        reg = binario_lerRegistro(bin, &erro);

        if (erro) {  // Falha ao ler registro
            cabecalho_apagar(&cab);
            for (i = 0; i < n; i++) criterio_apagar(&criterios[i]);
            free(criterios);
            binario_fechar(&bin);
            registro_apagar(&reg);

            printf("Falha no processamento do arquivo.\n");
            return;
        }

        if (!reg) continue;

        for (j = 0; j < n; j++) {  // Itera sobre criterio
            if (criterio_satisfaz(criterios[j], reg)) {
                binario_apontar(bin, -1, SEEK_CUR);  // Volta para o inicio do registro lido
                binario_remover(bin);
                cabecalho_remover(cab);
                apagou = true;
                break;
            }
        }

        registro_apagar(&reg);
    }

    for (i = 0; i < n; i++) criterio_apagar(&criterios[i]);
    free(criterios);
    binario_fechar(&bin);

    // Marca arquivo como consistente e atualiza cabacalho
    cabecalho_setStatus(cab, true);
    ok = binario_setCabecalho(path, cab);
    if (!ok) {  // Falha ao modificar cabecalho
        printf("Falha no carregamento do arquivo.\n");
        return;
    }

    // Imprime resultado
    if (apagou) {
        binarioNaTela(path);
    } else {
        printf("Registro Inexistente.\n");  // Nada foi impresso}
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
    int i;  // Iteradores
    // Le opcao
    char path[PATH_TAM];
    int n;

    scanf(" %s %d", path, &n);

    Registro** regs = malloc(n * sizeof(Registro*));
    if (!regs) return;  // Falha ao alocar vetor
    for (i = 0; i < n; i++) {
        regs[i] = registro_criarDoStdin();
        if (!regs[i]) {
            for (i--; i >= 0; i--) registro_apagar(&regs[i]);  // Desaloca o que ja foi alocado
            free(regs);
        }
    }

    // Verifica cabecalho
    Cabecalho* cab = binario_getCabecalho(path);
    if (!cab) {  // Falha ao ler registro cabecalho
        for (i--; i >= 0; i--) registro_apagar(&regs[i]);
        free(regs);

        printf("Falha no processamento do arquivo.\n");
        return;
    }

    if (!cabecalho_getStatus(cab)) {  // Arquivo inconsistente
        cabecalho_apagar(&cab);
        for (i--; i >= 0; i--) registro_apagar(&regs[i]);
        free(regs);

        printf("Falha no processamento do arquivo.\n");
        return;
    }

    // Marca arquivo como inconsistente
    cabecalho_setStatus(cab, false);
    bool ok = binario_setCabecalho(path, cab);
    if (!ok) {  // Falha ao modificar cabecalho
        cabecalho_apagar(&cab);
        for (i--; i >= 0; i--) registro_apagar(&regs[i]);
        free(regs);

        printf("Falha no processamento do arquivo.\n");
        return;
    }

    // Insere registros no arquivo
    Binario* bin = binario_abrirEscrita(path);
    if (!bin) {
        cabecalho_apagar(&cab);
        for (i = 0; i < n; i++) registro_apagar(&regs[i]);
        free(regs);

        printf("Falha no processamento do arquivo.\n");
        return;
    }
    binario_apontar(bin, cabecalho_getRRN(cab), SEEK_SET);

    ok = binario_inserir(bin, regs, n);
    for (i = 0; i < n; i++) registro_apagar(&regs[i]);
    free(regs);

    if (!ok) {  // Falha ao inserir no arquivo
        cabecalho_apagar(&cab);

        binario_fechar(&bin);
        printf("Falha no processamento do arquivo.\n");
        return;
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
        return;
    }

    // Imprimindo resultado
    binarioNaTela(path);
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
    int i;  // Iteradores

    // Le opcao
    char path[PATH_TAM];
    int n;
    scanf(" %s %d", path, &n);

    // Le o que deve atualizar (Guarda duplas campo-valor no TAD criterio)
    int* rrnsAtualizar = malloc(n * sizeof(int));
    if (!rrnsAtualizar) {  // Falha ao criar vetor de inteiros
        printf("Falha no processamento do arquivo.\n");
        return;
    }

    Criterio** novosValores = malloc(n * sizeof(Criterio*));
    if (!novosValores) {  // Falha ao criar vetor de criterios
        free(rrnsAtualizar);
        printf("Falha no processamento do arquivo.\n");
        return;
    }

    for (i = 0; i < n; i++) {
        scanf("%d ", &rrnsAtualizar[i]);  // Le rrn
        novosValores[i] = criterio_criarDoStdin();  // Le duplas campo-valor

        if (!novosValores[i]) {  // Falha ao criar criterio
            free(rrnsAtualizar);
            for (i--; i >= 0; i--) criterio_apagar(&novosValores[i]);
            free(novosValores);

            printf("Falha no processamento do arquivo.\n");
            return;
        }
    }

    // Verifica Cabecalho
    Cabecalho* cab = binario_getCabecalho(path);
    if (!cab) {  // Falha ao ler registro cabecalho
        free(rrnsAtualizar);
        for (i = 0; i < n; i++) criterio_apagar(&novosValores[i]);
        free(novosValores);

        printf("Falha no processamento do arquivo.\n");
        return;
    }

    if (!cabecalho_getStatus(cab)) {
        cabecalho_apagar(&cab);
        free(rrnsAtualizar);
        for (i = 0; i < n; i++) criterio_apagar(&novosValores[i]);
        free(novosValores);

        printf("Falha no processamento do arquivo.\n");
        return;
    }

    // Marca arquivo como insconsistente
    cabecalho_setStatus(cab, false);
    bool ok = binario_setCabecalho(path, cab);
    if (!ok) {  // Falha ao modificar cabecalho
        cabecalho_apagar(&cab);
        free(rrnsAtualizar);
        for (i = 0; i < n; i++) criterio_apagar(&novosValores[i]);
        free(novosValores);

        printf("Falha no processamento do arquivo.\n");
        return;
    }

    // Atualiza registros
    Binario* bin = binario_abrirEscrita(path);
    if (!bin) {  // Falha ao abrir arquivo
        cabecalho_apagar(&cab);
        free(rrnsAtualizar);
        for (i = 0; i < n; i++) criterio_apagar(&novosValores[i]);
        free(novosValores);

        printf("Falha no processamento do arquivo.\n");
        return;
    }

    bool erro;
    Registro* reg;
    int numRegs = cabecalho_getRRN(cab);
    for (i = 0; i < n; i++) {
        if (rrnsAtualizar[i] < 0 || rrnsAtualizar[i] >= numRegs) continue;  // Verifica RRN

        reg = binario_buscar(bin, rrnsAtualizar[i], &erro);  // Le registro
        binario_apontar(bin, -1, SEEK_CUR);  // Volta para registro no RRN rrns[i]

        if (erro) {  // Erro ao alocar
            cabecalho_apagar(&cab);
            free(rrnsAtualizar);
            for (i = 0; i < n; i++) criterio_apagar(&novosValores[i]);
            free(novosValores);
            binario_fechar(&bin);

            printf("Falha no processamento do arquivo.\n");
            return;
        }

        if (!reg) continue;  // Registro removido

        // Atualiza registro
        criterio_atualizarRegistro(&novosValores[i], reg);  // Pega os valores em criterio e colocar no registro
        ok = binario_atualizarRegistro(bin, reg);  // Escreve o registro em cima do anterior
        if (!ok) {
            cabecalho_apagar(&cab);
            registro_apagar(&reg);
            free(rrnsAtualizar);
            for (i = 0; i < n; i++) criterio_apagar(&novosValores[i]);
            free(novosValores);
            binario_fechar(&bin);

            printf("Falha no processamento do arquivo.\n");
            return;
        }

        cabecalho_atualizar(cab);
        registro_apagar(&reg);
    }

    binario_fechar(&bin);
    free(rrnsAtualizar);
    for (i = 0; i < n; i++) criterio_apagar(&novosValores[i]);
    free(novosValores);

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
}
