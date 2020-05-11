#include "registro.h"

struct _registro {
    int idNascimento;      // Int
    int idadeMae;          // Int
    char* dataNascimento;  // string: AAAA-MM-DD
    char sexoBebe;         // '0', '1', ou '2'
    char* estadoMae;       // 2 chars
    char* estadoBebe;      // 2 chars

    // Campos de Tamanho Variável
    char* cidadeMae;   // string, maximo 105
    char* cidadeBebe;  // string, maximo 105
};

Registro* registro_new(int idNascimento,
                       int idadeMae, char* dataNascimento,
                       char sexoBebe,
                       char* estadoMae, char* estadoBebe,
                       char* cidadeMae, char* cidadeBebe) {
    Registro* registro = (Registro*)malloc(sizeof(Registro));
    if (registro) {
        registro->idNascimento = idNascimento;
        registro->idadeMae = idadeMae;
        registro->dataNascimento = dataNascimento;
        registro->sexoBebe = sexoBebe;
        registro->estadoMae = estadoMae;
        registro->estadoBebe = estadoBebe;
        registro->cidadeMae = cidadeMae;
        registro->cidadeBebe = cidadeBebe;
    }
    return registro;
}

Registro* registro_newVazio() {
    Registro* registro = (Registro*)malloc(sizeof(Registro));
    if (registro) {
        registro->idNascimento = INTNULL;
        registro->idadeMae = INTNULL;
        registro->dataNascimento = NULL;
        registro->sexoBebe = '\0';
        registro->estadoMae = NULL;
        registro->estadoBebe = NULL;
        registro->cidadeMae = NULL;
        registro->cidadeBebe = NULL;
    }
    return registro;
}

void registro_del(Registro** registro) {
    // Verifica se objeto ja foi apagado
    if (!registro) return;
    if (!*registro) {
        *registro = NULL;
        return;
    }

    // Free nas strings
    if ((*registro)->dataNascimento) free((*registro)->dataNascimento = NULL);
    if ((*registro)->estadoMae) free((*registro)->estadoMae = NULL);
    if ((*registro)->estadoBebe) free((*registro)->estadoBebe = NULL);
    if ((*registro)->cidadeMae) free((*registro)->cidadeMae = NULL);
    if ((*registro)->cidadeBebe) free((*registro)->cidadeBebe = NULL);

    free((*registro));
    *registro = NULL;
}

void registro_rmv(Registro** registro) {
    // Verifica se registro já foi apagado
    if (!registro) return;
    if (!*registro) {
        *registro = NULL;
        return;
    }

    free(registro);
    *registro = NULL;
}

void registro_imprimir(Registro* registro) {
    if (!registro) {
        printf("Registro Vazio\n");
        return;
    }

    unsigned char acento0 = 0xc3;
    unsigned char acento1 = 0xaa;

    char sexo[50];
    switch (registro->sexoBebe) {
        case '0':
            strcpy(sexo, "IGNORADO");
            break;

        case '1':
            strcpy(sexo, "MASCULINO");
            break;

        case '2':
            strcpy(sexo, "FEMININO");
            break;

        default:
            strcpy(sexo, "-");
    }

    printf("Nasceu em %s/%s, em %s, um beb%c%c de sexo %s.\n",
           (registro->cidadeBebe != NULL ? registro->cidadeBebe : "-"),
           (registro->estadoBebe ? registro->estadoBebe : "-"),
           (registro->dataNascimento ? registro->dataNascimento : "-"),
           acento0, acento1, sexo);
}

//* ============================ *//
//* ===== Getter e Setters ===== *//
//* ============================ *//

int registro_getIdNascimento(Registro* registro) { return registro->idNascimento; }
void registro_setIdNascimento(Registro* registro, int idNascimento) { registro->idNascimento = idNascimento; }

int registro_getIdadeMae(Registro* registro) { return registro->idadeMae; }
void registro_setIdadeMae(Registro* registro, int idadeMae) { registro->idadeMae = idadeMae; }

char* registro_getDataNascimento(Registro* registro) { return registro->dataNascimento; }
void registro_setDataNascimento(Registro* registro, char* dataNascimento) { registro->dataNascimento = dataNascimento; }

char registro_getSexoBebe(Registro* registro) { return registro->sexoBebe; }
void registro_setSexoBebe(Registro* registro, char sexoBebe) { registro->sexoBebe = sexoBebe; }

char* registro_getEstadoMae(Registro* registro) { return registro->estadoMae; }
void registro_setEstadoMae(Registro* registro, char* estadoMae) { registro->estadoMae = estadoMae; }

char* registro_getEstadoBebe(Registro* registro) { return registro->estadoBebe; }
void registro_setEstadoBebe(Registro* registro, char* estadoBebe) { registro->estadoBebe = estadoBebe; }

char* registro_getCidadeMae(Registro* registro) { return registro->cidadeMae; }
void registro_setCidadeMae(Registro* registro, char* cidadeMae) { registro->cidadeMae = cidadeMae; }

char* registro_getCidadeBebe(Registro* registro) { return registro->cidadeBebe; }
void registro_setCidadeBebe(Registro* registro, char* cidadeBebe) { registro->cidadeBebe = cidadeBebe; }
