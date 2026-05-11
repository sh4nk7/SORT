#include <chrono>
#include <iostream>
#include <sstream>
#include <thread>
#include <vector>

#include "../../es2/src/matrix.h"

const size_t STD_SIZE = 10;
const size_t MAX_PRINT_SIZE = 30;

void mult(size_t row, size_t column, const matrix &M1, const matrix &M2, matrix &Res)
{
    Res(row, column) = 0;

    for (size_t position = 0; position < Res.size(); ++position)
    {
        Res(row, column) += M1(row, position) * M2(position, column);
    }
}

void compute_row(size_t row, const matrix &M1, const matrix &M2, matrix &Res)
{
    for (size_t column = 0; column < Res.size(); ++column)
    {
        mult(row, column, M1, M2, Res);
    }
}

void serial_product(const matrix &M1, const matrix &M2, matrix &Res)
{
    for (size_t row = 0; row < Res.size(); ++row)
    {
        compute_row(row, M1, M2, Res);
    }
}

void threaded_product(const matrix &M1, const matrix &M2, matrix &Res)
{
    std::vector<std::thread> workers;
    workers.reserve(Res.size());

    for (size_t row = 0; row < Res.size(); ++row)
    {
        workers.emplace_back(compute_row, row, std::cref(M1), std::cref(M2), std::ref(Res));
    }

    for (std::thread &worker : workers)
    {
        worker.join();
    }
}

void fill_matrices(matrix &MA, matrix &MB)
{
    for (size_t i = 0; i < MA.size(); ++i)
    {
        for (size_t j = 0; j < MA.size(); ++j)
        {
            MA(i, j) = 1;
            MB(i, j) = i + j + 1;
        }
    }
}

bool same_matrix(const matrix &M1, const matrix &M2)
{
    for (size_t i = 0; i < M1.size(); ++i)
    {
        for (size_t j = 0; j < M1.size(); ++j)
        {
            if (M1(i, j) != M2(i, j))
            {
                return false;
            }
        }
    }

    return true;
}

int main(int argc, char *argv[])
{
    size_t size = STD_SIZE;

    if (argc > 1)
    {
        std::istringstream iss(argv[1]);
        iss >> size;
    }

    matrix MA(size), MB(size), serial_result(size), threaded_result(size);
    fill_matrices(MA, MB);

    if (size < MAX_PRINT_SIZE)
    {
        std::cout << "Matrix Ma:" << std::endl << MA;
        std::cout << "Matrix Mb:" << std::endl << MB;
    }

    const auto serial_start = std::chrono::high_resolution_clock::now();
    serial_product(MA, MB, serial_result);
    const auto serial_end = std::chrono::high_resolution_clock::now();

    const auto threaded_start = std::chrono::high_resolution_clock::now();
    threaded_product(MA, MB, threaded_result);
    const auto threaded_end = std::chrono::high_resolution_clock::now();

    const std::chrono::duration<double, std::milli> serial_time = serial_end - serial_start;
    const std::chrono::duration<double, std::milli> threaded_time = threaded_end - threaded_start;

    if (size < MAX_PRINT_SIZE)
    {
        std::cout << "Matrix Mc seriale (Ma * Mb):" << std::endl << serial_result;
        std::cout << "Matrix Mc multithread (Ma * Mb):" << std::endl << threaded_result;
    }

    std::cout << "Dimensione matrice: " << size << "x" << size << std::endl;
    std::cout << "Tempo seriale: " << serial_time.count() << " ms" << std::endl;
    std::cout << "Tempo multithread: " << threaded_time.count() << " ms" << std::endl;
    std::cout << "Risultati uguali: " << (same_matrix(serial_result, threaded_result) ? "si" : "no") << std::endl;

    return 0;
}
