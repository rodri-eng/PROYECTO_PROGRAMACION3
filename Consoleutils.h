#pragma once
#include <iostream>
#include <string>
#include <stdexcept>

inline void limpiarPantalla() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

inline void pausar() {
    std::cout << "\nPresione Enter para continuar...";
    std::string dummy;
    std::getline(std::cin, dummy);
}

inline int leerOpcion(int min, int max) {
    while (true) {
        std::cout << "Opcion [" << min << "-" << max << "]: ";
        std::string linea;
        std::getline(std::cin, linea);

        if (linea.empty()) {
            std::cout << "  [!] No ingreso nada. Intente de nuevo.\n";
            continue;
        }

        try {
            std::size_t pos;
            int valor = std::stoi(linea, &pos);

            // Rechaza entradas mixtas como "2abc"
            if (pos != linea.size()) {
                std::cout << "  [!] Entrada invalida. Solo numeros enteros.\n";
                continue;
            }

            if (valor < min || valor > max) {
                std::cout << "  [!] Fuera de rango. Ingrese entre "
                          << min << " y " << max << ".\n";
                continue;
            }

            return valor;

        } catch (const std::invalid_argument&) {
            std::cout << "  [!] Entrada invalida. Ingrese un numero.\n";
        } catch (const std::out_of_range&) {
            std::cout << "  [!] Numero demasiado grande. Intente de nuevo.\n";
        }
    }
}

inline std::string leerTexto() {
    while (true) {
        std::string texto;
        std::getline(std::cin, texto);

        // Trim: elimina espacios/tabs al inicio y al final
        const std::string ws = " \t\r\n";
        size_t inicio = texto.find_first_not_of(ws);
        size_t fin    = texto.find_last_not_of(ws);

        if (inicio == std::string::npos) {
            std::cout << "  [!] El texto no puede estar vacio: ";
            continue;
        }

        return texto.substr(inicio, fin - inicio + 1);
    }
}