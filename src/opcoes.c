#include "opcoes.h"

#define PATH_TAM 100  // Tamanho do nome do arquivo

/**
 * Ponteiro para vetor de funcoes
 * 
 * Cada funcao deve ler os parametros necessarios para execucao do stdin
 */
typedef void Opcao();

//* ================== *//
//* ===== Opcoes ===== *//
//* ================== *//

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

    // Marca arquivo como insconsistente
    bool status = false;
    bool ok = binario_setCabecalho(dest, &status, NULL, NULL, NULL, NULL);
    if (!ok) {  // Falha ao modificar cabecalho
        printf("Falha no carregamento do arquivo.\n");
        return;
    }

    // Leitura e Escrita dos registro
    Registro* reg;

    CSV* csv = csv_abrir(src);
    if (!csv) {  // Falha ao abrir arquivo
        printf("Falha no carregamento do arquivo.\n");
        return;
    }

    bin = binario_abrirEscrita(dest);
    if (!bin) {  // Falha ao abrir arquivo
        csv_fechar(&csv);

        printf("Falha no carregamento do arquivo.\n");
        return;
    }

    int cont = 0;  // Numero de registro escritos
    while ((reg = csv_lerRegistro(csv))) {  // Enquanto houver registros para ler
        ok = binario_escreverRegistro(bin, reg);
        if (!ok) {  // Falha ao escrever registro
            registro_apagar(&reg);
            csv_fechar(&csv);
            binario_fechar(&bin);

            printf("Falha no carregamento do arquivo.\n");
            return;
        }

        registro_apagar(&reg);
        cont++;
    }

    csv_fechar(&csv);
    binario_fechar(&bin);

    // Marca arquivo como consistente e atualiza cabecalho
    status = true;
    ok = binario_setCabecalho(dest, &status, &cont, &cont, NULL, NULL);
    if (!ok) {  // Falha ao modificar cabecalho
        printf("Falha no carregamento do arquivo.\n");
        return;
    }

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
    bool status;
    int rrn, inseridos;
    bool ok = binario_getCabecalho(path, &status, &rrn, &inseridos, NULL, NULL);

    /*
    ok: falha o ler cabecalho
    status: arquivo inconsistente
    */

    if (!ok || !status) {
        printf("Falha no processamento do arquivo.\n");
        return;
    }

    if (inseridos == 0) {  // Arquivo nao possui dados
        printf("Registro inexistente.\n");
        return;
    }

    // Le e imprime registros
    Binario* bin = binario_abrirLeitura(path);
    if (!bin) {  // Arquivo nao abriu
        printf("Falha no processamento do arquivo.\n");
        return;
    }

    Registro* reg;
    bool erro;
    for (i = 0; i < rrn; i++) {
        reg = binario_lerRegistro(bin, &erro);

        if (erro) {  // Falha ao ler arquivo
            binario_fechar(&bin);

            printf("Falha no processamento do arquivo.\n");
            return;
        }

        if (!reg) continue;  // Registro NULL (Ocorreu falha ou registro esta removido)

        registro_imprimir(reg);
        registro_apagar(&reg);
    }

    binario_fechar(&bin);
}

