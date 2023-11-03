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
    u32string textoPreProcesado;
    vector<u32string> stopwords;

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
            if(is_in_u32string(c, U" \n\t\r,.\"\'")) break;
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
                textoPreProcesado += to_u32string(palabra_stemmed);
                textoPreProcesado += U" ";
            }
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

    u32string preProcesarTexto(const u32string& archive) {
        char32_t c;
        cont = 0;
        while (cont < archive.size()) {
            c = archive[cont];
            c = towlower(c);
            if(iswalpha(c)) {
                identifyU32Word(archive);
            } else {
                textoPreProcesado += c;
                cont++;
            }
        }

        return textoPreProcesado;
    }
};


#endif //ANALISISDEINTENCIONV3_PROCESADORTEXTO_H
