#include <iostream>
#include <sstream>
#include <thread>
#include <vector>

#include "matrix.h"

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

int main(int argc, char *argv[])
{
    size_t size = STD_SIZE;

    if (argc > 1)
    {
        std::istringstream iss(argv[1]);
        iss >> size;
    }

    matrix MA(size), MB(size), MC(size);

    for (size_t i = 0; i < size; ++i)
    {
        for (size_t j = 0; j < size; ++j)
        {
            MA(i, j) = 1;
            MB(i, j) = i + j + 1;
        }
    }

    if (size < MAX_PRINT_SIZE)
    {
        std::cout << "Matrix Ma:" << std::endl << MA;
        std::cout << "Matrix Mb:" << std::endl << MB;
    }

    std::vector<std::thread> workers;
    workers.reserve(size);

    for (size_t row = 0; row < size; ++row)
    {
        workers.emplace_back(compute_row, row, std::cref(MA), std::cref(MB), std::ref(MC));
    }

    for (std::thread &worker : workers)
    {
        worker.join();
    }

    if (size < MAX_PRINT_SIZE)
    {
        std::cout << "Matrix Mc (Ma * Mb):" << std::endl << MC;
    }

    return 0;
}
