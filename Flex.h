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

    vector<string> cargarData(string data) {
        ifstream archivo(data);
        vector<string> vec;
        if (!archivo.is_open()) {
            std::cerr << "Error al abrir el archivo: " << data << std::endl;
            return vec;
        }

        std::string linea;
        while (std::getline(archivo, linea)) {
            vec.push_back(linea);
        }

        archivo.close();
        return vec;
    }




public:
    Flex() {
        INFOMAR = cargarData("data/infomar.txt");
        CONVENCER = cargarData("data/convencer.txt");
        CONTAR = cargarData("data/contar.txt");
        DESCRIBIR = cargarData("data/describir.txt");

        flex_Text = "%{\n"
                    "#include <stdio.h>\n"
                    "#include <conio.h>\n"
                    "#include \"y.tab.h\"\n"
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
                    "\",\" {return(',');}\n"
                    "\";\" {return(';');}\n"
                    "\"(\" {return('(');}\n"
                    "\")\" {return(')');}\n"
                    "\"!\" {return('!');}\n"
                    "\"?\" {return('?');}\n";

        for(int i = 0; i < INFOMAR.size(); i++) {
            if(i == INFOMAR.size() - 1) {
                flex_Text += "\"" + INFOMAR[i] + "\" {return INFORMAR;}\n";
            } else {
                flex_Text += "\"" + INFOMAR[i] + "\" |\n";
            }
        }

        for(int i = 0; i < CONVENCER.size(); i++) {
            if(i == CONVENCER.size() - 1) {
                flex_Text += "\"" + CONVENCER[i] + "\" {return CONVENCER;}\n";
            } else {
                flex_Text += "\"" + CONVENCER[i] + "\" |\n";
            }
        }

        for(int i = 0; i < CONTAR.size(); i++) {
            if(i == CONTAR.size() - 1) {
                flex_Text += "\"" + CONTAR[i] + "\" {return CONTAR;}\n";
            } else {
                flex_Text += "\"" + CONTAR[i] + "\" |\n";
            }
        }

        for(int i = 0; i < DESCRIBIR.size(); i++) {
            if(i == DESCRIBIR.size() - 1) {
                flex_Text += "\"" + DESCRIBIR[i] + "\" {return DESCRIBIR;}\n";
            } else {
                flex_Text += "\"" + DESCRIBIR[i] + "\" |\n";
            }
        }

        flex_Text += "{ignoraPalabras}* {return IGNORADA;}\n"
                     ". {printf(\"LINEA %d: ERROR LEXICO\\n\",yylineno);}\n"
                     "%%\n"
                     "\n"
                     "int main()\n"
                     "{\n"
                     "int token;\n"
                     "yyin=fopen(\"archive.c\",\"rt\");\n"
                     "FILE *outputFile = fopen(\"output.txt\", \"w\");\n"
                     "if (yyin==NULL)\n"
                     "printf(\"No se puede abrie el archivo\");\n"
                     "else\n"
                     "while((token=yylex())>0) {\n"
                     "printf(\"TOKEN = %d LEXEMA = %s \\n\",token,yytext);\n"
                     "fprintf(outputFile, \"%d\\n\", token);\n"
                     "}\n"
                     "fclose(yyin);\n"
                     "fclose(outputFile);\n"
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
