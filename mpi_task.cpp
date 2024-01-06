#include <iostream>
#include <fstream>
#include <mpi.h>

using namespace std;

int main(int argc, char** argv) {
    ifstream matrix_1, matrix_2;

    int rank, size, extraData, dataForProcess, n = 1000;

    int* matrixA = new int[n * n], * matrixB = new int[n * n], * matrixResult = new int[n * n];

    for (int i = 0; i < n * n; ++i) matrixA[i] = matrixB[i] = matrixResult[i] = 0;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Status status;

    dataForProcess = n / size;
    extraData = (rank == size - 1 && n % size != 0) ? n % size : 0;

    if (rank == 0) {
        matrix_1.open("matrix_1.txt");
        matrix_2.open("matrix_2.txt");

        cout << "matrix reading started...\n";

        for (int i = 0; i < n; ++i)
            for (int j = 0; j < n; ++j) {
                matrix_1 >> matrixA[i * n + j];
                matrix_2 >> matrixB[i * n + j];
            }

        matrix_1.close();
        matrix_2.close();

        cout << "matrix reading done.\n";
    }

    cout << "matrix multiplication started...\n";

    double startTime = MPI_Wtime();

    for (int i = 1; i < size; ++i) {
        MPI_Send(&matrixA[dataForProcess * i * n], (i == size - 1) ? (dataForProcess + n % size) * n : dataForProcess * n,
                 MPI_DOUBLE, i, 1, MPI_COMM_WORLD);
        MPI_Send(&matrixB[0], n * n, MPI_DOUBLE, i, 2, MPI_COMM_WORLD);
    } else {
        MPI_Recv(&A[dataForProcess * rank * n], (dataForProcess + extraData) * n, MPI_DOUBLE, 0, 1, MPI_COMM_WORLD, &status);
        MPI_Recv(&B[0], n * n, MPI_DOUBLE, 0, 2, MPI_COMM_WORLD, &status);
    }

    for (int i = dataForProcess * rank; i < dataForProcess * (rank + 1) + extraData; ++i)
        for (int j = 0; j < n; ++j) {
            matrixResult[i * n + j] = 0;
            for (int t  = 0; t < n; ++t) matrixResult[i * n + j] += matrixA[i * n + t] * matrixB[t * n + j];
        }

    delete[] matrixA;
    delete[] matrixB;

    if (rank != 0) MPI_Send(&matrixResult[dataForProcess * rank * n], (dataForProcess + extraData) * n, MPI_DOUBLE, 0, 4, MPI_COMM_WORLD);
    else {
        for (int i = 1; i < size; ++i) MPI_Recv(&matrixResult[dataForProcess * i * n], (i == size - 1) ? (dataForProcess + n % size) * n :
        dataForProcess * n, MPI_DOUBLE, i, 4, MPI_COMM_WORLD, &status);

        double endTime = MPI_Wtime();

        cout << "matrix multiplication ended.\ntime spent: " <<  endTime - startTime;
    }

    MPI_Finalize();
    delete[] matrixResult;
    return 0;
}