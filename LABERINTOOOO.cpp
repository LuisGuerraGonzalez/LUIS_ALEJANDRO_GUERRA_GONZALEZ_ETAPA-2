#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <windows.h>
#include <cstring>

using namespace std;

const int FILAS = 70;
const int COLUMNAS = 71;
vector<string> laberinto;
bool visitado[FILAS][COLUMNAS];

struct Punto {
    int x, y;
};

Punto padre[FILAS][COLUMNAS];

int dx[4] = {-1, 1, 0, 0};
int dy[4] = {0, 0, -1, 1};

void mover_cursor(int x, int y) {
    COORD coord;
    coord.X = (SHORT)y;
    coord.Y = (SHORT)x;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

bool es_valido(int x, int y) {
    return x >= 0 && x < FILAS && y >= 0 && y < COLUMNAS &&
           (laberinto[x][y] == ' ' || laberinto[x][y] == 'A' || laberinto[x][y] == 'B');
}

void imprimir_laberinto() {
    for (int i = 0; i < laberinto.size(); ++i) {
        cout << laberinto[i] << '\n';
    }
}

bool encontrar_camino(Punto inicio, Punto fin) {
    memset(visitado, false, sizeof(visitado));
    queue<Punto> q;
    q.push(inicio);
    visitado[inicio.x][inicio.y] = true;

    bool encontrado = false;
    while (!q.empty()) {
        Punto actual = q.front();
        q.pop();

        if (actual.x == fin.x && actual.y == fin.y) {
            encontrado = true;
            break;
        }

        for (int d = 0; d < 4; d++) {
            int nx = actual.x + dx[d];
            int ny = actual.y + dy[d];

            if (es_valido(nx, ny) && !visitado[nx][ny]) {
                visitado[nx][ny] = true;
                padre[nx][ny] = actual;
                q.push({nx, ny});
            }
        }
    }

    return encontrado;
}

int main() {
    ifstream archivo("laberinto_70x70.txt");
    if (!archivo.is_open()) {
        cout << "Error: no se pudo abrir laberinto_70x70.txt\n";
        return 1;
    }

    string linea;
    laberinto.clear();
    while (getline(archivo, linea)) {
        if ((int)linea.length() < COLUMNAS) {
            linea.append(COLUMNAS - linea.length(), ' ');
        } else if ((int)linea.length() > COLUMNAS) {
            linea = linea.substr(0, COLUMNAS);
        }
        laberinto.push_back(linea);
    }
    archivo.close();

    if ((int)laberinto.size() < FILAS) {
        cout << "Error: el archivo no contiene suficientes filas para el laberinto.\n";
        return 1;
    }

    Punto inicio, fin;
    bool entrada_encontrada = false;
    bool salida_encontrada = false;

    // Buscar entrada y salida en bordes con espacio ' '
    for (int i = 0; i < FILAS; ++i) {
        for (int j = 0; j < COLUMNAS; ++j) {
            if ((i == 0 || i == FILAS - 1 || j == 0 || j == COLUMNAS - 1) && laberinto[i][j] == ' ') {
                if (!entrada_encontrada) {
                    inicio = {i, j};
                    entrada_encontrada = true;
                    laberinto[i][j] = 'A';
                } else if (!salida_encontrada) {
                    fin = {i, j};
                    salida_encontrada = true;
                    laberinto[i][j] = 'B';
                }
            }
        }
    }

    if (!entrada_encontrada || !salida_encontrada) {
        cout << "No se encontraron suficientes entradas/salidas en el laberinto.\n";
        return 1;
    }

    system("cls");
    imprimir_laberinto();

    if (!encontrar_camino(inicio, fin)) {
        cout << "No se encontró un camino entre A y B.\n";
        return 1;
    }

    vector<Punto> camino;
    Punto actual = fin;
    while (!(actual.x == inicio.x && actual.y == inicio.y)) {
        camino.push_back(actual);
        actual = padre[actual.x][actual.y];
    }
    camino.push_back(inicio);

    // Mostrar camino con '#'
    for (int i = (int)camino.size() - 1; i >= 0; --i) {
        Punto p = camino[i];
        mover_cursor(p.x, p.y);
        cout << '#' << flush;
        Sleep(50);
    }

    mover_cursor(FILAS + 1, 0);
    cout << "\nRecorrido completado.\n";

    return 0;
}

