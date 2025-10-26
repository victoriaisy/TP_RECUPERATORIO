#include <iostream>
#include <fstream>
using namespace std;

struct Mesa {
    int numero;
    bool libre;
    int ganancia;
    Mesa* siguiente;
};

Mesa* crearMesa(int num, bool libre, int ganancia) {
    Mesa* nueva = new Mesa;
    nueva->numero = num;
    nueva->libre = libre;
    nueva->ganancia = ganancia;
    nueva->siguiente = nullptr;
    return nueva;
}

void agregarMesa(Mesa*& inicio, int num, bool libre, int ganancia) {
    Mesa* nueva = crearMesa(num, libre, ganancia);
    if (!inicio) inicio = nueva;
    else {
        Mesa* actual = inicio;
        while (actual->siguiente)
            actual = actual->siguiente;
        actual->siguiente = nueva;
    }
    cout << "Mesa agregada correctamente.\n";
}

Mesa* buscarMesa(Mesa* inicio, int num) {
    Mesa* actual = inicio;
    while (actual) {
        if (actual->numero == num)
            return actual;
        actual = actual->siguiente;
    }
    return nullptr;
}

void eliminarMesa(Mesa*& inicio, int num) {
    Mesa* actual = inicio;
    Mesa* anterior = nullptr;

    while (actual && actual->numero != num) {
        anterior = actual;
        actual = actual->siguiente;
    }

    if (!actual) {
        cout << "No se encontró la mesa.\n";
        return;
    }

    if (!anterior)
        inicio = actual->siguiente;
    else
        anterior->siguiente = actual->siguiente;

    delete actual;
    cout << "Mesa eliminada correctamente.\n";
}

void modificarMesa(Mesa* inicio, int num) {
    Mesa* m = buscarMesa(inicio, num);
    if (!m) {
        cout << "Mesa no encontrada.\n";
        return;
    }

    int opcion;
    cout << "\n--- MODIFICAR MESA ---\n"
         << "1. Cambiar estado (libre/ocupada)\n"
         << "2. Cambiar ganancia\n"
         << "Seleccione opción: ";
    cin >> opcion;

    if (opcion == 1) {
        m->libre = !m->libre;
        cout << "Estado cambiado correctamente.\n";
    } else if (opcion == 2) {
        cout << "Ingrese nueva ganancia: $";
        cin >> m->ganancia;
        cout << "Ganancia actualizada.\n";
    } else {
        cout << "Opción inválida.\n";
    }
}

void mostrarMesas(Mesa* inicio) {
    if (!inicio) {
        cout << "No hay mesas cargadas.\n";
        return;
    }

    cout << "\n--- LISTA DE MESAS ---\n";
    Mesa* actual = inicio;
    while (actual) {
        cout << "Mesa " << actual->numero << " | Estado: ";
        if (actual->libre)
            cout << "Libre";
        else
            cout << "Ocupada";
        cout << " | Ganancia: $" << actual->ganancia << endl;
        actual = actual->siguiente;
    }
    cout << "----------------------\n";
}

void guardarArchivo(Mesa* inicio) {
    ofstream file("mesas.txt");
    if (!file) {
        cout << "Error al abrir mesas.txt para guardar.\n";
        return;
    }

    Mesa* actual = inicio;
    while (actual) {
        file << actual->numero << " "
             << actual->libre << " "
             << actual->ganancia << endl;
        actual = actual->siguiente;
    }
    file.close();
    cout << "Datos guardados correctamente en mesas.txt\n";
}

Mesa* cargarArchivo() {
    ifstream file("mesas.txt");
    Mesa* inicio = nullptr;

    if (!file) {
        cout << "No se encontró el archivo mesas.txt. Se creará uno nuevo.\n";
        return nullptr;
    }

    int num;
    bool libre;
    int ganancia;

    while (file >> num >> libre >> ganancia) {
        agregarMesa(inicio, num, libre, ganancia);
    }

    file.close();
    cout << "Mesas cargadas correctamente desde mesas.txt\n";
    return inicio;
}

void liberarMemoria(Mesa*& inicio) {
    while (inicio) {
        Mesa* temp = inicio;
        inicio = inicio->siguiente;
        delete temp;
    }
}

int main() {
    Mesa* listaMesas = cargarArchivo();
    int opcion;

    do {
        cout << "\n=== SISTEMA 1: CONFIGURADOR DE MESAS ===\n";
        cout << "1. Ver mesas\n";
        cout << "2. Agregar mesa\n";
        cout << "3. Eliminar mesa\n";
        cout << "4. Modificar mesa\n";
        cout << "5. Guardar y salir\n";
        cout << "Seleccione opción: ";
        cin >> opcion;

        switch (opcion) {
            case 1:
                mostrarMesas(listaMesas);
                break;
            case 2: {
                int num;
                cout << "Ingrese número de mesa: ";
                cin >> num;
                if (buscarMesa(listaMesas, num)) {
                    cout << "Ya existe una mesa con ese número.\n";
                } else {
                    agregarMesa(listaMesas, num, true, 0);
                }
                break;
            }
            case 3: {
                int num;
                cout << "Ingrese número de mesa a eliminar: ";
                cin >> num;
                eliminarMesa(listaMesas, num);
                break;
            }
            case 4: {
                int num;
                cout << "Ingrese número de mesa a modificar: ";
                cin >> num;
                modificarMesa(listaMesas, num);
                break;
            }
            case 5:
                guardarArchivo(listaMesas);
                liberarMemoria(listaMesas);
                cout << "Saliendo del sistema.\n";
                break;
            default:
                cout << "Opción inválida.\n";
        }

    } while (opcion != 5);

    return 0;
} 
