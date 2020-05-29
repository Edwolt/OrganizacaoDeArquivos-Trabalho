#include "opcoes.h"

#define PATH_TAM 100  // Tamanho do nome do arquivo

/**
 * Ponteiro para vetor de funcoes
 * 
 * Cada funcao deve ler os parametros necessarios para execucao do stdin
 */
typedef void Opcao();

//* ============================ *//
//* ===== Metodos Privados ===== *//
//* ============================ *//

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
    char src[PATH_TAM];
    char dest[PATH_TAM];
    scanf(" %s %s", src, dest);

    CSV* csv = csv_abrir(src);
    if (!csv) {  // Falha ao abrir csv
        printf("Falha no carregamento do arquivo.\n");
        return;
    }

    bool ok = binario_gerarDoCSV(dest, csv);
    csv_fechar(&csv);

    if (!ok) {  // Falha ao gerar CSV
        printf("Falha no carregamento do arquivo.\n");
        return;
    }

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
    char path[PATH_TAM];
    scanf(" %s", path);

    int i;  // Iteradores

    bool status;
    int inseridos, removidos;
    int rrn;  // Quantidade de registros de dados no arquivo contando os removidos
    bool ok = binario_getCabecalho(path, &status, &rrn, &inseridos, &removidos, NULL);

    if (!ok) {  // Falha ao ler o registro cabecalho
        printf("Falha no processamento do arquivo.\n");
        return;
    }

    if (!status) {  // Arquivo inconsistente
        printf("Falha no processamento do arquivo.\n");
        return;
    }

    if (inseridos - removidos == 0) {  // Arquivo nao possui dados
        printf("Registro inexistente.\n");
        return;
    }

    Binario* bin = binario_abrirLeitura(path);
    if (!bin) {  // Arquivo nao abriu
        printf("Falha no processamento do arquivo.\n");
        return;
    }

    Registro* registro;
    bool erro;

    for (i = 0; i < rrn; i++) {
        registro = binario_leRegistro(bin, &erro);

        if (erro) {  // Falha ao ler arquivo
            printf("Falha no processamento do arquivo.\n");
            return;
        }

        if (!registro) continue;  // Registro NULL (Ocorreu falha ou registro esta removido)

        registro_imprimir(registro);
        registro_apagar(&registro);
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

    char path[PATH_TAM];
    scanf(" %s", path);

    Criterio* criterio = leCriterio();

    bool status;
    int inseridos, removidos;
    int rrn;  // Quantidade de registros de dados no arquivo contando os removidos
    bool ok = binario_getCabecalho(path, &status, &rrn, &inseridos, &removidos, NULL);

    if (!ok) {  // Ocorreu uma falha ao ler o registro cabecalho
        printf("Falha no processamento do arquivo.\n");
        return;
    }

    if (!status) {  // O arquivo esta inconsistente
        printf("Falha no processamento do arquivo.\n");
        return;
    }

    if (inseridos - removidos == 0) {  // O arquivo nao possui dados
        printf("Registro inexistente.\n");
        return;
    }

    Binario* bin = binario_abrirEscrita(path);
    if (!bin) {  // Falha ao abrir arquivo
        printf("Falha no processamento do arquivo.\n");
        return;
    }

    Registro* registro;
    bool erro;
    bool imprimiu = false;

    for (i = 0; i < rrn; i++) {
        registro = binario_leRegistro(bin, &erro);

        if (erro) {  // Falha ao ler registro
            printf("Falha no processamento do arquivo.\n");
            return;
        }

        if (!registro) continue;

        if (registro_satisfaz(registro, criterio)) {  // TODO
            registro_imprimir(registro);
            imprimiu = true;
        }

        registro_apagar(&registro);
    }

    if (!imprimiu) printf("Registro inexistente.\n");  // Nada foi impresso

    binario_fechar(&bin);
    apagarCriterio(&criterio);
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
    char path[PATH_TAM];
    scanf(" %s", path);

    int rrn;
    scanf("%d", &rrn);

    bool status;
    int rrn_arquivo;  // Quantidade de registros de dados no arquivo contando os removidos
    bool ok = binario_getCabecalho(path, &status, &rrn_arquivo, NULL, NULL, NULL);

    if (!ok) {  // Falha ao ler cabecalho
        printf("Falha no processamento do arquivo.\n");
        return;
    }

    if (!status) {  // Arquivo inconsistente
        printf("Falha no processamento do arquivo.\n");
        return;
    }

    if (0 < rrn || rrn > rrn_arquivo) {  // Registro nao existe
        printf("Falha no processamento do arquivo.\n");
        return;
    }

    Binario* binario = binario_abrirLeitura(path);

    if (!binario) {  // Falha ao abrir arquivo
        printf("Falha no processamento do arquivo.\n");
        return;
    }

    bool erro;
    Registro* registro = binario_buscar(binario, rrn, &erro);

    if (erro) {  // Falha ao ler registro
        printf("Falha no processamento do arquivo.\n");
        return;
    }

    if (registro) {  // registro != NULL
        registro_imprimir(registro);
    } else {
        printf("Registro inexistente.");
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
static void opcao5() {
    int i;  // Iteradores

    char path[PATH_TAM];
    scanf(" %s", path);

    int n;
    scanf(" %d", &n);

    Dupla*** criterios = (Dupla***)malloc(n * sizeof(Dupla**));
    if (!criterios) {  // Falha ao alocar vetor de criterios
        printf("Falha no processamento do arquivo\n");
        return;
    }

    // Le criterios
    int m;
    for (i = 0; i < n; i++) {  // TODO
        scanf("%d", &m);
        criterios[i] = leCriterio(m);
        if (!criterios[i]) {
            for (i--; i >= 0; i--) {
                apagarCriterio(&criterios[i], m);
            }
        }
    }

    bool status;
    int inseridos, removidos;
    int rrn;  // Quantidade de registros de dados no arquivo contando os removidos
    bool ok = binario_getCabecalho(path, &status, &rrn, &inseridos, &removidos, NULL);

    if (!ok) {  // Ocorreu uma falha ao ler o registro cabecalho
        printf("Falha no processamento do arquivo.\n");
        return;
    }

    if (!status) {  // O arquivo esta inconsistente
        printf("Falha no processamento do arquivo.\n");
        return;
    }

    if (inseridos - removidos == 0) {  // O arquivo nao possui dados
        printf("Registro inexistente.\n");
        return;
    }

    Binario* bin = binario_abrirEscrita(path);
    if (!bin) {  // Falha ao abrir arquivo
        printf("Falha no processamento do arquivo.\n");
        return;
    }
    Registro* registro;
    bool erro;
    bool imprimiu = false;

    for (i = 0; i < rrn; i++) {
        registro = binario_leRegistro(bin, &erro);

        if (erro) {  // Falha ao ler registro
            printf("Falha no processamento do arquivo.\n");
            return;
        }

        if (!registro) continue;

        if (registro_satisfaz(registro, duplas, m)) {
            registro_imprimir(registro);
            imprimiu = true;
        }

        registro_apagar(&registro);
    }

    if (!imprimiu) printf("Registro inexistente.\n");  // Nada foi impresso

    binario_fechar(&bin);
    for (i = 0; i < m; i++) dupla_apagar(&duplas[i]);
    free(duplas);
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
    printf("Operação não implementada :(\n");
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
    printf("Operação não implementada :(\n");
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