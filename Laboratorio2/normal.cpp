#include <iostream>
#include <vector>

#include <ctime>

using namespace std;

// Función para multiplicación de matrices de manera clásica
void matrixMultiplication(const vector<vector<int>>& A, const vector<vector<int>>& B, vector<vector<int>>& C) {
    int N = A.size();
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            for (int k = 0; k < N; ++k) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

int main() {
    int N = 750; // Tamaño de las matrices
    
    // Inicializa las matrices A, B y C
    vector<vector<int>> A(N, vector<int>(N, 1));
    vector<vector<int>> B(N, vector<int>(N, 2));
    vector<vector<int>> C(N, vector<int>(N, 0));

    unsigned t0, t1;
    double time;
    t0 = clock();
    // Realiza la multiplicación de matrices de manera clásica
    matrixMultiplication(A, B, C);

    t1 = clock();

    time = (double(t1 - t0) / CLOCKS_PER_SEC);
    cout << "Multiplicacion clasica tardo: " << time << " segundos"<< endl;

    

    // Imprime el resultado de la multiplicación clásica
    // cout << "Resultado de la multiplicación clásica:" << endl;
    // for (int i = 0; i < N; ++i) {
    //     for (int j = 0; j < N; ++j) {
    //         cout << C[i][j] << " ";
    //     }
    //     cout << endl;
    // }

    return 0;
}
