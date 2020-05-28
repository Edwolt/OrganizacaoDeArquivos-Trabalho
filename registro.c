#include "registro.h"

#define SEXO_IGNORADO '0'
#define SEXO_MASCULINO '1'
#define SEXO_FEMININO '2'

struct _registro {
    int idNascimento;  // int
    int idadeMae;  // int
    char* dataNascimento;  // string: AAAA-MM-DD
    char sexoBebe;  // Ignorado ('0'), Masculino ('1'), ou Feminino ('2')
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

Registro* registro_criarVazio() {
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

void registro_apagar(Registro** registro) {
    // Verifica se objeto ja foi apagado
    if (!registro || !*registro) return;

    // Free nas strings
    if ((*registro)->dataNascimento) free((*registro)->dataNascimento = NULL);
    if ((*registro)->estadoMae) free((*registro)->estadoMae = NULL);
    if ((*registro)->estadoBebe) free((*registro)->estadoBebe = NULL);
    if ((*registro)->cidadeMae) free((*registro)->cidadeMae = NULL);
    if ((*registro)->cidadeBebe) free((*registro)->cidadeBebe = NULL);

    free((*registro));
    *registro = NULL;
}

bool registro_satisfaz(Registro* registro, Dupla** duplas, int n) {
    if (!registro || !duplas) return NULL;
    int i;  // Iteradores

    for (i = 0; i < n; i++) {
        switch (dupla_getCampo(duplas[i])) {
            case DUPLA_IDNASCIMENTO:
                if (registro->idNascimento != dupla_getInteiro(duplas[i])) return false;
                break;

            case DUPLA_IDADEMAE:
                if (registro->idadeMae != dupla_getInteiro(duplas[i])) return false;
                break;

            // Campo Char
            case DUPLA_SEXOBEBE:
                if (registro->sexoBebe != dupla_getCaracter(duplas[i])) return false;
                break;

            // Campo String
            case DUPLA_DATANASCIMENTO:
                // Verifica se uma dos dois, mas nao ambos, eh NULL
                // (dupla == NULL) xor (campo == NULL)
                if ((!dupla_getString(duplas[i]) != !registro->dataNascimento)) return false;
                if (!registro->dataNascimento) continue;  // Ambos sao nulos

                if (strcmp(registro->dataNascimento, dupla_getString(duplas[i])) != 0) return false;
                break;

            case DUPLA_ESTADOMAE:
                // Verifica se uma dos dois, mas não ambos, eh NULL
                // (dupla == NULL) xor (campo == NULL)
                if ((!dupla_getString(duplas[i]) != !registro->estadoMae)) return false;
                if (!registro->estadoMae) continue;  // Ambos sao nulos

                if (strcmp(registro->estadoMae, dupla_getString(duplas[i])) != 0) return false;
                break;

            case DUPLA_ESTADOBEBE:
                // Verifica se uma dos dois, mas não ambos, eh NULL
                // (dupla == NULL) xor (campo == NULL)
                if ((!dupla_getString(duplas[i]) != !registro->estadoBebe)) return false;
                if (!registro->estadoBebe) continue;  // Ambos sao nulos

                if (strcmp(registro->estadoBebe, dupla_getString(duplas[i])) != 0) return false;
                break;

            case DUPLA_CIDADEMAE:
                // Verifica se uma dos dois, mas não ambos, eh NULL
                // (dupla == NULL) xor (campo == NULL)
                if ((!dupla_getString(duplas[i]) != !registro->cidadeMae)) return false;
                if (!registro->cidadeMae) continue;  // Ambos sao nulos

                if (strcmp(registro->cidadeMae, dupla_getString(duplas[i])) != 0) return false;
                break;

            case DUPLA_CIDADEBEBE:
                // Verifica se uma dos dois, mas não ambos, eh NULL
                // (dupla == NULL) xor (campo == NULL)
                if ((!dupla_getString(duplas[i]) != !registro->cidadeBebe)) return false;
                if (!registro->cidadeBebe) continue;  // Ambos sao nulos

                if (strcmp(registro->cidadeBebe, dupla_getString(duplas[i])) != 0) return false;
                break;

            default:
                return false;
        }
    }

    return true;
}

void registro_imprimir(Registro* registro) {
    if (!registro) {  // Verifica se o registro existe
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

    // O acento tam bytes 0xc3aa
    // Em UTF-8 eh o caracter U+00EA
    // Para escrever pelo Linux Mint pode se usar ctrl+shift+u seguido do codigo no UTF8 (00EA)
    // (ptr ? x : y) eh o mesmo que (ptr != NULL? x : y)
    printf("Nasceu em %s/%s, em %s, um bebê de sexo %s.\n",
           (registro->cidadeBebe ? registro->cidadeBebe : "-"),
           (registro->estadoBebe ? registro->estadoBebe : "-"),
           (registro->dataNascimento ? registro->dataNascimento : "-"),
           sexo);
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
    if (registro) registro->dataNascimento = dataNascimento;
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
    if (registro) registro->estadoMae = estadoMae;
}

char* registro_getEstadoBebe(Registro* registro) {
    if (!registro) return NULL;
    return registro->estadoBebe;
}
void registro_setEstadoBebe(Registro* registro, char* estadoBebe) {
    if (registro) registro->estadoBebe = estadoBebe;
}

char* registro_getCidadeMae(Registro* registro) {
    if (!registro) return NULL;
    return registro->cidadeMae;
}
void registro_setCidadeMae(Registro* registro, char* cidadeMae) {
    if (registro) registro->cidadeMae = cidadeMae;
}

char* registro_getCidadeBebe(Registro* registro) {
    if (!registro) return NULL;
    return registro->cidadeBebe;
}
void registro_setCidadeBebe(Registro* registro, char* cidadeBebe) {
    if (registro) registro->cidadeBebe = cidadeBebe;
}
