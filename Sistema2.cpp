#include <iostream>
#include <string>
#include <fstream>
using namespace std;

struct Mesa {
    int numero;
    bool libre;
    int ganancia;
};

const int MAX_MESAS = 5;
const int MAX_COLA = 10;

void guardarEnArchivo(Mesa mesas[], int n) {
    ofstream file("mesas.txt");
    if (!file) {
        cout << "Error al abrir el archivo para guardar.\n";
        return;
    }

    for (int i = 0; i < n; i++) {
        file << mesas[i].numero << " "
             << mesas[i].libre << " "
             << mesas[i].ganancia << endl;
    }
    file.close();
    cout << "Datos guardados correctamente en mesas.txt\n";
}

void cargarDesdeArchivo(Mesa mesas[], int n) {
    ifstream file("mesas.txt");
    if (!file) {
        cout << "No se encontró el archivo mesas.txt. Se crearán mesas nuevas.\n";
        for (int i = 0; i < n; i++) {
            mesas[i].numero = i + 1;
            mesas[i].libre = true;
            mesas[i].ganancia = 0;
        }
        return;
    }

    int i = 0;
    while (i < n && file >> mesas[i].numero >> mesas[i].libre >> mesas[i].ganancia) {
        i++;
    }

    for (; i < n; i++) {
        mesas[i].numero = i + 1;
        mesas[i].libre = true;
        mesas[i].ganancia = 0;
    }

    file.close();
    cout << "Mesas cargadas correctamente desde mesas.txt\n";
}

void mostrarMesas(Mesa mesas[], int n) {
    cout << "\n--- ESTADO DE LAS MESAS ---\n";
    for (int i = 0; i < n; i++) {
        cout << "Mesa " << mesas[i].numero << " | Estado: ";
        if (mesas[i].libre)
            cout << "Libre";
        else
            cout << "Ocupada";
        cout << " | Ganancia: $" << mesas[i].ganancia << endl;
    }
    cout << "----------------------------\n";
}

void recibirCliente(Mesa mesas[], int n, string cola[], int &inicioCola, int &finCola) {
    string nombre;
    cout << "Ingrese el nombre del cliente: ";
    cin >> nombre;

    for (int i = 0; i < n; i++) {
        if (mesas[i].libre) {
            mesas[i].libre = false;
            cout << "Cliente " << nombre << " fue asignado a la mesa " << mesas[i].numero << ".\n";
            return;
        }
    }

    if (finCola < MAX_COLA) {
        cola[finCola] = nombre;
        finCola++;
        cout << "Todas las mesas están ocupadas. Cliente " << nombre << " agregado a la cola de espera.\n";
    } else {
        cout << "La cola de espera está llena. No se puede agregar más clientes.\n";
    }
}

void liberarMesa(Mesa mesas[], int n, string cola[], int &inicioCola, int &finCola) {
    int numero;
    int consumo;

    cout << "Ingrese el número de mesa a liberar: ";
    cin >> numero;

    if (numero < 1 || numero > n) {
        cout << "Número de mesa inválido.\n";
        return;
    }

    Mesa &m = mesas[numero - 1];

    if (m.libre) {
        cout << "Esa mesa ya está libre.\n";
        return;
    }

    cout << "Ingrese el monto consumido: $";
    cin >> consumo;

    m.ganancia += consumo;
    m.libre = true;
    cout << "Mesa " << m.numero << " liberada. Ganancia acumulada: $" << m.ganancia << "\n";

    if (inicioCola < finCola) {
        string cliente = cola[inicioCola];
        inicioCola++;

        for (int i = inicioCola; i < finCola; i++) {
            cola[i - 1] = cola[i];
        }
        finCola--;

        m.libre = false;
        cout << "Cliente en espera " << cliente << " fue asignado a la mesa " << m.numero << ".\n";
    }
}

void mostrarCola(string cola[], int inicioCola, int finCola) {
    cout << "\n--- COLA DE ESPERA ---\n";
    if (inicioCola == finCola) {
        cout << "No hay clientes en espera.\n";
    } else {
        for (int i = inicioCola; i < finCola; i++) {
            cout << "- " << cola[i] << endl;
        }
    }
    cout << "----------------------\n";
}

int main() {
    Mesa mesas[MAX_MESAS];
    string cola[MAX_COLA];
    int inicioCola = 0;
    int finCola = 0;

    cargarDesdeArchivo(mesas, MAX_MESAS);

    int opcion;
    do {
        cout << "\n--- MENU PRINCIPAL ---\n";
        cout << "1. Recibir cliente\n";
        cout << "2. Liberar mesa\n";
        cout << "3. Mostrar mesas\n";
        cout << "4. Mostrar cola de espera\n";
        cout << "5. Guardar y salir\n";
        cout << "Elija una opción: ";
        cin >> opcion;

        switch (opcion) {
            case 1:
                recibirCliente(mesas, MAX_MESAS, cola, inicioCola, finCola);
                break;
            case 2:
                liberarMesa(mesas, MAX_MESAS, cola, inicioCola, finCola);
                break;
            case 3:
                mostrarMesas(mesas, MAX_MESAS);
                break;
            case 4:
                mostrarCola(cola, inicioCola, finCola);
                break;
            case 5:
                guardarEnArchivo(mesas, MAX_MESAS);
                cout << "Saliendo del sistema.\n";
                break;
            default:
                cout << "Opción inválida.\n";
        }
    } while (opcion != 5);

    return 0;
}
