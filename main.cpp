#include <iostream>
#include <string>
#include <locale>
#include <codecvt>
#include <fstream>
#include "ProcesadorTexto.h"

using namespace std;

u32string readFile(const string& filename) {
    basic_ifstream<char32_t> fin(filename);
    u32string str{};
    getline(fin, str, U'\0');
    return str;
}

int main() {
    ProcesadorTexto PC;
    setlocale(LC_ALL, "es_ES.UTF-8");
    u32string archive = readFile("text.txt");
    u32string textoProcesado = PC.ProcesarTexto(archive);
    string textoPostProcesado = PC.PostProcesado(archive);
    wstring_convert<codecvt_utf8<char32_t>, char32_t> converter;
    cout << converter.to_bytes (archive) << endl;
    cout << "-------------------------" << endl;
    cout << converter.to_bytes(textoProcesado) << endl;
    cout << "-------------------------" << endl;
    cout << textoPostProcesado << endl;

    return 0;
}
