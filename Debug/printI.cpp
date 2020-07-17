// Utilitario para imprimir o arquivo de indice de forma mais legivel

#include <iostream>
#include "stdio.h"

using namespace std;

struct Chave {
    int id;
    int dado;
};

struct Pagina {
    Chave chaves[5];
    int pont[6];
};

char lc(FILE* f) {
    char c;
    fread(&c, sizeof(char), 1, f);
    return c;
}

int li(FILE* f) {
    int n;
    fread(&n, sizeof(int), 1, f);
    return n;
}

int main(int argc, char const* argv[]) {
    FILE* f;
    f = fopen(argv[1], "rb");

    char c;
    char lixo[54];

    cout << "status: " << lc(f) << ", ";
    cout << "raiz: " << li(f) << ", ";
    cout << "niveis: " << li(f) << ", ";
    cout << "prox: " << li(f) << ", ";
    cout << "chave: " << li(f) << endl;
    fread(&lixo, 1, 55, f);

    for (int line = 0; true; line++) {
        int q = li(f);
        if (feof(f)) break;

        int t = li(f);
        cout << line << ": [nivel: " << q << " n: " << t << "] ";

        Pagina p;
        for (int i = 0; i < 5; i++) p.chaves[i] = {li(f), li(f)};
        for (int i = 0; i < 6; i++) p.pont[i] = li(f);

        cout << p.pont[0];
        for (int i = 0; i < 5; i++) cout << (i < t ? " {" : " (") << p.chaves[i].id << ", " << p.chaves[i].dado << (i < t ? "} " : ") ") << p.pont[i + 1] << " ";
        cout << endl;
    }

    fclose(f);
    return 0;
}
