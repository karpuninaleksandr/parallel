#include <iostream>
#include <fstream>

using namespace std;

int main() {
    int n = 1000;

    cout << "matrix generation started";
    fstream matrix_1("C:\\Users\\karpunin\\CLionProjects\\parallel\\matrix_1.txt", fstream::in | fstream::out | fstream::trunc);
    fstream matrix_2("C:\\Users\\karpunin\\CLionProjects\\parallel\\matrix_2.txt", fstream::in | fstream::out | fstream::trunc);

    for (int i = 0; i < n; ++i) {
        for (int j  = 0; j < n; ++j) {
            matrix_1 << rand() << " ";
            matrix_2 << rand() << " ";
        }
        matrix_1 << "\n";
        matrix_2 << "\n";
    }

    matrix_1.close();
    matrix_2.close();
    return 0;
}