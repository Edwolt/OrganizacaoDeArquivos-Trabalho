#include <fstream>
#include <iostream>

using namespace std;

struct Chave {
    int id;
    int dado;
};

struct Pagina {
    Chave chaves[5];
    int pont[6];
};

int li(ifstream& f) {
    int n;
    char c;
    for (int i = 0; i < 4; i++) {
        f.read(&c, 1);
        n |= c << i * 8;
    }
    return n;
}

char lc(ifstream& f) {
    char c;
    f.read(&c, 1);
    return c;
}

int main(int argc, char const* argv[]) {
    ifstream f;
    f.open(argv[1], ios::binary);

    char c;

    cout << "status: " << lc(f) << ", ";
    cout << "raiz: " << li(f) << ", ";
    cout << "prox: " << li(f) << ", ";
    cout << "chave: " << li(f) << endl;
    for (int i = 0; i < 54; i++) f.read(&c, 1);

    while (!f.eof()) {
        cout.fill(5);
        cout << "nivel: " << li(f) << " ";

        cout.fill(5);
        cout << "n: " << li(f) << " ";

        Pagina p;
        for (int i = 0; i < 5; i++) {
            Chave k;
            k.id = li(f);
            k.dado = li(f);
            p.chaves[i] = k;
        }

        for (int i = 0; i < 6; i++) {
            p.pont[i] = li(f);
        }

        for (int i = 0; i < 5; i++) {
            cout.fill(5);
            cout << p.pont[i] << " {" << p.chaves[i].id << ", " << p.chaves[i].dado << "} ";
        }
        cout.fill(5);
        cout << p.pont[5];
        cout << endl;
    }

    f.close();

    return 0;
}
