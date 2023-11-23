//
// Created by jalb2 on 22/11/2023.
//
#include <string>
#include <stdio.h>
#include <vector>

using namespace std;

#ifndef ANALISISDEINTENCIONV3_FLEX_H
#define ANALISISDEINTENCIONV3_FLEX_H

class Flex {
private:
    string flex_Text;
    const char *const command = "flex analisis.l";
    vector<string> INFOMAR, CONVENCER, DESCRIBIR, CONTAR;
public:
    Flex() {
        flex_Text = "%{\n"
                    "#include <stdio.h>\n"
                    "#include <conio.h>\n"
                    "\n"
                    "#define INFORMAR 300\n"
                    "#define CONVENCER 301\n"
                    "#define DESCRIBIR 302\n"
                    "#define CONTAR 303\n"
                    "%}\n"
                    "\n"
                    "ignoraPalabras [a-z]*\n"
                    "\n"
                    "%option noyywrap\n"
                    "%option yylineno\n"
                    "\n"
                    "ignora \" \"|\\t|\\n|\\r\n"
                    "\n"
                    "%%\n"
                    "{ignora}* {;}\n"
                    "\"\\\"\" {return('\\\"');}\n"
                    "\"\\'\" {return('\\'');}\n"
                    "\".\" {return('.');}\n"
                    "\"anunci\" |\n"
                    "\"promocion\" |\n"
                    "\"line\" {return CONVENCER;}\n"
                    "{ignoraPalabras}* {;}\n"
                    ". {printf(\"LINEA %d: ERROR LEXICO\\n\",yylineno);}\n"
                    "%%\n"
                    "\n"
                    "int main()\n"
                    "{\n"
                    "int token;\n"
                    "yyin=fopen(\"archive.c\",\"rt\");\n"
                    "if (yyin==NULL)\n"
                    "printf(\"No se puede abrie el archivo %s\",\"archive.c\");\n"
                    "else\n"
                    "while((token=yylex())>0)\n"
                    "printf(\"TOKEN = %d LEXEMA = %s \\n\",token,yytext);\n"
                    "\n"
                    "fclose(yyin);\n"
                    "getch();\n"
                    "return(0);\n"
                    "}";
    }

    const string &getFlexText() const {
        return flex_Text;
    }

    void createFlex() {
        // Verificar si el archivo existe
        FILE* archivo = fopen("analisis.l", "r");
        if (archivo != nullptr) {
            // Sí existe, cerrar el archivo y eliminarlo
            fclose(archivo);
            if (std::remove("analisis.l") == 0) {
                printf("El archivo %s ha sido eliminado.\n", "analisis.l");
            } else {
                fprintf(stderr, "Error al intentar eliminar el archivo.\n");
            }
        }
        // Si no existe, crearlo y escribir el contenido
        archivo = fopen("analisis.l", "w");
        if (archivo != nullptr) {
            fprintf(archivo, "%s\n", flex_Text.c_str());
            fclose(archivo);
            printf("Se ha creado el archivo %s\n", "analisis.l");
        } else {
            fprintf(stderr, "Error al intentar crear el archivo.\n");
        }
    }

    void excecuteFlex() {
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

#endif //ANALISISDEINTENCIONV3_FLEX_H
