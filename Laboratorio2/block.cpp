#include <iostream>
#include <vector>

#include <ctime>

using namespace std;

// Función para multiplicación de matrices por bloques
void matrixBlockMultiplication(const vector<vector<int>>& A, const vector<vector<int>>& B, vector<vector<int>>& C, int blockSize) {
    int N = A.size();
    int numBlocks = N / blockSize;

    for (int i_block = 0; i_block < numBlocks; ++i_block) {
        for (int j_block = 0; j_block < numBlocks; ++j_block) {
            for (int k_block = 0; k_block < numBlocks; ++k_block) {
                for (int i = i_block * blockSize; i < (i_block + 1) * blockSize; ++i) {
                    for (int j = j_block * blockSize; j < (j_block + 1) * blockSize; ++j) {
                        for (int k = k_block * blockSize; k < (k_block + 1) * blockSize; ++k) {
                            C[i][j] += A[i][k] * B[k][j];
                        }
                    }
                }
            }
        }
    }
}

int main() {
    int N = 750; // Tamaño de las matrices
    int blockSize = 250; // Tamaño de bloque

    // Inicializa las matrices A, B y C
    vector<vector<int>> A(N, vector<int>(N, 1));
    vector<vector<int>> B(N, vector<int>(N, 2));
    vector<vector<int>> C(N, vector<int>(N, 0));

    unsigned t0, t1;
    double time;
    t0 = clock();
    
    // Realiza la multiplicación de matrices por bloques
    matrixBlockMultiplication(A, B, C, blockSize);

    t1 = clock();

    time = (double(t1 - t0) / CLOCKS_PER_SEC);
    cout << "Multiplicacion en bloques tardo: " << time << " segundos"<< endl;

    // Imprime el resultado
    // cout << "Resultado de la multiplicación por bloques:" << endl;
    // for (int i = 0; i < N; ++i) {
    //     for (int j = 0; j < N; ++j) {
    //         cout << C[i][j] << " ";
    //     }
    //     cout << endl;
    // }

    return 0;
}
