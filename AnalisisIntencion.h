//
// Created by jalb2 on 23/11/2023.
//
#include <vector>
#include <iostream>
#include <fstream>
#include <map>

using namespace std;

#ifndef ANALISISDEINTENCIONV3_ANALISISINTENCION_H
#define ANALISISDEINTENCIONV3_ANALISISINTENCION_H

class AnalisisIntencion {
private:
    vector<int> tokens;

    vector<int> cargarData(string data) {
        ifstream archivo(data);
        int aux;
        vector<int> vec;
        if (!archivo.is_open()) {
            std::cerr << "Error al abrir el archivo: " << data << std::endl;
            return vec;
        }

        std::string linea;
        while (std::getline(archivo, linea)) {
            aux = stoi(linea);
            vec.push_back(aux);
        }

        archivo.close();
        return vec;
    }
public:
    AnalisisIntencion() {
        tokens = cargarData("output.txt");
    }

    int getIntencionTexto() {
        map<int, int> recuento;

        for (int numero : tokens) {
            recuento[numero]++;
        }

        int maxRecuento = -1;

        for (const auto& par : recuento) {
            if (par.second > maxRecuento && (par.first > 200 && par.first != 258)) {
                maxRecuento = par.first;
            }
        }

        return maxRecuento;
    }
};
#endif //ANALISISDEINTENCIONV3_ANALISISINTENCION_H
