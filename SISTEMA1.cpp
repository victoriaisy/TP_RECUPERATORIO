#include <iostream>
#include <cstdio>
#include <cstring>
using namespace std;

// ESTRUCTURA DE MESA
struct Mesa {
    int numero;
    bool libre;
    int ganancia;
    Mesa* siguiente;  // lista enlazada
};

// estructura para leer/escribir en archivo
struct MesaData {
    int numero;
    int libre;   // 0 o 1
    int ganancia;
};

// Otras funciones
Mesa* crearMesa(int num, bool libre, int ganancia) {
    Mesa* nueva = new Mesa;
    nueva->numero = num;
    nueva->libre = libre;
    nueva->ganancia = ganancia;
    nueva->siguiente = nullptr;
    return nueva;
}

// busca mesa por número, devuelve puntero o nullptr
Mesa* buscarMesa(Mesa* inicio, int num) {
    Mesa* actual = inicio;
    while (actual) {
        if (actual->numero == num) return actual;
        actual = actual->siguiente;
    }
    return nullptr;
}

// agrega evitando duplicados
void agregarMesa(Mesa*& inicio, int num, bool libre, int ganancia) {
    if (buscarMesa(inicio, num) != nullptr) {
        cout << "Ya existe una mesa con ese numero.\n";
        return;
    }
    Mesa* nueva = crearMesa(num, libre, ganancia);
    if (!inicio)
        inicio = nueva;
    else {
        Mesa* actual = inicio;
        while (actual->siguiente)
            actual = actual->siguiente;
        actual->siguiente = nueva;
    }
    cout << "Mesa agregada correctamente.\n";
}

void eliminarMesa(Mesa*& inicio, int num) {
    Mesa* actual = inicio;
    Mesa* anterior = nullptr;

    while (actual && actual->numero != num) {
        anterior = actual;
        actual = actual->siguiente;
    }

    if (!actual) {
        cout << "Mesa no encontrada.\n";
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
    Mesa* actual = buscarMesa(inicio, num);

    if (!actual) {
        cout << "Mesa no encontrada.\n";
        return;
    }

    int opcion;
    cout << "Modificar mesa " << num << ":\n"
         << "1. Cambiar estado (libre/ocupada)\n"
         << "2. Cambiar ganancia\n"
         << "Seleccione: ";
    cin >> opcion;

    if (opcion == 1) {
        actual->libre = !actual->libre;
        cout << "Estado cambiado.\n";
    } else if (opcion == 2) {
        cout << "Ingrese nueva ganancia: ";
        cin >> actual->ganancia;
        cout << "Ganancia actualizada.\n";
    } else {
        cout << "Opción inválida.\n";
    }
}

void mostrarMesas(Mesa* inicio) {
    if (!inicio) {
        cout << "\nNo hay mesas registradas.\n";
        return;
    }
    Mesa* actual = inicio;
    cout << "\n LISTA DE MESAS";
    while (actual) {
        cout << "Mesa " << actual->numero
             << " | Estado: " << (actual->libre ? "Libre" : "Ocupada")
             << " | Ganancia: $" << actual->ganancia << endl;
        actual = actual->siguiente;
    }
}

// liberar memoria de la lista
void liberarLista(Mesa*& inicio) {
    Mesa* actual = inicio;
    while (actual) {
        Mesa* sig = actual->siguiente;
        delete actual;
        actual = sig;
    }
    inicio = nullptr;
}

// FUNCIONES DE ARCHIVO (binario .dat)
void guardarArchivo(Mesa* inicio) {
    FILE* archivo = fopen("mesas.dat", "wb");
    if (!archivo) {
        cout << "Error: no se pudo abrir mesas.dat para escritura.\n";
        return;
    }

    Mesa* actual = inicio;
    while (actual) {
        MesaData data;
        data.numero = actual->numero;
        data.libre = actual->libre ? 1 : 0;
        data.ganancia = actual->ganancia;
        fwrite(&data, sizeof(MesaData), 1, archivo);
        actual = actual->siguiente;
    }

    fclose(archivo);
    cout << "Archivo mesas.dat guardado correctamente.\n";
}

Mesa* cargarArchivo() {
    Mesa* inicio = nullptr;
    Mesa* fin = nullptr;
    FILE* archivo = fopen("mesas.dat", "rb");
    if (!archivo) {
        cout << "No existe mesas.dat, se creará al guardar.\n";
        return nullptr;
    }

    MesaData data;
    while (fread(&data, sizeof(MesaData), 1, archivo) == 1) {
        Mesa* nueva = crearMesa(data.numero, data.libre != 0, data.ganancia);
        if (!inicio)
            inicio = fin = nueva;
        else {
            fin->siguiente = nueva;
            fin = nueva;
        }
    }

    fclose(archivo);
    return inicio;
}

//  MENÚ PRINCIPAL
int main() {
    Mesa* listaMesas = cargarArchivo();
    int opcion;

    do {
        cout << "\n SISTEMA 1: CONFIGURADOR DE MESAS "
             << "1. Alta de mesa\n"
             << "2. Baja de mesa\n"
             << "3. Modificar mesa\n"
             << "4. Mostrar mesas\n"
             << "0. Salir\n"
             << "Seleccione: ";
        cin >> opcion;

        switch (opcion) {
            case 1: {
                int num;
                cout << "Ingrese número de mesa: ";
                cin >> num;
                agregarMesa(listaMesas, num, true, 0);
                break;
            }
            case 2: {
                int num;
                cout << "Ingrese número de mesa a eliminar: ";
                cin >> num;
                eliminarMesa(listaMesas, num);
                break;
            }
            case 3: {
                int num;
                cout << "Ingrese número de mesa a modificar: ";
                cin >> num;
                modificarMesa(listaMesas, num);
                break;
            }
            case 4:
                mostrarMesas(listaMesas);
                break;
            case 0:
                cout << "Saliendo del sistema...\n";
                break;
            default:
                cout << "Opción inválida.\n";
        }
    } while (opcion != 0);

    guardarArchivo(listaMesas);
    liberarLista(listaMesas);
    return 0;
}
