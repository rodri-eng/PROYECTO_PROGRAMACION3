#include <iostream>
#include <fstream>
#include <vector>
#include <string>
using namespace std;

int main() {
    string archivo_entrada = "C:/Users/Carlos/Downloads/wiki_movie_plots_deduped.csv";
    string archivo_salida  = "C:/Users/Carlos/Downloads/wiki_movie_plots_deduped_copy.csv";

    ifstream entrada(archivo_entrada);
    ofstream salida(archivo_salida);

    if (!entrada.is_open() || !salida.is_open()) {
        cout << "Error al abrir archivos" << endl;
        return 1;
    }
    string linea;

    while (getline(entrada, linea)) {
        vector<string> fila_procesada;
        string celda;
        bool en_comillas = false;
        for (char c : linea) {
            if (c == '"') {
                en_comillas = !en_comillas;
                celda += c;
            }
            else if (c == ',' && !en_comillas) {
                if (celda == "" || celda == "Unknown" || celda == "\"\"" || celda == "\"Unknown\"") {
                    celda = "unknown";
                }
                fila_procesada.push_back(celda);
                celda.clear();
            }
            else {
                celda += c;
            }
        }
        if (celda == "" || celda == "Unknown" || celda == "\"\"" || celda == "\"Unknown\"") {
            celda = "unknown";
        }
        fila_procesada.push_back(celda);


        for (size_t i = 0; i < fila_procesada.size(); i++) {
            salida << fila_procesada[i];
            if (i != fila_procesada.size() - 1) {
                salida << ",";
            }
        }
        salida << "\n";
    }

    entrada.close();
    salida.close();

    cout << "Archivo _copy generado correctamente." << endl;

    return 0;
}