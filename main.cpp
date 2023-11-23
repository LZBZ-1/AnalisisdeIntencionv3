#include <iostream>
#include <string>
#include <locale>
#include <codecvt>
#include <fstream>
#include "ProcesadorTexto.h"
#include "Flex.h"
#include "Bison.h"
#include "AnalisisIntencion.h"

using namespace std;

u32string readFile(const string& filename) {
    basic_ifstream<char32_t> fin(filename);
    u32string str{};
    getline(fin, str, U'\0');
    return str;
}

void excecuteParser() {
    FILE* pipe = popen("gcc lex.yy.c", "r");
    if (!pipe) {
        fprintf(stderr, "Error al ejecutar el comando.\n");
        return;
    }

    char buffer[128];
    while (fgets(buffer, sizeof(buffer), pipe) != NULL) {
        printf("%s", buffer);
    }

    int estado = pclose(pipe);
    if (estado == -1) {
        fprintf(stderr, "Error al cerrar el comando.\n");
    }
}

void excecuteProgram() {
    FILE* pipe = popen("a.exe", "r");
    if (!pipe) {
        fprintf(stderr, "Error al ejecutar el comando.\n");
        return;
    }

    char buffer[128];
    while (fgets(buffer, sizeof(buffer), pipe) != NULL) {
        printf("%s", buffer);
    }

    int estado = pclose(pipe);
    if (estado == -1) {
        fprintf(stderr, "Error al cerrar el comando.\n");
    }
}

int main() {
    ProcesadorTexto PC;
    Flex flex;
    Bison bison;
    AnalisisIntencion AI;
    setlocale(LC_ALL, "es_ES.UTF-8");
    u32string archive = readFile("file.txt");
    u32string textoProcesado = PC.ProcesarTexto(archive);
    string textoPostProcesado = PC.PostProcesado(archive);
    wstring_convert<codecvt_utf8<char32_t>, char32_t> converter;
    cout << converter.to_bytes (archive) << endl;
    cout << "-------------------------" << endl;
    cout << converter.to_bytes(textoProcesado) << endl;
    cout << "-------------------------" << endl;
    cout << textoPostProcesado << endl;
    cout << "-------------------------" << endl;
    bison.createBison();
    bison.excecuteBison();
    flex.createFlex();
    flex.excecuteFlex();
    excecuteParser();
    excecuteProgram();

    int intencion = AI.getIntencionTexto();
    switch (intencion) {
        case 262:
            cout << "La intencion del texto es CONTAR UNA HISTORIA" << endl;
            break;
        case 263:
            cout << "La intencion del texto es INFORMAR" << endl;
            break;
        case 264: cout << "La intencion del texto es DESCRIBIR" << endl;
            break;
        case 265: cout << "La intencion del texto es CONVENCER" << endl;
            break;
        default: cout << "NO SE PUDO OBTENER LA INTENCION DEL TEXTO" << endl;
    }

    return 0;
}
