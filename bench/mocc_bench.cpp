#include "mocc.h"

#include <chrono>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <vector>

static double measure(const std::function<void()>& callback)
{
    const auto start = std::chrono::steady_clock::now();
    callback();
    const auto end = std::chrono::steady_clock::now();
    return std::chrono::duration<double, std::milli>(end - start).count();
}

static void print_benchmark_result(const double vector_time, const double mocc_time)
{

    auto diff = mocc_time - vector_time;
    auto percentage_diff = (diff / vector_time) * 100.0;

    if (vector_time < mocc_time)
    {
        std::cout << " +++ std::vector: " << vector_time << " ms" << std::endl;
        std::cout << "     mocc       : " << mocc_time << " ms" << std::endl;
    }
    else
    {
        std::cout << "     std::vector: " << vector_time << " ms" << std::endl;
        std::cout << " +++ mocc       : " << mocc_time << " ms" << std::endl;
    }

    std::cout << "     Delta      : " << diff << " ms (" << percentage_diff << "%)" << std::endl;

    std::cout << std::endl;
}

static void benchmark_push_back(size_t count)
{
    std::cout << "Benchmark: push_back(" << count << ")" << std::endl;

    std::vector<int> vec;
    mocc_object* mocc = nullptr;
    mocc_ctor(sizeof(int), 0, &mocc);
    int value;

    const double vector_time = measure(
        [&]()
        {
            for (size_t i = 0; i < count; ++i)
            {
                vec.push_back(static_cast<int>(i));
            }
        });

    const double mocc_time = measure(
        [&]()
        {
            for (size_t i = 0; i < count; ++i)
            {
                value = static_cast<int>(i);
                mocc_push_back(mocc, &value);
            }
        });

    mocc_dtor(mocc);
    print_benchmark_result(vector_time, mocc_time);
}

static void benchmark_random_access(size_t count)
{
    std::cout << "Benchmark: sequential access(" << count << ")" << std::endl;

    std::vector<int> vec;

    for (size_t i = 0; i < count; ++i)
    {
        vec.push_back(static_cast<int>(i));
    }

    mocc_object* mocc = nullptr;
    mocc_ctor(sizeof(int), 0, &mocc);
    int value;
    int* pointer = nullptr;

    for (size_t i = 0; i < count; ++i)
    {
        value = static_cast<int>(i);
        mocc_push_back(mocc, &value);
    }

    const double vector_time = measure(
        [&]()
        {
            volatile int sum = 0;
            for (size_t i = 0; i < count; ++i)
            {
                sum += vec[i];
            }
            (void)sum;
        });

    const double mocc_time = measure(
        [&]()
        {
            volatile int sum = 0;
            for (size_t i = 0; i < count; ++i)
            {
                mocc_at(mocc, i, reinterpret_cast<void**>(&pointer));
                sum += *pointer;
            }
            (void)sum;
        });

    mocc_dtor(mocc);
    print_benchmark_result(vector_time, mocc_time);
}

static void benchmark_erase_middle(size_t count)
{
    std::cout << "Benchmark: erase middle(" << count << ")" << std::endl;
    std::vector<int> vec;
    vec.reserve(count);
    for (size_t i = 0; i < count; ++i)
    {
        vec.push_back(static_cast<int>(i));
    }

    mocc_object* mocc = nullptr;
    mocc_ctor(sizeof(int), count, &mocc);
    size_t index;
    for (size_t i = 0; i < count; ++i)
    {
        const int value = static_cast<int>(i);
        mocc_push_back(mocc, &value);
    }

    const double vector_time = measure(
        [&]()
        {
            for (int i = 0; i < 10 && !vec.empty(); ++i)
            {
                vec.erase(vec.begin() + vec.size() / 2);
            }
        });

    const double mocc_time = measure(
        [&]()
        {
            for (int i = 0; i < 10; ++i)
            {
                index = (count - static_cast<size_t>(i)) / 2;
                mocc_erase(mocc, index);
            }
        });

    mocc_dtor(mocc);
    print_benchmark_result(vector_time, mocc_time);
}

static void benchmark_reserve(size_t count)
{
    std::cout << "Benchmark: reserve(" << count << ")" << std::endl;
    std::vector<int> vec;
    mocc_object* mocc = nullptr;
    mocc_ctor(sizeof(int), 0, &mocc);

    const double vector_time = measure([&]() { vec.reserve(count); });

    const double mocc_time = measure([&]() { mocc_reserve(mocc, count); });

    mocc_dtor(mocc);

    print_benchmark_result(vector_time, mocc_time);
}

int main()
{
    const size_t count = 500000;

    std::cout << "mocc vs std::vector benchmark" << std::endl;
    std::cout << "--------------------------------" << std::endl;

    benchmark_reserve(count);
    benchmark_push_back(count);
    benchmark_random_access(count);
    benchmark_erase_middle(count);

    return EXIT_SUCCESS;
}
