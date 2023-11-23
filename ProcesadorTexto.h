//
// Created by jalb2 on 3/11/2023.
//

#include <iostream>
#include <string>
#include <fstream>
#include <cctype>
#include <codecvt>
#include <locale>
#include <cstring>
#include <vector>
#include "utils.h"
#include "libstemmer_c/include/libstemmer.h"

using namespace std;

#ifndef ANALISISDEINTENCIONV3_PROCESADORTEXTO_H
#define ANALISISDEINTENCIONV3_PROCESADORTEXTO_H


class ProcesadorTexto {
private:
    int cont;
    u32string textoProcesado;
    vector<u32string> stopwords;
    string textoPostProcesado;

    bool is_in_u32string(char32_t c, const u32string& str) {
        return str.find(c) != string::npos;
    }

    u32string to_u32string(const std::string& str) {
        wstring_convert<codecvt_utf8<char32_t>, char32_t> converter;
        return converter.from_bytes(str);
    }

    void identifyU32Word(const u32string& archive) {
        char32_t c;
        u32string lexema;
        bool bandera = true;


        while (true){
            c = archive[cont];
            c = towlower(c);
            if(is_in_u32string(c, U" \n\t\r,.\"\'()¡¿!?")) break;
            if(bandera) {
                bandera = iswalpha(c);
            }
            lexema += c;
            cont++;
        }

        if(bandera) {
            int index = search(stopwords,lexema);
            if (index == -1) {
                struct sb_stemmer *stemmer = sb_stemmer_new("spanish", NULL);
                string utf8_lexema = wstring_convert<codecvt_utf8<char32_t>, char32_t>{}.to_bytes(lexema);
                const char *word = utf8_lexema.c_str();
                const unsigned char* stemmed_word = sb_stemmer_stem(stemmer, reinterpret_cast<const sb_symbol *>(word), strlen(word));
                string palabra_stemmed(reinterpret_cast<const char*>(stemmed_word));
                sb_stemmer_delete(stemmer);
                textoProcesado += to_u32string(palabra_stemmed);
                textoProcesado += U" ";
            }
        }
    }

    string quitarTilde(char32_t c) {
        if (c == U'á') return "a";
        else if (c == U'é') return "e";
        else if (c == U'í') return "i";
        else if (c == U'ó') return "o";
        else if (c == U'ú' || c == U'ü') return "u";
        else return "nh";
    }

    char replaceSimbols(char32_t c) {
        if (c == U'¿') {
            return  '?';
        } else {
            return '!';
        }
    }

    void crearArchivoProcesado() {
        // Verificar si el archivo existe
        FILE* archivo = fopen("archive.c", "r");
        if (archivo != nullptr) {
            // Sí existe, cerrar el archivo y eliminarlo
            fclose(archivo);
            if (std::remove("archive.c") == 0) {
                printf("El archivo %s ha sido eliminado.\n", "archive.c");
            } else {
                fprintf(stderr, "Error al intentar eliminar el archivo.\n");
            }
        }
        // Si no existe, crearlo y escribir el contenido
        archivo = fopen("archive.c", "w");
        if (archivo != nullptr) {
            fprintf(archivo, "%s\n", textoPostProcesado.c_str());
            fclose(archivo);
            printf("Se ha creado el archivo %s\n", "archive.c");
        } else {
            fprintf(stderr, "Error al intentar crear el archivo.\n");
        }
    }

public:
    ProcesadorTexto() {
        basic_ifstream<char32_t> fin("data/stopwordsOrdened.txt");
        u32string str{};
        getline(fin, str, U'\0');
        stopwords = split(str, U'\n');
    }

    virtual ~ProcesadorTexto() {

    }

    u32string ProcesarTexto(const u32string& archive) {
        char32_t c;
        cont = 0;
        while (cont < archive.size()) {
            c = archive[cont];
            if(is_in_u32string(c,U"¿¡")) {
                c = replaceSimbols(c);
                textoProcesado += c;
                cont++;
            }
            else if(iswalnum(c)) {
                identifyU32Word(archive);
            } else {
                textoProcesado += c;
                cont++;
            }
        }

        return textoProcesado;
    }

    string PostProcesado(u32string archive) {
        if(textoProcesado.empty()) {
            textoProcesado = ProcesarTexto(archive);
        }

        char32_t c;
        char c_transformed;
        cont = 0;

        while (cont < textoProcesado.size()) {
            c = textoProcesado[cont];

            if (is_in_u32string(c, U"\n\t\r ") && cont == 0)
                cont++;
            else if (is_in_u32string(c, U"áéíóúüñ")) {
                string transform = quitarTilde(c);
                textoPostProcesado += transform;
                cont++;
            } else if (is_in_u32string(c, U"\n\t\r ") && is_in_u32string(textoProcesado[cont - 1], U" \n\t\r")) {
                cont++;
            } else {
                c_transformed = (char) c;
                textoPostProcesado += c_transformed;
                cont++;
            }
        }
        crearArchivoProcesado();
        return textoPostProcesado;
    }
};


#endif //ANALISISDEINTENCIONV3_PROCESADORTEXTO_H
