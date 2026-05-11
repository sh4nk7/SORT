#include <algorithm>
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

void compute_row_block(size_t first_row, size_t last_row, const matrix &M1, const matrix &M2, matrix &Res)
{
    for (size_t row = first_row; row < last_row; ++row)
    {
        compute_row(row, M1, M2, Res);
    }
}

void serial_product(const matrix &M1, const matrix &M2, matrix &Res)
{
    compute_row_block(0, Res.size(), M1, M2, Res);
}

void threaded_product(size_t thread_count, const matrix &M1, const matrix &M2, matrix &Res)
{
    const size_t size = Res.size();
    thread_count = std::max<size_t>(1, std::min(thread_count, size));

    std::vector<std::thread> workers;
    workers.reserve(thread_count);

    const size_t rows_per_thread = size / thread_count;
    const size_t extra_rows = size % thread_count;
    size_t first_row = 0;

    for (size_t thread_index = 0; thread_index < thread_count; ++thread_index)
    {
        const size_t rows_for_this_thread = rows_per_thread + (thread_index < extra_rows ? 1 : 0);
        const size_t last_row = first_row + rows_for_this_thread;

        workers.emplace_back(compute_row_block, first_row, last_row, std::cref(M1), std::cref(M2), std::ref(Res));
        first_row = last_row;
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

size_t read_argument(char *argument, size_t default_value)
{
    size_t value = default_value;
    std::istringstream iss(argument);
    iss >> value;
    return value;
}

int main(int argc, char *argv[])
{
    size_t size = STD_SIZE;
    size_t thread_count = std::thread::hardware_concurrency();

    if (argc > 1)
    {
        size = read_argument(argv[1], STD_SIZE);
    }

    if (argc > 2)
    {
        thread_count = read_argument(argv[2], thread_count);
    }

    if (thread_count == 0)
    {
        thread_count = 1;
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
    threaded_product(thread_count, MA, MB, threaded_result);
    const auto threaded_end = std::chrono::high_resolution_clock::now();

    const std::chrono::duration<double, std::milli> serial_time = serial_end - serial_start;
    const std::chrono::duration<double, std::milli> threaded_time = threaded_end - threaded_start;
    const size_t used_threads = std::max<size_t>(1, std::min(thread_count, size));

    if (size < MAX_PRINT_SIZE)
    {
        std::cout << "Matrix Mc seriale (Ma * Mb):" << std::endl << serial_result;
        std::cout << "Matrix Mc a blocchi (Ma * Mb):" << std::endl << threaded_result;
    }

    std::cout << "Dimensione matrice: " << size << "x" << size << std::endl;
    std::cout << "Thread usati: " << used_threads << std::endl;
    std::cout << "Tempo seriale: " << serial_time.count() << " ms" << std::endl;
    std::cout << "Tempo multithread a blocchi: " << threaded_time.count() << " ms" << std::endl;
    std::cout << "Risultati uguali: " << (same_matrix(serial_result, threaded_result) ? "si" : "no") << std::endl;

    return 0;
}
