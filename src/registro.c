#include "registro.h"

#define SEXO_IGNORADO '0'
#define SEXO_MASCULINO '1'
#define SEXO_FEMININO '2'

struct Registro {
    int idNascimento;  // int
    int idadeMae;  // int
    char* dataNascimento;  // string: AAAA-MM-DD
    char sexoBebe;  // Ignorado ('0'), Masculino ('1') ou Feminino ('2')
    char* estadoMae;  // 2 chars (string)
    char* estadoBebe;  // 2 chars (string)

    // Campos de Tamanho Variavel
    char* cidadeMae;  // string (campo de tamanho variavel)
    char* cidadeBebe;  // string (campo de tamanho variavel)
};

Registro* registro_criar(int idNascimento,
                         int idadeMae, char* dataNascimento,
                         char sexoBebe,
                         char* estadoMae, char* estadoBebe,
                         char* cidadeMae, char* cidadeBebe) {
    Registro* registro = malloc(sizeof(Registro));
    if (!registro) return NULL;  // Falha ao criar registro

    registro->idNascimento = idNascimento;
    registro->idadeMae = idadeMae;
    registro->dataNascimento = dataNascimento;
    registro->sexoBebe = sexoBebe;
    registro->estadoMae = estadoMae;
    registro->estadoBebe = estadoBebe;
    registro->cidadeMae = cidadeMae;
    registro->cidadeBebe = cidadeBebe;

    return registro;
}

Registro* registro_criarVazio() {
    return registro_criar(INTNULL,
                          INTNULL, NULL,
                          '\0',
                          NULL, NULL,
                          NULL, NULL);
}

void registro_apagar(Registro** registro) {
    if (!registro || !*registro) return;  // Objeto ja apagado

    // Apagar strings
    string_apagar(&(*registro)->dataNascimento);
    string_apagar(&(*registro)->estadoMae);
    string_apagar(&(*registro)->estadoBebe);
    string_apagar(&(*registro)->cidadeMae);
    string_apagar(&(*registro)->cidadeBebe);

    free(*registro);
    *registro = NULL;
}

Registro* registro_criarDoStdin() {
    Registro* registro = registro_criarVazio();
    if (!registro) return NULL;

    char aux[STR_TAM];

    // Aloca strings
    registro->dataNascimento = string_criar(STR_TAM);
    if (!registro->dataNascimento) {
        return NULL;
    }

    registro->estadoMae = string_criar(STR_TAM);
    if (!registro->estadoMae) {
        string_apagar(&registro->dataNascimento);
        return NULL;
    }

    registro->estadoBebe = string_criar(STR_TAM);
    if (!registro->estadoBebe) {
        string_apagar(&registro->dataNascimento);
        string_apagar(&registro->estadoBebe);
        return NULL;
    }

    registro->cidadeMae = string_criar(STR_TAM);
    if (!registro->dataNascimento) {
        string_apagar(&registro->dataNascimento);
        string_apagar(&registro->estadoBebe);
        string_apagar(&registro->cidadeMae);
        return NULL;
    }

    registro->cidadeBebe = string_criar(STR_TAM);
    if (!registro->dataNascimento) {
        string_apagar(&registro->dataNascimento);
        string_apagar(&registro->estadoBebe);
        string_apagar(&registro->cidadeMae);
        string_apagar(&registro->cidadeBebe);
        return NULL;
    }

    // Lendo do Stdin
    scan_quote_string(registro->cidadeMae);
    string_apagarSeVazio(&registro->cidadeMae);

    scan_quote_string(registro->cidadeBebe);
    string_apagarSeVazio(&registro->cidadeBebe);

    scanf("%d", &registro->idNascimento);  // Tenho certeza que nao e nulo

    scan_quote_string(aux);
    registro->idadeMae = (strlen(aux) != 0 ? atoi(aux) : INTNULL);

    scan_quote_string(registro->dataNascimento);
    string_apagarSeVazio(&registro->dataNascimento);

    scan_quote_string(aux);
    registro->sexoBebe = (strlen(aux) == 1 ? aux[0] : '\0');

    scan_quote_string(registro->estadoMae);
    string_apagarSeVazio(&registro->estadoMae);

    scan_quote_string(registro->estadoBebe);
    string_apagarSeVazio(&registro->estadoBebe);

    return registro;
}

