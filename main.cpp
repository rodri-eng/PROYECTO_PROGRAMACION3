#include "Vistas.h"
#include "MotorBusqueda.h"

int main() {
    EstadoPantalla estado = MENU_PRINCIPAL;
    std::vector<Pelicula> db = cargarCSV();
    std::vector<Pelicula> resultados;
    std::string ultimaBusqueda;

    while (estado != SALIR) {
        switch (estado) {
            case MENU_PRINCIPAL:
                estado = vistaMenuPrincipal();
                break;

            case BUSCAR: {
                std::string consulta;
                estado = vistaBuscar(consulta);          // consulta se rellena dentro
                if (!consulta.empty()) {
                    resultados = buscarPorPalabra(db, consulta);
                    ultimaBusqueda = consulta;
                }
                break;
            }

            case RESULTADOS:
                estado = vistaResultados(resultados);    // recibe el vector filtrado
                break;

            case VER_MAS_TARDE:
                estado = vistaVerMasTarde();
                break;

            default:
                estado = SALIR;
                break;
        }
    }

    limpiarPantalla();
    std::cout << "Hasta luego!\n";
    return 0;
}