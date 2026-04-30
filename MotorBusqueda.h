#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include <cctype>

// ── Índices de columnas del CSV de Wikipedia ──────────────────────────────────
static constexpr int COL_TITULO   = 1;
static constexpr int COL_SINOPSIS = 6;

// ── Estructura de película ────────────────────────────────────────────────────
struct Pelicula {
    std::string titulo;
    std::string sinopsis;
    std::vector<std::string> raw; // fila completa por si otros integrantes necesitan más columnas
};

// ── Estandarizar celda vacía / Unknown ───────────────────────────────────────
inline std::string estandarizar(const std::string& celda) {
    if (celda.empty()         ||
        celda == "Unknown"    ||
        celda == "\"\""       ||
        celda == "\"Unknown\"")
        return "unknown";
    return celda;
}

// ── Parsear una línea CSV respetando comas dentro de comillas ─────────────────
inline std::vector<std::string> parsearLinea(const std::string& linea) {
    std::vector<std::string> fila;
    std::string celda;
    bool en_comillas = false;

    for (char c : linea) {
        if (c == '"') {
            en_comillas = !en_comillas;
            celda += c;
        } else if (c == ',' && !en_comillas) {
            fila.push_back(estandarizar(celda));
            celda.clear();
        } else {
            celda += c;
        }
    }
    fila.push_back(estandarizar(celda)); // última celda
    return fila;
}

// ── Limpieza: minúsculas + eliminar puntuación especial ──────────────────────
inline std::string limpiar(std::string texto) {
    // Minúsculas
    std::transform(texto.begin(), texto.end(), texto.begin(),
                   [](unsigned char c){ return std::tolower(c); });

    // Eliminar: . , ( ) ! ? ; : " ' - _ / \ | [ ] { } * # @ ^ ~ ` + = < > %
    const std::string descartados = ".,()!?;:\"'\\-_/|[]{}*#@^~`+=<>%";
    std::string resultado;
    resultado.reserve(texto.size());
    for (char c : texto) {
        if (descartados.find(c) == std::string::npos)
            resultado += c;
    }
    return resultado;
}

// ── Tokenización: divide string limpio en palabras ───────────────────────────
inline std::vector<std::string> tokenizar(std::string texto) {
    texto = limpiar(texto);
    std::vector<std::string> tokens;
    std::istringstream ss(texto);
    std::string palabra;
    while (ss >> palabra) {
        if (!palabra.empty())
            tokens.push_back(palabra);
    }
    return tokens;
}

// ── Carga el CSV y devuelve vector de Pelicula ────────────────────────────────
inline std::vector<Pelicula> cargarCSV(const std::string& ruta = "wiki_movie_plots_deduped.csv"){
    std::vector<Pelicula> peliculas;
    std::ifstream archivo(ruta);

    if (!archivo.is_open()) {
        std::cerr << "[MotorBusqueda] Error: no se pudo abrir \"" << ruta << "\"\n";
        return peliculas;
    }

    std::string linea;
    bool primeraLinea = true;

    while (std::getline(archivo, linea)) {
        if (primeraLinea) { primeraLinea = false; continue; } // saltar cabecera
        if (linea.empty()) continue;

        std::vector<std::string> fila = parsearLinea(linea);

        Pelicula p;
        p.raw = fila;
        p.titulo   = (fila.size() > COL_TITULO)   ? fila[COL_TITULO]   : "unknown";
        p.sinopsis = (fila.size() > COL_SINOPSIS)  ? fila[COL_SINOPSIS] : "unknown";
        peliculas.push_back(std::move(p));
    }

    archivo.close();
    std::cout << "[MotorBusqueda] " << peliculas.size()
              << " peliculas cargadas desde \"" << ruta << "\"\n";
    return peliculas;
}

// ── Búsqueda por token en título y sinopsis ───────────────────────────────────
inline std::vector<Pelicula> buscarPorPalabra(const std::vector<Pelicula>& db,
                                               const std::string& consulta) {
    std::vector<std::string> tokensConsulta = tokenizar(consulta);
    std::vector<Pelicula> resultados;

    for (const auto& p : db) {
        std::vector<std::string> tokensTitulo   = tokenizar(p.titulo);
        std::vector<std::string> tokensSinopsis = tokenizar(p.sinopsis);

        for (const auto& t : tokensConsulta) {
            bool enTitulo = std::find(tokensTitulo.begin(),
                                      tokensTitulo.end(), t) != tokensTitulo.end();
            bool enSinopsis = std::find(tokensSinopsis.begin(),
                                         tokensSinopsis.end(), t) != tokensSinopsis.end();
            if (enTitulo || enSinopsis) {
                resultados.push_back(p);
                break; // evitar duplicar la misma película
            }
        }
    }
    return resultados;
}