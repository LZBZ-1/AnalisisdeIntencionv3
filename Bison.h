//
// Created by jalb2 on 22/11/2023.
//
#include <stdio.h>
#include <string>

using namespace std;

#ifndef ANALISISDEINTENCIONV3_BISON_H
#define ANALISISDEINTENCIONV3_BISON_H

class Bison {
private:
    string bison_text;
    const char *const command = "bison -dy analisis.y";
public:
    Bison() {
        bison_text = "%{\n"
                    "    int yystopparser=0;\n"
                    "%}\n"
                    "\n"
                    "%start sentence\n"
                    "%token IGNORADA\n"
                    "%token INFORMAR\n"
                    "%token CONVENCER\n"
                    "%token DESCRIBIR\n"
                    "%token CONTAR\n"
                    "\n"
                    "%%\n"
                    "sentence                    :   expresion terminal\n"
                    "                            |   expresion '.'\n"
                    "                            ;\n"
                    "\n"
                    "expresion                   :   symbol\n"
                    "                            |   separator\n"
                    "                            |   terminal\n"
                    "                            |   word\n"
                    "                            ;\n"
                    "\n"
                    "symbol                      :   '(' content ')'\n"
                    "                            |   '\\'' content '\\''\n"
                    "                            |   '\"' content '\"'\n"
                    "                            ;\n"
                    "\n"
                    "separator                   :   ','\n"
                    "                            |   ';'\n"
                    "                            |   '.'\n"
                    "                            ;\n"
                    "\n"
                    "terminal                    :   '!' content '!'\n"
                    "                            |   '?' content '?'\n"
                    "                            ;\n"
                    "\n"
                    "content                     :   expresion\n"
                    "                            |   expresion content\n"
                    "                            ;\n"
                    "\n"
                    "\n"
                    "word                        :   INFORMAR\n"
                    "                            |   CONVENCER\n"
                    "                            |   DESCRIBIR\n"
                    "                            |   CONTAR\n"
                    "                            |   IGNORADA\n"
                    "                            ; \n"
                    "%%";
    }

    const string &getBisonText() const {
        return bison_text;
    }

    void createBison() {
        // Verificar si el archivo existe
        FILE* archivo = fopen("analisis.y", "r");
        if (archivo != nullptr) {
            // Sí existe, cerrar el archivo y eliminarlo
            fclose(archivo);
            if (std::remove("analisis.y") == 0) {
                printf("El archivo %s ha sido eliminado.\n", "analisis.y");
            } else {
                fprintf(stderr, "Error al intentar eliminar el archivo.\n");
            }
        }
        // Si no existe, crearlo y escribir el contenido
        archivo = fopen("analisis.y", "w");
        if (archivo != nullptr) {
            fprintf(archivo, "%s\n", bison_text.c_str());
            fclose(archivo);
            printf("Se ha creado el archivo %s\n", "analisis.y");
        } else {
            fprintf(stderr, "Error al intentar crear el archivo.\n");
        }
    }

    void excecuteBison() {
        FILE* pipe = popen(command, "r");
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
};


#endif //ANALISISDEINTENCIONV3_BISON_H
