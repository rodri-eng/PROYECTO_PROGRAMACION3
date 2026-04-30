#pragma once
#include <iostream>
#include <string>
#include <vector>
#include "ConsoleUtils.h"
#include "MotorBusqueda.h"

enum EstadoPantalla {
    MENU_PRINCIPAL,
    BUSCAR,
    RESULTADOS,
    VER_MAS_TARDE,
    SALIR
};

// ── Menu Principal ────────────────────────────────────────────────────────────
inline EstadoPantalla vistaMenuPrincipal() {
    limpiarPantalla();
    std::cout << "========================================\n";
    std::cout << "        BUSCADOR DE PELICULAS           \n";
    std::cout << "========================================\n";
    std::cout << "  [1] Buscar pelicula\n";
    std::cout << "  [2] Ver mas tarde\n";
    std::cout << "  [0] Salir\n";
    std::cout << "----------------------------------------\n";

    switch (leerOpcion(0, 2)) {
        case 1:  return BUSCAR;
        case 2:  return VER_MAS_TARDE;
        default: return SALIR;
    }
}

// ── Buscar — rellena 'consulta' por referencia ────────────────────────────────
inline EstadoPantalla vistaBuscar(std::string& consulta) {
    limpiarPantalla();
    std::cout << "========================================\n";
    std::cout << "             BUSCAR                     \n";
    std::cout << "========================================\n";
    std::cout << "  [1] Por palabra clave\n";
    std::cout << "  [2] Por tag / genero\n";
    std::cout << "  [0] Volver\n";
    std::cout << "----------------------------------------\n";

    int op = leerOpcion(0, 2);
    if (op == 0) return MENU_PRINCIPAL;

    std::cout << (op == 1 ? "Ingrese palabra clave: " : "Ingrese tag / genero: ");
    consulta = leerTexto();
    std::cout << "\nBuscando \"" << consulta << "\"...\n";
    pausar();
    return RESULTADOS;
}

// ── Resultados — recibe vector de Pelicula ────────────────────────────────────
inline EstadoPantalla vistaResultados(const std::vector<Pelicula>& resultados) {
    limpiarPantalla();
    std::cout << "========================================\n";
    std::cout << "            RESULTADOS                  \n";
    std::cout << "========================================\n";

    if (resultados.empty()) {
        std::cout << "  No se encontraron peliculas.\n";
    } else {
        int i = 1;
        for (const auto& p : resultados) {
            std::cout << "  " << i++ << ". " << p.titulo << "\n";
            if (i > 20) {          // límite visual para no desbordar pantalla
                std::cout << "  ... y " << (resultados.size() - 20) << " mas.\n";
                break;
            }
        }
    }

    std::cout << "----------------------------------------\n";
    std::cout << "  [0] Volver al menu principal\n";
    std::cout << "----------------------------------------\n";
    leerOpcion(0, 0);
    return MENU_PRINCIPAL;
}

// ── Ver Mas Tarde ─────────────────────────────────────────────────────────────
inline EstadoPantalla vistaVerMasTarde() {
    limpiarPantalla();
    std::cout << "========================================\n";
    std::cout << "           VER MAS TARDE                \n";
    std::cout << "========================================\n";
    std::cout << "  (Lista vacia)\n";
    std::cout << "----------------------------------------\n";
    std::cout << "  [0] Volver al menu principal\n";
    std::cout << "----------------------------------------\n";
    leerOpcion(0, 0);
    return MENU_PRINCIPAL;
}