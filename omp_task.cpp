#include <iostream>
#include <fstream>
#include <omp.h>

using namespace std;

int main() {
    ifstream matrix_1, matrix_2;
    int amountOfThreads, a, b, c, n = 1000;

    int** matrixA, ** matrixB, ** matrixResult;

    matrixA = new int*[n];
    matrixB = new int*[n];
    matrixResult = new int*[n];

    for (int i = 0; i < n; ++i) {
        matrixA[i] = new int[n];
        matrixB[i] = new int[n];
        matrixResult[i] = new int[n];
    }

    matrix_1.open("matrix_1.txt");
    matrix_2.open("matrix_2.txt");

    cout << "matrix reading started...\n";

    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j) {
            matrix_1 >> matrixA[i][j];
            matrix_2 >> matrixB[i][j];
            matrixResult[i][j] = 0;
        }

    matrix_1.close();
    matrix_2.close();

    cout << "matrix reading done.\n" << "enter amount of threads: ";
    cin >> amountOfThreads;

    omp_set_num_threads(amountOfThreads);

    cout << "matrix multiplication started...\n";

    double startTime = omp_get_wtime();

#pragma omp parallel num_threads(amountOfThreads)
    {
#pragma omp for private(a, b, c)
            for (a = 0; a < n; ++a)
                for (b = 0; b < n; ++b)
                    for (c = 0; c < n; ++c) matrixResult[a][b] += matrixA[a][c] * matrixB[c][b];
    }

    cout << "matrix multiplication ended.\namount of threads: " << amountOfThreads <<  "\ntime spent: " << omp_get_wtime() - startTime;

    system("pause");
    return 0;
}