void registro_imprimir(Registro* registro) {
    if (!registro) {  // Objeto nao existe
        printf("Registro Inexistente\n");
        return;
    }

    // String com o sexo do bebe
    char sexo[50];
    switch (registro->sexoBebe) {
        case SEXO_IGNORADO:
            strcpy(sexo, "IGNORADO");
            break;

        case SEXO_MASCULINO:
            strcpy(sexo, "MASCULINO");
            break;

        case SEXO_FEMININO:
            strcpy(sexo, "FEMININO");
            break;

        default:
            strcpy(sexo, "-");
    }

    /*
    O acento tam bytes 0xc3aa
    Em UTF-8 eh o caracter U+00EA
    Para escrever pelo Linux Mint pode se usar ctrl+shift+u seguido do codigo no UTF8 (00EA)
    Em vez de escrever "ê" escreve "ê" 
    */
    // (ptr ? x : y) eh o mesmo que (ptr != NULL? x : y)
    printf("Nasceu em %s/%s, em %s, um bebê de sexo %s.\n",
           (registro->cidadeBebe ? registro->cidadeBebe : "-"),
           (registro->estadoBebe ? registro->estadoBebe : "-"),
           (registro->dataNascimento ? registro->dataNascimento : "-"),
           sexo);
}

void registro_extrairDados(Registro* registro,
                           int* idNascimento,
                           int* idadeMae, char** dataNascimento,
                           char* sexoBebe,
                           char** estadoMae, char** estadoBebe,
                           char** cidadeMae, char** cidadeBebe) {
    if (!registro) return;  // Registro nao existe

    if (idNascimento) *idNascimento = registro->idNascimento;
    if (idadeMae) *idadeMae = registro->idadeMae;
    if (dataNascimento) *dataNascimento = registro->dataNascimento;
    if (sexoBebe) *sexoBebe = registro->sexoBebe;
    if (estadoMae) *estadoMae = registro->estadoMae;
    if (estadoBebe) *estadoBebe = registro->estadoBebe;
    if (cidadeMae) *cidadeMae = registro->cidadeMae;
    if (cidadeBebe) *cidadeBebe = registro->cidadeBebe;
}

//* ============================= *//
//* ===== Getters e Setters ===== *//
//* ============================= *//

int registro_getIdNascimento(Registro* registro) {
    if (!registro) return INTNULL;
    return registro->idNascimento;
}
void registro_setIdNascimento(Registro* registro, int idNascimento) {
    if (registro) registro->idNascimento = idNascimento;
}

int registro_getIdadeMae(Registro* registro) {
    if (!registro) return INTNULL;
    return registro->idadeMae;
}
void registro_setIdadeMae(Registro* registro, int idadeMae) {
    if (registro) registro->idadeMae = idadeMae;
}

char* registro_getDataNascimento(Registro* registro) {
    if (!registro) return NULL;
    return registro->dataNascimento;
}
void registro_setDataNascimento(Registro* registro, char* dataNascimento) {
    if (!registro) return;
    string_apagar(&registro->dataNascimento);
    registro->dataNascimento = dataNascimento;
}

char registro_getSexoBebe(Registro* registro) {
    if (!registro) return '\0';
    return registro->sexoBebe;
}
void registro_setSexoBebe(Registro* registro, char sexoBebe) {
    if (registro) registro->sexoBebe = sexoBebe;
}

char* registro_getEstadoMae(Registro* registro) {
    if (!registro) return NULL;
    return registro->estadoMae;
}
void registro_setEstadoMae(Registro* registro, char* estadoMae) {
    if (!registro) return;
    string_apagar(&registro->estadoMae);
    registro->estadoMae = estadoMae;
}

char* registro_getEstadoBebe(Registro* registro) {
    if (!registro) return NULL;
    return registro->estadoBebe;
}
void registro_setEstadoBebe(Registro* registro, char* estadoBebe) {
    if (!registro) return;
    string_apagar(&registro->estadoBebe);
    registro->estadoBebe = estadoBebe;
}

char* registro_getCidadeMae(Registro* registro) {
    if (!registro) return NULL;
    return registro->cidadeMae;
}
void registro_setCidadeMae(Registro* registro, char* cidadeMae) {
    if (!registro) return;
    string_apagar(&registro->cidadeMae);
    registro->cidadeMae = cidadeMae;
}

char* registro_getCidadeBebe(Registro* registro) {
    if (!registro) return NULL;
    return registro->cidadeBebe;
}
void registro_setCidadeBebe(Registro* registro, char* cidadeBebe) {
    if (!registro) return;
    string_apagar(&registro->cidadeBebe);
    registro->cidadeBebe = cidadeBebe;
}