/**
 * 3 bin m campo[1] valor[1] ... campo[m] valor[m]
 * 
 * bin: arquivo binario de registros
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
    bool status;
    int rrn, inseridos;
    bool ok = binario_getCabecalho(path, &status, &rrn, &inseridos, NULL, NULL);

    /*
    ok: falha o ler cabecalho
    status: arquivo inconsistente
    */

    if (!ok || !status) {
        criterio_apagar(&criterio);

        printf("Falha no processamento do arquivo.\n");
        return;
    }

    if (inseridos == 0) {  // O arquivo nao possui dados
        criterio_apagar(&criterio);

        printf("Registro Inexistente.\n");
        return;
    }

    // Busca no arquivo
    Binario* bin = binario_abrirLeitura(path);
    if (!bin) {  // Falha ao abrir arquivo
        criterio_apagar(&criterio);

        printf("Falha no processamento do arquivo.\n");
        return;
    }

    Registro* reg;
    bool erro;
    bool imprimiu = false;
    for (i = 0; i < rrn; i++) {
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
    bool status;
    int rrn_arquivo;
    bool ok = binario_getCabecalho(path, &status, &rrn_arquivo, NULL, NULL, NULL);

    /*
    ok: falha o ler cabecalho
    status: arquivo inconsistente
    */

    if (!ok || !status) {
        printf("Falha no processamento do arquivo.\n");
        return;
    }

    if (rrn <= 0 || rrn > rrn_arquivo) {  // Registro nao existe
        printf("Registro Inexistente.");
        return;
    }

    // Le e imprime registro
    Binario* bin = binario_abrirLeitura(path);

    if (!bin) {  // Falha ao abrir arquivo
        printf("Falha no processamento do arquivo.\n");
        return;
    }

    bool erro;
    // rrn + 1 porque o rrn recebido do stdin desconsidera a existencia do registro cabecalho
    Registro* reg = binario_buscar(bin, rrn, &erro);

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

    binario_fechar(&bin);
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
static void opcao5() {
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
            return;
        }
    }

    // Verifica cabecalho
    bool status;
    int rrn, inseridos, removidos;
    bool ok = binario_getCabecalho(path, &status, &rrn, &inseridos, &removidos, NULL);

    /*
    ok: falha o ler cabecalho
    status: arquivo inconsistente
    */
    if (!ok || !status) {
        for (i = 0; i < n; i++) criterio_apagar(&criterios[i]);
        free(criterios);

        printf("Falha no processamento do arquivo.\n");
        return;
    }

    if (inseridos == 0) {  // O arquivo nao possui dados
        for (i = 0; i < n; i++) criterio_apagar(&criterios[i]);
        free(criterios);

        printf("Registro Inexistente.\n");
        return;
    }

    // Marca arquivo como inconsistente
    status = false;
    ok = binario_setCabecalho(path, &status, NULL, NULL, NULL, NULL);
    if (!ok) {  // Falha ao modificar cabecalho
        for (i = 0; i < n; i++) criterio_apagar(&criterios[i]);
        free(criterios);

        printf("Falha no carregamento do arquivo.\n");
        return;
    }

    // Remove registros
    Binario* bin = binario_abrirEscrita(path);

    if (!bin) {  // Falha ao abrir arquivo
        for (i = 0; i < n; i++) criterio_apagar(&criterios[i]);
        free(criterios);

        printf("Falha no processamento do arquivo.\n");
        return;
    }

    bool erro;
    bool apagou = false;
    Registro* reg;

    for (i = 0; i < rrn; i++) {  // Itera sobre registro
        reg = binario_lerRegistro(bin, &erro);

        if (erro) {  // Falha ao ler registro
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
                removidos++;
                inseridos--;
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
    status = true;
    ok = binario_setCabecalho(path, &status, NULL, &inseridos, &removidos, NULL);
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
    Criterio* criterio = NULL;
    criterio_apagar(&criterio);
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
    bool status;
    int rrn, inseridos;
    bool ok = binario_getCabecalho(path, &status, &rrn, &inseridos, NULL, NULL);

    /*
    ok: falha o ler cabecalho
    status: arquivo inconsistente
    */
    if (!ok || !status) {
        for (i = 0; i < n; i++) {
            registro_apagar(&regs[i]);
        }
        printf("Falha no processamento do arquivo.\n");
        return;
    }

    // Marca arquivo como inconsistente
    status = false;
    ok = binario_setCabecalho(path, &status, NULL, NULL, NULL, NULL);
    if (!ok) {  // Falha ao modificar cabecalho
        for (i = 0; i < n; i++) {
            registro_apagar(&regs[i]);
        }
        printf("Falha no processamento do arquivo.\n");
        return;
    }

    // Insere registros no arquivo
    Binario* bin = binario_abrirEscrita(path);
    if (!bin) {
        for (i = 0; i < n; i++) registro_apagar(&regs[i]);
        free(regs);

        printf("Falha no processamento do arquivo.\n");
        return;
    }
    binario_apontar(bin, rrn, SEEK_SET);

    ok = binario_inserir(bin, regs, n);
    for (i = 0; i < n; i++) registro_apagar(&regs[i]);
    free(regs);

    if (!ok) {  // Falha ao inserir no arquivo
        binario_fechar(&bin);
        printf("Falha no processamento do arquivo.\n");
        return;
    }

    binario_fechar(&bin);

    // Atualiza Cabecalho
    status = true;
    rrn += n;
    inseridos += n;
    ok = binario_setCabecalho(path, &status, &rrn, &inseridos, NULL, NULL);
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
static void opcao7() {
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
    bool status;
    int rrn, atualizados;
    bool ok = binario_getCabecalho(path, &status, &rrn, NULL, NULL, &atualizados);

    /*
    ok: falha o ler cabecalho
    status: arquivo inconsistente
    */
    if (!ok || !status) {
        free(rrnsAtualizar);
        for (i = 0; i < n; i++) criterio_apagar(&novosValores[i]);
        free(novosValores);

        printf("Falha no processamento do arquivo.\n");
        return;
    }

    // Marca arquivo como insconsistente
    status = false;
    ok = binario_setCabecalho(path, &status, NULL, NULL, NULL, NULL);
    if (!ok) {  // Falha ao modificar cabecalho
        free(rrnsAtualizar);
        for (i = 0; i < n; i++) criterio_apagar(&novosValores[i]);
        free(novosValores);

        printf("Falha no processamento do arquivo.\n");
        return;
    }

    // Atualiza registros
    Binario* bin = binario_abrirEscrita(path);
    if (!bin) {  // Falha ao abrir arquivo
        free(rrnsAtualizar);
        for (i = 0; i < n; i++) criterio_apagar(&novosValores[i]);
        free(novosValores);

        printf("Falha no processamento do arquivo.\n");
        return;
    }

    bool erro;
    Registro* reg;
    for (i = 0; i < n; i++) {
        if (rrnsAtualizar[i] < 0 || rrnsAtualizar[i] >= rrn) continue;  // Verifica RRN

        reg = binario_buscar(bin, rrnsAtualizar[i], &erro);  // Le registro
        binario_apontar(bin, -1, SEEK_CUR);  // Volta para registro no RRN rrns[i]

        if (erro) {  // Erro ao alocar
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
            registro_apagar(&reg);
            free(rrnsAtualizar);
            for (i = 0; i < n; i++) criterio_apagar(&novosValores[i]);
            free(novosValores);
            binario_fechar(&bin);

            printf("Falha no processamento do arquivo.\n");
            return;
        }

        atualizados++;
        registro_apagar(&reg);
    }

    binario_fechar(&bin);
    free(rrnsAtualizar);
    for (i = 0; i < n; i++) criterio_apagar(&novosValores[i]);
    free(novosValores);

    // Atualiza cabecalho
    status = true;
    ok = binario_setCabecalho(path, &status, NULL, NULL, NULL, &atualizados);
    if (!ok) {  // Falha ao modificar cabecalho
        printf("Falha no processamento do arquivo.\n");
        return;
    }

    // Imprime resultado
    binarioNaTela(path);
}

/**
 * Vetor com todas as funcoes de cada uma das opcoes para facilitar
 * Usei isso para facilitar a execucao da opcao apena usando o indice dela
 */
static Opcao* funcOpcoes[] = {opcao1,
                              opcao2,
                              opcao3,
                              opcao4,
                              opcao5,
                              opcao6,
                              opcao7};

//* ============================ *//
//* ===== Metodos Publicos ===== *//
//* ============================ *//

bool opcoes_executar(int n) {
    int length = (sizeof(funcOpcoes) / sizeof(Opcao*));  // Tamanho do vetor de funcoes
    if (0 < n && n <= length) {
        (*funcOpcoes[n - 1])();  // Executa funcOpoes na com valor opcao
        return true;
    }
    return false;
}